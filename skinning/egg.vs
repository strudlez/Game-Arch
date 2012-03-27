#version 150

in vec4 in_Position;
in vec2 in_Tex;
in vec4 in_joint;
in vec4 in_weight;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 Joints[128];

out vec2 v_tex;

void main( void )
{
    vec3 p = in_Position.xyz;        // original position

    vec4 anim = vec4 (0, 0, 0, 0);
    vec4 pos = vec4(p, 1.0);

    ivec4 jointIndex = ivec4(in_joint);
    for (int i = 0; i < 4; i++)
    {
      int ind = jointIndex[i];
      mat4 mat = Joints[ind];
      float weight = in_weight[i];
      vec4 add = mat * pos * weight;
      anim += add;
    }
    if(in_weight[0] == 0.0f) anim = pos;
    anim = pos;

  // continue the transformation.
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * anim;
  v_tex = in_Tex;
}
