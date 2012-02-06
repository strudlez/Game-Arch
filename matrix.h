#pragma once

#include"vector3.h"
#include"quaternion.h"

class Quaternion;
class Vector3;

class Matrix {
 public:
  float data[4][4];

  Matrix();

  void print();

  Matrix& operator =(const Matrix& m);
  Matrix operator *(const Matrix& m) const;
  Matrix operator *(const Matrix* m) const;
  Matrix& operator +=(const Matrix& m);
  Matrix& operator *=(const Matrix& m);


  void apply(Vector3* v);
  void reset();
  void translateX(float x);
  void translateY(float y);
  void translateZ(float z);
  void rotateH(float h);
  void rotateP(float p);
  void rotateR(float r);
  void scale(float s);
  void translateXYZ(float x, float y, float z);
  void translateXYZ(const Vector3& v);
  void rotateHPR(float h, float p, float r);
  void transformXYZHPRS(float x, float y, float z,
      float h, float p, float r, float s);
  Quaternion toQuat();

 protected:
  int width, height;
};

