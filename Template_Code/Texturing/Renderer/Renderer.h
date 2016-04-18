#ifndef Renderer_h__
#define Renderer_h__

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <memory>


#include "Shaders/shader.hpp"
#include "Texture/texture.h"
#include "Model/Model.h"
#include "EulerCamera/EulerCamera.h"

class Renderer
{
	enum RenderingMode
	{
		NO_TEXTURE,
		TEXTURE_ONLY,
		BLEND
	};

	GLuint programID;

	// Transformation
	GLuint MatrixID;

	std::unique_ptr<Model> myTriangle;
	std::unique_ptr<Model> mySquare;


	std::unique_ptr<EulerCamera> myCamera;

	std::unique_ptr<Texture> mTexture1;
	std::unique_ptr<Texture> mTexture2;

	GLuint mRenderingModeID;
	RenderingMode mRenderingMode;


	std::unique_ptr<Model> Square;
	glm::mat4 squareM;

	glm::mat4 floorM;




public:
	Renderer();
	~Renderer();

	void Initialize();
	void Draw();
	void HandleKeyboardInput(int key);
	void HandleMouse(double deltaX,double deltaY);
	void Update(double deltaTime);
	void Cleanup();

	void initShader();
	void initTransformation();
	void createSquare(std::unique_ptr<Model> &Square);

};

#endif // Renderer_h__

