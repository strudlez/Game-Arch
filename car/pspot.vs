#version 150

in vec4 in_Position;
in vec2 in_Tex;
in vec3 in_Normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

const vec3 LightPosition = vec3(10.0, 0.0, 0.0); 

out vec2 v_tex;
out vec4 out_Position;
out vec3 HV;
out vec3 VP;
out float d;
out vec3 tnorm;

void main(void)
{   
  vec3 ecPos = (ViewMatrix * ModelMatrix * in_Position).xyz;
  vec3 eye = -normalize(ecPos);
  tnorm = normalize(NormalMatrix * in_Normal);

  //vector from surface to light position
  VP = vec3(LightPosition - ecPos);
  //distance from light to surface
  d = length(VP);
  //normalize VP (aux)
  VP = normalize(VP);
  HV = normalize(VP + eye);

  vec3 p = in_Position.xyz;        // original position
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4( p, 1 );
  v_tex = in_Tex;
}
