#pragma once

#include <noise/noise.h>
#include "egg.h"
#include <GL/glfw.h>
#include <glm/glm.hpp> //feel free to use your own math library!
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class terrain {
 public:
  terrain(int w, int h, float gSize, GLuint s1, GLuint s2);
  void Draw(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix, bool wire, glm::mat4& CarMat);
  void getGridPos(float x, float y, int& gridX, int& gridY);
  float getHeight(int x, int y);
  
  glm::mat4 ModelMatrix;

  int width;
  int height;
  int gridSize;
  int off;
  int offDiff;
  egg* model;
  GLuint tex;
  GLuint texWire;
};
