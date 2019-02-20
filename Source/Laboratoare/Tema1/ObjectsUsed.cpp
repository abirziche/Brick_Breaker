#include "ObjectsUsed.h"

#include <Core/Engine.h>

Mesh* ObjectsUsed::CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float length, float height, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, height, 0), color),
		VertexFormat(corner + glm::vec3(0, height, 0), color)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		rectangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}

Mesh* ObjectsUsed::CreateCircle(std::string name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color, bool fill) 
{
	int nrPuncte = 360;
	std::vector<VertexFormat> vertices = 
	{
		VertexFormat(leftBottomCorner, color),
	};

	for (int i = 1; i <= nrPuncte; i++) {
		/*vertices.push_back(VertexFormat(glm::vec3(
			sin((float)i / nrPuncte * pi2),
			cos((float)i / nrPuncte * pi2), 0),

			glm::vec3(sin((float)i / nrPuncte * pi2) * radius + .5f, cos((float)i / nrPuncte * pi2) * radius + .5f, 1)));
	
	*/
		vertices.push_back(VertexFormat(glm::vec3(sin((float)i / nrPuncte * M_PI * 2) * radius, cos((float)i / nrPuncte * M_PI * 2)
			* radius, 1), color));
	}

	Mesh* circle = new Mesh(name);

	std::vector<unsigned short> indices =
	{
		
	};

	for (int i = 1; i < nrPuncte; i ++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}
	indices.push_back(0);
	indices.push_back(nrPuncte);
	indices.push_back(1);

	if (!fill) {
		circle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		
	}
	
	circle->InitFromData(vertices, indices);
	return circle;
}
