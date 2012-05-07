#pragma once

#include "egg.h"
#include <glm/glm.hpp> //feel free to use your own math library!
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class phys;

class phys {
 public:
  egg* model;
  void Draw(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix, bool wire);
  void calc(glm::mat4* parent, glm::mat4* parent2);
  int Tex;
  int Tex2;
  glm::mat4 LocalMatrix;
  glm::mat4 TurnMatrix;
  glm::mat4 MoveMatrix;
  glm::mat4 ModelMatrix;
};
