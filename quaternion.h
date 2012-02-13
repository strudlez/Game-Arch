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
  Quaternion operator +(const Quaternion& b) const;
  Quaternion operator *(const Quaternion& b) const;
  Quaternion operator *(float b) const;
  Quaternion& operator +=(const Quaternion& b);
  Quaternion& operator *=(const Quaternion& b);
  Quaternion& operator *=(float f);

  float dot(const Quaternion & b) const;
  

  Matrix slerp(const Quaternion& p, const Quaternion& q, float b);
  Matrix slerp(const Quaternion& q, float b);
  Matrix slerp(const Matrix& q, float b);
  Matrix toMatrix() const;

  void print() const;
};
