#version 150

in vec4 in_Position;
in vec2 in_Tex;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec2 v_tex;

void main( void )
{
  vec3 p = in_Position.xyz;        // original position

  // continue the transformation.
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4( p, 1.0 );
  v_tex = in_Tex;
}
