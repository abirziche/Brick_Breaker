#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform float angle;
uniform float t;

uniform vec3 object_color;

layout(location = 0) out vec4 out_color;

void main()
{
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	vec3 N = normalize(world_normal);
	float light;

	// TODO: define ambient light component
	float ambient_light = 0.25;


	// TODO: compute diffuse light component
	float diffuse_light = max(dot(L, N), 0) * material_kd;

	// TODO: compute specular light component

	float specular_light = 0;
	if (diffuse_light > 0)
	{
		vec3 R = reflect(-L, N);
		specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
	}

	// TODO: compute light
	if (t > 0)  {
	float cut_off = angle;
	float light_att_factor;
	float spot_light = dot(-L, light_direction);
	if (spot_light > cos(cut_off))
	{
		float spot_light_limit = cos(cut_off);

		// Quadratic attenuation
		float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
		light_att_factor = pow(linear_att, 2);
		light = (light_att_factor * (diffuse_light + specular_light));
	}
	}
	else 
		light = ambient_light + diffuse_light+ specular_light;

	// TODO: send color light output to fragment shader
	vec3 color = object_color * light;

	// TODO: write pixel out color
	out_color = vec4(color, 1);
}