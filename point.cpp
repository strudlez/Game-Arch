#include<stdlib.h>
#include"vector3.h"

#include"point.h"

void Point::set(float nX, float nY, float nZ) {
  x = nX;
  y = nY;
  z = nZ;
  w = 1;
}

void Point::setlerp(const Point& p, float b) {
  setlerp(*this, p, b);
}

void Point::setlerp(const Point & p, const Point& q, float b) {
  float nX, nY, nZ;
  
  float nB = 1 - b;

  nX = (nB * p.x) + (b * q.x);
  nY = (nB * p.y) + (b * q.y);
  nZ = (nB * p.z) + (b * q.z);

  x = nX;
  y = nY;
  z = nZ;
}

Point Point::lerp(const Point& p, float b) {
  return lerp(*this, p, b);
}
Point Point::lerp(const Point& p, const Point& q, float b) {
  Point ret;
  float nB = 1 - b;
  ret.x = (nB * p.x) + (b * q.x);
  ret.y = (nB * p.y) + (b * q.y);
  ret.z = (nB * p.z) + (b * q.z);

  return ret;
}
