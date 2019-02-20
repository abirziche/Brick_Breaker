#pragma once
#include <Component/SimpleScene.h>
#include "Camera.h"
#include "TransformD3.h"
#include "Objs.h"

class Tema2 : public SimpleScene
{
public:
	Tema2();
	~Tema2();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;
	void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, glm::vec3 color);
	void RenderSimpleMeshCue(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, glm::vec3 color);
	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	Tema::Camera *camera, *cameraTP;
	glm::mat4 projectionMatrix;
	bool renderCameraTarget;
	float fov;
	float ballX = 0, ballZ = 0;
	bool moved = false, cueApp = false;
	typedef struct {
		float x;
		float y;
	} balls;
	float cueX = 0, cueZ = 0;
	bool mouseM = false;
	float mY = 3.14f;
	float aux;
	bool leftButtonPressed = false;
	bool rightButtonPressed = false;
	float mX = 0;
	float velX = 0, velZ = 0;
	float timez = 0;
	bool hitBall = false;
};
