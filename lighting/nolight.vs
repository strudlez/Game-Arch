#version 150

in vec4 in_Position;
in vec2 in_Tex;
in vec3 in_Normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

out vec2 v_tex;

void main(void)
{
	gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * in_Position;
	v_tex = in_Tex;
}