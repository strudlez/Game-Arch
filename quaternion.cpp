#include "quaternion.h"
#include <iostream>
#include <math.h>

using namespace std;

void Quaternion::set(float nX, float nY, float nZ, float nW) {
  x = nX;
  y = nY;
  z = nZ;
  w = nW;
}

void Quaternion::print() { 
  cout<<"("<<x<<", "<<y<<", "<<z<<", "<<w<<")\n";
}

Quaternion& Quaternion::operator =(const Quaternion* b) {
  operator =(*b);
}
Quaternion& Quaternion::operator =(const Quaternion& b) {
  x = b.x;
  y = b.y;
  z = b.z;
  w = b.w;
}

Quaternion Quaternion::operator *(const Quaternion& b) const {
  Quaternion ret;
  ret.x = x*b.w + w*b.x + y*b.z - z*b.y;
  ret.y = y*b.w + w*b.y + z*b.x - x*b.z;
  ret.z = z*b.w + w*b.z + x*b.y - y*b.x;
  ret.w = w*b.w + x*b.x + y*b.y - z*b.z;

  return ret;
}

Quaternion Quaternion::operator *(float b) const {
  Quaternion ret;
  ret.x = x * b; 
  ret.y = y * b; 
  ret.z = z * b; 
  ret.w = w * b; 

  return ret;
}

Quaternion& Quaternion::operator *=(const Quaternion& b) {
  float nX, nY, nZ;
  nX = x*b.w + w*b.x + y*b.z - z*b.y;
  nY = y*b.w + w*b.y + z*b.x - x*b.z;
  nZ = z*b.w + w*b.z + x*b.y - y*b.x;
  w = w*b.w + x*b.x + y*b.y - z*b.z;

  x = nY;
  y = nY;
  z = nZ;
}

Quaternion& Quaternion::operator *=(float f) {
  x *= f;
  y *= f;
  z *= f;
  w *= f;
}

Matrix Quaternion::slerp(const Quaternion& q, float b) {
  Quaternion ret, temp;
  float wP, wQ, a, sinA;

  /*a = acos(p.dot(q));

  sinA = sin(a);

  wP = sin((1 - b) * a) / sinA;
  wQ = sin(b * a) / sinA;

  temp = q * wQ;*/

  /*ret = this * wP;
  ret += temp;

  return ret;*/
}


Matrix Quaternion::slerp(const Matrix& q, float b) {
  Quaternion ret, temp;
  float wP, wQ, a, sinA;

  /*a = acos(p.dot(q));

  sinA = sin(a);

  wP = sin((1 - b) * a) / sinA;
  wQ = sin(b * a) / sinA;

  temp = q * wQ;
  ret = p * wP;
  ret += temp;

  return ret;*/
}

Matrix Quaternion::toMatrix() {
  Matrix ret;

  float xy, xz, xw, yy, yz, yw, zw;

  xy = x*y;
  xz = x*z;
  xw = x*w;
  yy = y*y;
  yz = y*z;
  yw = y*w;
  zw = z*w;

  ret.data[0][0] = 1 - 2 * (yy + yz);
  ret.data[1][0] = 2 * (xy - zw);
  ret.data[2][0] = 2 * (xz + yw);

  ret.data[0][1] = 2 * (xy + zw);
  ret.data[1][1] = 1 - 2 * (xy + yz);
  ret.data[2][1] = 2 * (yz - xw);
  
  ret.data[0][2] = 2 * (xz - yw);
  ret.data[1][2] = 2 * (yz + xw);
  ret.data[2][2] = 1 - 2 * (xy + yy);

  return ret;
}
