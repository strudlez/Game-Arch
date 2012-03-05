#version 150

in vec2 v_tex;
in float nDotVP;
in float pf;
in float attenuation;
uniform sampler2D s_tex;

out vec4 FragColor;

const vec3 ambient = vec3(.05, 0.05, 0.05);
const vec3 diffuse = vec3(0.7, 0.7, 0.9);
const vec3 specular = vec3(0.7, 0.7, 0.7);

void main(void)
{
	FragColor = texture(s_tex, v_tex) * vec4((ambient * attenuation + diffuse*nDotVP * attenuation + specular * pf * attenuation), 1);
}