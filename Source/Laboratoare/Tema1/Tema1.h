#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
	bool checkCollisions(float ballY, float ballX, float bposX, float bposY, float radius);

protected:
	glm::mat3 modelMatrix;
	float scaleX, scaleY;
	float mvx, mvy;
	float vellx = 0;
	float velly = 0;
	float ballX = 640, ballY = 35;
	float radius = 10;
	typedef struct {
		float x = 0;
		float y = 0;
		bool started = false;
		float rotate = 0;
	} powerUp;
	typedef struct {
		float x;
		float y;
		bool alive = true;
		int length = 80;
		int width = 20;
		float scaleX = 100, scaleY = 100;
		bool scale = false;
		bool doneScaling = false;
		bool powerUP1 = false;
		bool powerUP2 = false;
		bool rotate = false;
		powerUp p1;
		powerUp p2;
	} Brick;
	bool pwrUpActive = false;
	bool pwrUp2Active = false;
	bool newpwrUp = false;
	int startTimePwP;
	int nrLifes;
	bool inAir = false;
	Brick bricks[7][10];
	//int nrBricks = 0;
};
