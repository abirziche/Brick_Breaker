#version 330
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_tex;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 sphere_color;
uniform float timez;
uniform vec3 fwd;
out vec3 frag_color;

void main()
{
	frag_color = sphere_color;
	gl_Position = Projection * View * (Model * vec4(v_position + vec3(sin(timez) / 15,0,0) , 1.0));
}
