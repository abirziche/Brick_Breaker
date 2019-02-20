#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform.h"
#include "ObjectsUsed.h"
#include <ctime>

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	radius = 10;

	Mesh* recUP = ObjectsUsed::CreateRectangle("recUP", corner, resolution.x, resolution.y, glm::vec3(1, 0, 0), true);
	AddMeshToList(recUP);

	Mesh* recLR = ObjectsUsed::CreateRectangle("recLR", corner, 20, resolution.y - 50, glm::vec3(1, 0, 0), true);
	AddMeshToList(recLR);

	Mesh* pad = ObjectsUsed::CreateRectangle("pad", corner, 200, 10, glm::vec3(0, 1, 0), true);
	AddMeshToList(pad);

	Mesh* circle = ObjectsUsed::CreateCircle("ball", corner, radius, glm::vec3(1, 1, 1), true);
	AddMeshToList(circle);

	Mesh* circle1 = ObjectsUsed::CreateCircle("ballUP", corner, radius * 2, glm::vec3(1, 1, 1), true);
	AddMeshToList(circle1);

	Mesh *brick = ObjectsUsed::CreateRectangle("brick", corner, 80, 20, glm::vec3(1, 0, 0), true);
	AddMeshToList(brick);

	Mesh *powerUp1 = ObjectsUsed::CreateRectangle("powerUp1", corner, 20, 20, glm::vec3(1, 1, 0), true);
	AddMeshToList(powerUp1);

	Mesh *powerUp2 = ObjectsUsed::CreateRectangle("powerUp2", corner, 20, 20, glm::vec3(0, 0, 1), true);
	AddMeshToList(powerUp2);

	int i, j, k;
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 10; j++) {
			bricks[i][j].alive = true;
			bricks[i][j].scale = false;
			bricks[i][j].doneScaling = false;
			bricks[i][j].rotate = false;
			bricks[i][j].powerUP1 = false;
			bricks[i][j].powerUP2 = false;
			bricks[i][j].p1.rotate = 0;
			bricks[i][j].p1.started = false;
			bricks[i][j].p2.rotate = 0;
			bricks[i][j].p2.started = false;
			bricks[i][j].scaleX = 100;
			bricks[i][j].scaleY = 100;
			bricks[i][j].x = (float)(130 + j * 80 + 20 * (j + 1));
			bricks[i][j].y = (float)(300 + i * 20 + 20 * (i + 1));
		}
	}

	for (k = 0; k < 5; k++) {
		i = rand() % 7;
		j = rand() % 10;
		bricks[i][j].powerUP1 = true;
		bricks[i][j].p1.x = bricks[i][j].x;
		bricks[i][j].p1.y = bricks[i][j].y;
	}

	for (k = 0; k < 3; k++) {
		i = rand() % 7;
		j = rand() % 10;
		bricks[i][j].powerUP2 = true;
		bricks[i][j].p2.x = bricks[i][j].x;
		bricks[i][j].p2.y = bricks[i][j].y;
	}

	mvx = 530;
	mvy = 50;
	nrLifes = 3;
	ballY = 35;
	velly = 0;
	vellx = 0;
	inAir = false;
	pwrUpActive = false;
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->GetResolution();
	ballY = ballY + velly * deltaTimeSeconds;
	ballX = ballX + vellx * deltaTimeSeconds;

	// Randare perete superior
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform::Translate(0, resolution.y - 20);
	RenderMesh2D(meshes["recUP"], shaders["VertexColor"], modelMatrix);

	// Randare pereti laterali
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform::Translate(0, 50);
	RenderMesh2D(meshes["recLR"], shaders["VertexColor"], modelMatrix);
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform::Translate(resolution.x - 20, 50);
	RenderMesh2D(meshes["recLR"], shaders["VertexColor"], modelMatrix);
	
	// Verificare pozitia platformei sa nu depaseasca marginile
	if (mvx - 100 <= 0)
		mvx = 100;
	if (mvx + 100 > resolution.x)
		mvx = resolution.x - 100;

	// Randare platforma
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform::Translate(mvx - 100, 10);
	RenderMesh2D(meshes["pad"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	
	// Verificare lovire perete superior
	if (ballY >= resolution.y - radius - 20 && velly > 0)
		velly = -velly;

	bool ok1 = false;
	if ((ballY <= 15 || ballX <= 20 || ballX >= resolution.x - 20) && inAir) {
		nrLifes--;
		ok1 = true; // a cazut
		ballY = 35;
		velly = 0;
		vellx = 0;
		inAir = false;
		if (pwrUpActive) {
			radius = 10;
			pwrUpActive = false;
		}
		if (nrLifes == 0) {
			Tema1::Init();
		}
	}

	// Verificare lovire platforma
	if (ballY < 25 + radius && (mvx - 100 <= ballX && mvx + 100 >= ballX) && !ok1) {
		if (inAir) {
			double rez = (ballX - mvx) / 100;
			vellx = 300 * rez;
			velly = 300 * sqrt(1 - rez * rez);
		}
	}

	// Verificare lovire pereti laterali
	if (ballX <= 20 + radius && vellx < 0 && ballY > 50 - radius)
		vellx = -vellx;
	if (ballX >= resolution.x - radius - 20 && vellx > 0 && ballY > 50 - radius)
		vellx = -vellx;

	// Verificare daca bila e pe platforma sau in aer
	if (inAir)
		modelMatrix *= Transform::Translate(ballX, ballY);
	else
		modelMatrix *= Transform::Translate(mvx, ballY);
	
	time_t now = time(0);
	tm *ltm = localtime(&now);
	
	if (newpwrUp == true) {
		startTimePwP = ltm->tm_sec;
		newpwrUp = false;
	}

	if (pwrUpActive == true && abs(ltm->tm_sec - startTimePwP) < 30) {
		RenderMesh2D(meshes["ballUP"], shaders["VertexColor"], modelMatrix);		
	}
	else {
		RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
		pwrUpActive = false;
		radius = 10;
	}

	if (pwrUp2Active == true) {
		nrLifes++;
		pwrUp2Active = false;
	}

	int i, j;
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 10; j++) {
			if (bricks[i][j].alive == true) {
				if (!checkCollisions(ballY, ballX, bricks[i][j].x, bricks[i][j].y, radius)) {
					modelMatrix = glm::mat3(1);
					modelMatrix *= Transform::Translate(bricks[i][j].x, bricks[i][j].y);
					RenderMesh2D(meshes["brick"], shaders["VertexColor"], modelMatrix);
				}
				else {
					modelMatrix = glm::mat3(1);
					ballY = ballY + velly * deltaTimeSeconds;
					ballX = ballX + vellx * deltaTimeSeconds;
					modelMatrix *= Transform::Translate(ballX, ballY);
					RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
					bricks[i][j].scale = true;
					bricks[i][j].alive = false;
					if (bricks[i][j].powerUP1 == true || bricks[i][j].powerUP2 == true)
						bricks[i][j].rotate = true;
				}
			}

			// Scalare la coliziunea cu caramizile
			if (bricks[i][j].scale == true && bricks[i][j].doneScaling == false) {
				bricks[i][j].scaleY -= deltaTimeSeconds * 100;
				bricks[i][j].scaleX -= deltaTimeSeconds * 100;

				glm::mat3 translateBack = Transform::Translate(bricks[i][j].x, bricks[i][j].y);
				modelMatrix = glm::mat3(1);
				glm::mat3 scaleM = Transform::Scale(bricks[i][j].scaleX / 100, bricks[i][j].scaleY / 100);

				RenderMesh2D(meshes["brick"], shaders["VertexColor"], translateBack * modelMatrix * scaleM);
				if (bricks[i][j].scaleY < 0 && bricks[i][j].scaleX < 0)
					bricks[i][j].doneScaling = true;
			}

			// Rotatia power-ups urilor
			if (bricks[i][j].rotate == true) {

				if (bricks[i][j].powerUP1 == true) {
					modelMatrix = glm::mat3(1);
					bricks[i][j].p1.rotate += deltaTimeSeconds * 3;
					bricks[i][j].p1.y -= 100 * deltaTimeSeconds;

					modelMatrix *= Transform::Translate(bricks[i][j].p1.x + 20 / 2.f, bricks[i][j].p1.y + 20 / 2.f);
					modelMatrix *= Transform::Rotate(bricks[i][j].p1.rotate);
					modelMatrix *= Transform::Translate(-20 / 2.f, -20 / 2.0f);

					if (bricks[i][j].p1.y > 15) {

						RenderMesh2D(meshes["powerUp1"], shaders["VertexColor"], modelMatrix);
						if (bricks[i][j].p1.y < 35 && (mvx - 100 <= bricks[i][j].p1.x && mvx + 100 >= bricks[i][j].p1.x)) {
							pwrUpActive = true;
							time_t now = time(0);
							tm *ltm = localtime(&now);
							if (bricks[i][j].p1.started == false) {
								radius = 20;
								startTimePwP = ltm->tm_sec;
								pwrUpActive = true;
								bricks[i][j].p1.started = true;
								newpwrUp = true;
								if (!inAir) {
									vellx = 0;
									velly = 0;
									ballY = 35;
								}
							}
						}
					}
				}
					
				if (bricks[i][j].powerUP2 == true) {
					modelMatrix = glm::mat3(1);
					bricks[i][j].p2.rotate += deltaTimeSeconds * 3;
					bricks[i][j].p2.y -= 100 * deltaTimeSeconds;

					modelMatrix *= Transform::Translate(bricks[i][j].p2.x + 20 / 2.f, bricks[i][j].p2.y + 20 / 2.f);
					modelMatrix *= Transform::Rotate(bricks[i][j].p2.rotate);
					modelMatrix *= Transform::Translate(-20 / 2.f, -20 / 2.0f);
					if (bricks[i][j].p2.y > 15) {
						RenderMesh2D(meshes["powerUp2"], shaders["VertexColor"], modelMatrix);
						if (bricks[i][j].p2.y < 35 && (mvx - 100 <= bricks[i][j].p2.x && mvx + 100 >= bricks[i][j].p2.x)) {
							if (bricks[i][j].p2.started == false) {
								pwrUp2Active = true;
								bricks[i][j].p2.started = true;
							}
						}
					}
				}
			}

		}
	}
	
	// Randare vieti
	for (i = 0; i < nrLifes; i++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform::Translate(20 * i + 20, 10);
		RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
	}

	// Verificare terminare joc
	int nrBricks = 0;
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 10; j++) {
			if (bricks[i][j].alive == true)
				nrBricks++;
		}
	}
	if (nrBricks == 0)
		Tema1::Init();

}

bool Tema1::checkCollisions(float ballY, float ballX, float bposX, float bposY, float radius)
{
	if (inAir) {
		if (ballX >= bposX && (ballX <= bposX + 80) && (ballY <= bposY) && (ballY >= bposY - radius)) {
			velly = -velly;
			return true;
		}
		if (ballX >= bposX && (ballX <= bposX + 80) && (ballY <= bposY + 20 + radius) && (ballY >= bposY + 20)) {
			velly = -velly;
			return true;
		}
		if (ballX >= bposX - radius && (ballX <= bposX) && (ballY >= bposY) && (ballY <= bposY + 20)) {
			vellx = -vellx;
			return true;
		}
		if ((ballX >= bposX + 80) && (ballX <= bposX + 80 + radius) && (ballY >= bposY) && (ballY <= bposY + 20)) {
			vellx = -vellx;
			return true;
		}
		if ((ballX >= bposX - radius) && (ballX <= bposX) && (ballY <= bposY) && (ballY >= bposY - radius)) {
			if (vellx > 0 && velly > 0) {
				vellx = -vellx;
				velly = -velly;
			}
			else if (vellx > 0) {
				vellx = -vellx;
			}
			else if (velly > 0) {
				velly = -velly;
			}
			return true;
		}
		if ((ballX >= bposX + 80) && (ballX <= bposX + 80 + radius) && (ballY <= bposY) && (ballY >= bposY - radius)) {
			if (vellx > 0 && velly > 0) {
				velly = -velly;
			}
			else if (vellx < 0 && velly < 0) {
				vellx = -vellx;
			}
			else if (velly > 0) {
				vellx = -vellx;
				velly = -velly;
			}
			return true;
		}
		if ((ballX >= bposX - radius) && (ballX <= bposX) && (ballY <= bposY + 20 + radius) && (ballY >= bposY + 20)) {
			if (vellx > 0 && velly > 0) {
				vellx = -vellx;
			}
			else if (vellx > 0) {
				vellx = -vellx;
				velly = -velly;
			}
			else if (velly < 0 && vellx < 0) {
				velly = -velly;
			}
			return true;
		}
		if ((ballX >= bposX + 80) && (ballX <= bposX + 80 + radius) && (ballY <= bposY + 20 + radius) && (ballY >= bposY + 20)) {
			if (vellx > 0) {
				velly = -velly;
			}
			else if (vellx < 0 && velly < 0) {
				vellx = -vellx;
				velly = -velly;
			}
			else if (velly > 0) {
				vellx = -vellx;
			}
			return true;
		}
	}
	return false;
}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{

}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (mouseX > 0) {
		mvx = mouseX;
		if (!inAir)
			ballX = mvx;
	}
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	if (button == 1 && !inAir) {
		velly = 300;
		inAir = true;
	}
		
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
