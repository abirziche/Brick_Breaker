#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>


using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	renderCameraTarget = false;

	camera = new Tema::Camera();
	camera->Set(glm::vec3(1, 3, 0.5f), glm::vec3(5, -35, 0.5f), glm::vec3(0, 1, 0));
	glm::vec3 corner = glm::vec3(0, 0, 0);
	Mesh* table = Objs::CreateParallelipiped("table", corner, 2, 0.5f, 1, glm::vec3(0, 174.f/255, 0), true);
	AddMeshToList(table);

	Mesh* wall1 = Objs::CreateParallelipiped("wall1", glm::vec3(0,0.5f,0), 2, 0.055f, 0.05f, glm::vec3(136.f/255, 66.f/255, 29.f/255), true);
	AddMeshToList(wall1);
	Mesh* wall2 = Objs::CreateParallelipiped("wall2", glm::vec3(0, 0.5f, 0.95f), 2, 0.055f, 0.05f, glm::vec3(136.f / 255, 66.f / 255, 29.f / 255), true);
	AddMeshToList(wall2);
	Mesh* wall3 = Objs::CreateParallelipiped("wall3", glm::vec3(0, 0.5f, 0), 0.05f, 0.055f, 1, glm::vec3(136.f / 255, 66.f / 255, 29.f / 255), true);
	AddMeshToList(wall3);
	Mesh* wall4 = Objs::CreateParallelipiped("wall4", glm::vec3(1.95f, 0.5f, 0), 0.05f, 0.055f, 1, glm::vec3(136.f / 255, 66.f / 255, 29.f / 255), true);
	AddMeshToList(wall4);
	//Mesh* cue = Objs::CreateParallelipiped("cue", glm::vec3(0, 0, 0), 1, 0.01f, 0.01f, glm::vec3(139.f / 255, 69.f / 255, 19.f / 255), true);
	//AddMeshToList(cue);

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("cue");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "cue.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Shader *shader = new Shader("ShaderH2");
		shader->AddShader("Source/Laboratoare/Tema2/VertexSh.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/FragmentSh.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	ballX = 0.5f;
	ballZ = 0.5f;
	balls b[15];


	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
	fov = 60.f;
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
		glm::mat4 modelMatrix = glm::mat4(1);
		// Randare masa
		RenderMesh(meshes["table"], shaders["VertexColor"], modelMatrix);
		RenderMesh(meshes["wall1"], shaders["VertexColor"], modelMatrix);
		RenderMesh(meshes["wall2"], shaders["VertexColor"], modelMatrix);
		RenderMesh(meshes["wall3"], shaders["VertexColor"], modelMatrix);
		RenderMesh(meshes["wall4"], shaders["VertexColor"], modelMatrix);
		// Randare gauri
		modelMatrix *= TransformD3::Translate(1, 0.51f, 0.95f);
		modelMatrix *= TransformD3::Scale(0.09f, 0.09f, 0.09f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(0,0,0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(0.06f, 0.51f, 0.94f);
		modelMatrix *= TransformD3::Scale(0.09f, 0.09f, 0.09f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(0, 0, 0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(0.06f, 0.51f, 0.06f);
		modelMatrix *= TransformD3::Scale(0.09f, 0.09f, 0.09f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(0, 0, 0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(1, 0.51f, 0.05f);
		modelMatrix *= TransformD3::Scale(0.09f, 0.09f, 0.09f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(0, 0, 0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(1.93f, 0.51f, 0.07f);
		modelMatrix *= TransformD3::Scale(0.09f, 0.09f, 0.09f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(0, 0, 0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(1.935f, 0.51f, 0.935f);
		modelMatrix *= TransformD3::Scale(0.09f, 0.09f, 0.09f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(0, 0, 0));

		// Randare bila alba
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(ballX, 0.53f, ballZ);
		modelMatrix *= TransformD3::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(1, 1, 1));

		// Randare restul bilelor
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(1.5f, 0.53f, 0.5f);
		modelMatrix *= TransformD3::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(1, 0, 0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(1.545f, 0.53f, 0.525f);
		modelMatrix *= TransformD3::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(1, 0, 0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(1.545f, 0.53f, 0.475f);
		modelMatrix *= TransformD3::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(1, 1, 0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(1.59f, 0.53f, 0.55f);
		modelMatrix *= TransformD3::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(1, 1, 0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(1.59f, 0.53f, 0.5f);
		modelMatrix *= TransformD3::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(0, 0, 0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(1.59f, 0.53f, 0.45f);
		modelMatrix *= TransformD3::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(1, 0, 0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(1.635f, 0.53f, 0.575f);
		modelMatrix *= TransformD3::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(1, 1, 0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(1.635f, 0.53f, 0.525f);
		modelMatrix *= TransformD3::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(1, 0, 0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(1.635f, 0.53f, 0.475f);
		modelMatrix *= TransformD3::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(1, 0, 0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(1.635f, 0.53f, 0.425f);
		modelMatrix *= TransformD3::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(1, 1, 0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(1.68f, 0.53f, 0.6f);
		modelMatrix *= TransformD3::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(1, 0, 0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(1.68f, 0.53f, 0.55f);
		modelMatrix *= TransformD3::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(1, 0, 0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(1.68f, 0.53f, 0.5f);
		modelMatrix *= TransformD3::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(1, 1, 0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(1.68f, 0.53f, 0.45f);
		modelMatrix *= TransformD3::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(1, 1, 0));
		modelMatrix = glm::mat4(1);
		modelMatrix *= TransformD3::Translate(1.68f, 0.53f, 0.4f);
		modelMatrix *= TransformD3::Scale(0.05f, 0.05f, 0.05f);
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderH2"], modelMatrix, glm::vec3(1, 0, 0));
	
		/*modelMatrix = glm::mat4(1);
		if (cueApp == true) {
			modelMatrix *= TransformD3::Translate(ballX - 1.1f, 0.55f, ballZ - 0.008f);
			RenderMesh(meshes["cue"], shaders["VertexColor"], modelMatrix);
		}
		*/
	

	// Render the camera target. Useful for understanding where is the rotation point in Third-person camera movement
	if (renderCameraTarget)
	{
		if (cueApp == true) {
			if (mouseM == false) {
				//camera->Set(glm::vec3(-0.5f, 0.75f, 0.5f), glm::vec3(2, 0.5f, 0.5f), glm::vec3(0, 1, 0));
				camera->Set(glm::vec3(ballX - 1.f, 0.75f, ballZ), glm::vec3(2, 0.5f, 0.5f), glm::vec3(0, 1, 0));
				cueX = ballX - 1.1f;
				cueZ = ballZ - 0.008f;
			}
			//glm::vec3 f = glm::normalize(glm::vec3(2, 0.5f, 0.5f) - glm::vec3(-0.5f, 0.75f, 0.5f));
			
			modelMatrix = glm::mat4(1);
			modelMatrix *= TransformD3::Translate(ballX, 0.55f, ballZ);
			modelMatrix *= TransformD3::RotateOY(mY);
			modelMatrix *= TransformD3::Translate(0.1f, -0.02f, -0.001f);
			//RenderMesh(meshes["cue"], shaders["VertexColor"], modelMatrix);
			RenderSimpleMeshCue(meshes["cue"], shaders["ShaderH2"], modelMatrix, glm::vec3(139.f / 255, 69.f / 255, 19.f / 255));
		}
	}

}

void Tema2::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, glm::vec3 color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);
	int Model = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(Model, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	int View = glGetUniformLocation(shader->program, "View");
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glUniformMatrix4fv(View, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	int Projection = glGetUniformLocation(shader->program, "Projection");
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(Projection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	GLint p_obj = glGetUniformLocation(shader->program, "sphere_color");
	glUniform3fv(p_obj, 1, glm::value_ptr(color));
	
	timez = 0;
	int timezPosition = glGetUniformLocation(shader->program, "timez");
	glUniform1f(timezPosition, timez);
	GLint camPosition = glGetUniformLocation(shader->program, "fwd");
	glUniform3fv(camPosition, 1, glm::value_ptr(camera->forward));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::RenderSimpleMeshCue(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, glm::vec3 color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);
	int Model = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(Model, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	int View = glGetUniformLocation(shader->program, "View");
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glUniformMatrix4fv(View, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	int Projection = glGetUniformLocation(shader->program, "Projection");
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(Projection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	GLint p_obj = glGetUniformLocation(shader->program, "sphere_color");
	glUniform3fv(p_obj, 1, glm::value_ptr(color));

	timez = 0;
	GLint camPosition = glGetUniformLocation(shader->program, "fwd");
	glUniform3fv(camPosition, 1, glm::value_ptr(camera->forward));
	if (!strcmp(mesh->GetMeshID(), "cue")) {
		int timezPosition = glGetUniformLocation(shader->program, "timez");
		if (leftButtonPressed == true)
			timez = Engine::GetElapsedTime();
		else timez = 0;
		glUniform1f(timezPosition, timez);
	}
	else
		timez = 0;

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{


	// move the camera only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			// TODO : translate the camera forward
			camera->TranslateForward(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// TODO : translate the camera to the left
			camera->TranslateRight(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO : translate the camera backwards
			camera->TranslateForward(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO : translate the camera to the right
			camera->TranslateRight(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			// TODO : translate the camera down
			camera->TranslateUpword(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// TODO : translate the camera up
			camera->TranslateUpword(deltaTime * cameraSpeed);
		}

	}
	else {
		if (moved == false) {
			if (window->KeyHold(GLFW_KEY_W)) {
				if (ballX + deltaTime < 2.f / 3 - 0.1)
					ballX += deltaTime;
			}
			if (window->KeyHold(GLFW_KEY_A)) {
				if (ballZ - deltaTime > 0.1)
					ballZ -= deltaTime;
			}
			if (window->KeyHold(GLFW_KEY_D)) {
				if (ballZ + deltaTime < 0.9)
					ballZ += deltaTime;
			}
			if (window->KeyHold(GLFW_KEY_S)) {
				if (ballX - deltaTime > 0.1)
					ballX -= deltaTime;
			}
			if (window->KeyHold(GLFW_KEY_SPACE)) {
				moved = true;
				cueApp = true;
				renderCameraTarget = true;
				
			}
		}


	}

}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event
	/*if (key == GLFW_KEY_T)
	{
		renderCameraTarget = !renderCameraTarget;
	}*/

}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;
		leftButtonPressed = false;
		rightButtonPressed = true;
		if (window->GetSpecialKeyState() == 0 && cueApp == false) {
			camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
			camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
		}

		if (window->GetSpecialKeyState() == 0 && cueApp == true) {
			mouseM = true;
			camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
			mY += (-deltaX * sensivityOY);
			
		}
	
	}

}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	if (button == 1 && leftButtonPressed == true) {
		camera->Set(glm::vec3(1, 3, 0.5f), glm::vec3(5, -35, 0.5f), glm::vec3(0, 1, 0));
		//moved = false;
		renderCameraTarget = false;
		cueApp = false;
	}
	if (button == 1 && leftButtonPressed == false) {
		leftButtonPressed = true;
		rightButtonPressed = false;
		mX = mouseX;
	}
	
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
