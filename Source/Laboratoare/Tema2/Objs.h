#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Objs
{
	// Create square with given bottom left corner, length and color
	Mesh* CreateParallelipiped(std::string name, glm::vec3 leftBottomCorner, float length, float height, float width, glm::vec3 color, bool fill = false);
	Mesh* CreateCircle(std::string name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color, bool fill = false);
}
