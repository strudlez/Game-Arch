#pragma once

#include "vector3.h"
#include "matrix.h"

class Matrix;

class Quaternion {
 public:
  float x, y, z, w;

  void set(float nX, float nY, float nZ, float nW);
  Quaternion& operator =(const Quaternion* b);
  Quaternion& operator =(const Quaternion& b);
  Quaternion operator *(const Quaternion& b) const;
  Quaternion operator *(float b) const;
  Quaternion& operator *=(const Quaternion& b);
  Quaternion& operator *=(float f);
  

  Matrix slerp(const Quaternion& q, float b);
  Matrix slerp(const Matrix& q, float b);
  Matrix toMatrix();

  void print();
};
