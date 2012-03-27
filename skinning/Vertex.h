#pragma once

#include <vector>
#include "Joint.h"

using namespace std;

typedef struct Vertex {
  float Position[4];
  float UV[2];
  int jointNum;
  float joints[4];
  float jointInfluence[4];
} Vertex;
