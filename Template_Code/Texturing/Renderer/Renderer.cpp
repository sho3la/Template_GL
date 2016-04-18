#include "Renderer.h"

#include <gtc/matrix_transform.hpp> 
#include <gtx/transform.hpp>

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
	Cleanup();
}

void Renderer::Initialize()
{
	myCamera = std::unique_ptr<EulerCamera>(new EulerCamera());





	createSquare(mySquare);
	floorM =  glm::scale(200.0f,2.0f,200.0f)*glm::translate(0.0f,-0.25f,0.0f)*glm::rotate(90.0f,glm::vec3(1.0f,0.0f,0.0f));



	createSquare(Square);
	squareM =  glm::scale(1.0f,1.0f,1.0f) *glm::translate(0.0f,0.0f,0.0f)*glm::rotate(00.0f,glm::vec3(1.0f,0.0f,0.0f));



	//////////////////////////////////////////////////////////////////////////
	// loading and initalizing textures.
	mTexture1 = std::unique_ptr<Texture>(new Texture("uvtemplate.bmp",0));
	mTexture2 = std::unique_ptr<Texture>(new Texture("bricks.jpg",1));
	//////////////////////////////////////////////////////////////////////////

	initShader();

	initTransformation();




}


void Renderer::Update(double deltaTime)
{
	double triangle1RotationSpeed = 0.06;
	double trianglesRotationAngle = triangle1RotationSpeed*deltaTime;
	glm::mat4 rotationMat = glm::rotate((float)trianglesRotationAngle,glm::vec3(0.0,1.0,0.0));




}



void Renderer::Draw()
{		
	// Use our shader
	glUseProgram(programID);

	//send the rendering mode to the shader.
	mRenderingMode = RenderingMode::TEXTURE_ONLY;
	glUniform1i(mRenderingModeID,mRenderingMode);

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glm::mat4 VP = myCamera->GetProjectionMatrix() * myCamera->GetViewMatrix();



	mTexture1->Bind();
	glm::mat4 squareMVP =  VP * squareM; 
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &squareMVP[0][0]);
	Square->Draw();


	//the floor
	mTexture2->Bind();
	glm::mat4 floorMVP =  VP * floorM; 
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &floorMVP[0][0]);
	mySquare->Draw();
}

void Renderer::HandleKeyboardInput(int key)
{
	printf("Renderer::KEY = %d \n",key);
	switch (key)
	{
		//Moving forward
	case GLFW_KEY_UP:
	case GLFW_KEY_W:
		myCamera->Walk(0.5);
		break;

		//Moving backword
	case GLFW_KEY_DOWN:
	case GLFW_KEY_S:
		myCamera->Walk(-0.5);
		break;

		// Moving right
	case GLFW_KEY_RIGHT:
	case GLFW_KEY_D:
		myCamera->Strafe(0.1);
		break;

		// Moving left
	case GLFW_KEY_LEFT:
	case GLFW_KEY_A:
		myCamera->Strafe(-0.1);
		break;

		// Moving up
	case GLFW_KEY_SPACE:
	case GLFW_KEY_R:
		myCamera->Fly(0.1);
		break;

		// Moving down
	case GLFW_KEY_LEFT_CONTROL:
	case GLFW_KEY_F:
		myCamera->Fly(-0.1);
		break;
	default:
		break;
	}


	//continue the remaining movements.

	myCamera->UpdateViewMatrix();
}

void Renderer::HandleMouse(double deltaX,double deltaY)
{
	printf("Renderer::MOUSE = %f , %f \n",deltaX,deltaY);

	myCamera->Yaw(deltaX);
	myCamera->Pitch(deltaY);
	myCamera->UpdateViewMatrix();

}


void Renderer::createSquare(std::unique_ptr<Model> &Square)
{

	//drawing a square.
	Square = std::unique_ptr<Model>(new Model());

	Square->VertexData.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
	Square->VertexData.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
	Square->VertexData.push_back(glm::vec3( 1.0f,  1.0f, 0.0f));
	Square->VertexData.push_back(glm::vec3( -1.0f,  1.0f, 0.0f));

	Square->ColorData.push_back(glm::vec3(1.0f, 0.0f, 0.5f));
	Square->ColorData.push_back(glm::vec3(0.0f, 0.0f, 0.5f));
	Square->ColorData.push_back(glm::vec3(0.0f, 0.0f, 0.5f));
	Square->ColorData.push_back(glm::vec3(1.0f, 0.0f, 0.5f));

	Square->UVData.push_back(glm::vec2(0.0f,0.0f));
	Square->UVData.push_back(glm::vec2(1.0f,0.0f));
	Square->UVData.push_back(glm::vec2(0.0f,1.0f));
	Square->UVData.push_back(glm::vec2(1.0f,1.0f));

	//first triangle.
	Square->IndicesData.push_back(0);
	Square->IndicesData.push_back(1);
	Square->IndicesData.push_back(3);

	//second triangle.
	Square->IndicesData.push_back(1);
	Square->IndicesData.push_back(2);
	Square->IndicesData.push_back(3);

	Square->Initialize();

}

void Renderer::initShader()
{
	// Create and compile our GLSL program from the shaders
	programID = LoadShaders( "SimpleTransformWithColor.vertexshader", "MultiColor.fragmentshader" );

	//////////////////////////////////////////////////////////////////////////
	mRenderingModeID = glGetUniformLocation(programID,"RenderingMode");
	//////////////////////////////////////////////////////////////////////////
}

void Renderer::initTransformation()
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	// Transformations
	////////////////////
	// Get a handle for our "MVP" uniform the holder we created in the vertex shader)
	// GLint glGetUniformLocation(GLuint program​, const GLchar *name​);
	// program:  Specifies the program objec(t to be queried.
	// name: Points to a null terminated string containing the name of the uniform variable whose location is to be queried. 
	MatrixID = glGetUniformLocation(programID, "MVP");

	//////////////////////////////////////////////////////////////////////////

	// Projection matrix : 
	myCamera->SetPerspectiveProjection(45.0f,4.0f/3.0f,0.1f,100.0f);

	// View matrix : 
	myCamera->Reset(0.0,0.0,5.0,
		0,0,0,
		0,1,0);
	//////////////////////////////////////////////////////////////////////////
}

void Renderer::Cleanup()
{
	glDeleteProgram(programID);
}