#include<stdlib.h>
#include<iostream>
#include"vector3.h"

using namespace std;

Vector3::Vector3() {
}

void Vector3::set(float nX, float nY, float nZ) {
  x = nX;
  y = nY;
  z = nZ;
  w = 0;
}

void Vector3::print() { 
  cout<<"("<<x<<", "<<y<<", "<<z<<")\n";
}

Vector3& Vector3::operator =(const Vector3& b) {
  x = b.x;
  y = b.x;
  z = b.x;
  w = b.x;
}

Vector3 Vector3::operator +(const Vector3& b) const {
  Vector3 ret;

  ret.x = x + b.x;
  ret.y = y + b.y;
  ret.z = z + b.z;
  ret.w = w;

  return ret;
}

Vector3 Vector3::operator *(const Vector3& b) const {
  Vector3 ret;

  ret.x = y*b.z - z*b.y;
  ret.y = z*b.x - x*b.z;
  ret.z = x*b.y - y*b.x;
  ret.w = w;
  
  return ret;
}

Vector3 Vector3::operator *(const float f) const {
  Vector3 ret;

  ret.x = x * f;
  ret.x = y * f;
  ret.x = z * f;
  ret.w = w;

  return ret;
}

Vector3 Vector3::operator *(const Matrix& m) const {
  Vector3 ret;
  float d;
  int width = m.width;
  for(int i=0; i<3; i++) {

    d = m.data[0*width+i] * x + m.data[1*width+i] * y +
        m.data[2*width+i] * z + m.data[3*width+i] * w;
    switch(i)
    {
      case 0: ret.x = d;break;
      case 1: ret.y = d;break;
      case 2: ret.z = d;break;
    }
  }

  ret.w = w;

  return ret;
}

Vector3 Vector3::operator *(const Matrix* m) const{
  return operator *(*m);
}

Vector3& Vector3::operator +=(const Vector3& b) {
  x += b.x;
  y += b.z;
  z += b.z;
}
Vector3& Vector3::operator *=(const Vector3& b) {
  float nX = y*b.z - z*b.y;
  float nY = z*b.x - x*b.z;
  z = x*b.y - y*b.x;

  x = nX;
  y = nY;
}
Vector3& Vector3::operator *=(const float f) {
  x *= f;
  y *= f;
  z *= f;
}

Vector3& Vector3::operator *=(const Matrix& m) {
  float nX, nY, nZ, d;
  int width = m.width;
  for(int i=0; i<3; i++) {

    d = m.data[0*width+i] * x + m.data[1*width+i] * y +
        m.data[2*width+i] * z + m.data[3*width+i] * w;
    switch(i)
    {
      case 0: nX = d;break;
      case 1: nY = d;break;
      case 2: nZ = d;break;
    }
  }

  x = nX;
  y = nY;
  z = nZ;
}

Vector3& Vector3::operator *=(const Matrix* m) {
  operator *=(*m);
}

float Vector3::dot(const Vector3& b) {
  return x*b.x + y*b.y + z*b.z;
}
