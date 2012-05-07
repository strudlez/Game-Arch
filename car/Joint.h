#pragma once

#include <vector>
#include "matrix.h"

using namespace std;

class Joint {
 public:
  Joint();
  void addChild(Joint* j);
  void calculateFrames();
  void runJoint(Matrix& math, float dt);

  Joint* parent;
  vector<Joint*> children;

  Matrix localMat;
  Matrix worldMat;

  int fps;
  long unsigned int maxFrame;
  float frame;
  vector<float> hList;
  vector<float> pList;
  vector<float> rList;
  vector<float> xList;
  vector<float> yList;
  vector<float> zList;
  int num;

  int xN, yN, zN, hN, pN, rN;
};
