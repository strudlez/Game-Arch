#pragma once

#include "phys.h"
#include "terrain.h"
#include "tritest.h"
#include "vector3.h"

class wheel: public phys {
 public:
  wheel(egg* w);
  float rot;
  float spin;
  float turnSpeed;
  float maxTurn;

  float oldHeight;
  float newHeight;

  float radius;

  bool checkCollide(terrain* world);
  glm::vec4 getLoc();
  bool checkCollide(terrain* world, tritest::Sphere& s, Vertex* a, Vertex* b, Vertex* c);

  void calc(glm::mat4* parent, glm::mat4* parent2);

  float weight;

  float speed;

};
