#version 330
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_tex;
layout(location = 3) in vec3 v_color;
// TODO: get vertex attributes from each location

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float timeSinceStart;


// TODO: output values to fragment shader

out vec3 frag_color;
out vec3 frag_normal;
out vec2 frag_tex;

void main()
{
	frag_color = v_color + (0.5 + sin(timeSinceStart * 5)/2) * vec3(0,1,0);
	
	gl_Position = Projection * View * Model * vec4(v_position + sin(timeSinceStart) * vec3(0,1,0), 1.0); // builtin variable

	frag_normal = v_normal;
	frag_tex = v_tex;
}
