#pragma once

#include "matrix.h"

class Matrix;

class Vector3 {
 public:
  float x, y, z, w;
  Vector3();

  void set(float nX, float nY, float nZ);
  void print();
   
  Vector3& operator =(const Vector3& b);
  Vector3 operator +(const Vector3& b) const;
  Vector3 operator *(const Vector3& b) const;
  Vector3 operator *(const float f) const;
  Vector3 operator *(const Matrix& m) const;
  Vector3 operator *(const Matrix* m) const;
  
  Vector3& operator +=(const Vector3& b);
  Vector3& operator *=(const Vector3& b);
  Vector3& operator *=(const float f);
  Vector3& operator *=(const Matrix& m);
  Vector3& operator *=(const Matrix* m);

  float dot(const Vector3& b);

};
