#pragma once

#include "wheel.h"
#include "body.h"
#include "terrain.h"
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp> //feel free to use your own math library!
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class car {
 public:
  car(GLuint s1, GLuint s2);
  body* bod;
  wheel* wheels[4];

  //suspensions
  float susp[4];
  float k;

  void Draw(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix, bool wire);
  void step(int move, int turn, terrain* world);
  bool checkCollide(terrain* world);
  void calcWheels();
  void jump();
  float ModelPitch;
  float ModelYaw;
  float zMov;
  float g;
  float maxSpeed;
  glm::vec3 vel;
  bool onGround;
  
  float speed;
  float acc;
  bool first;
  glm::mat4 ModelMatrix;
  glm::mat4 TurnMatrix;
  glm::mat4 TurnMatrix2;

  //rolling friction
  float rollingC;
  float weight;

};
