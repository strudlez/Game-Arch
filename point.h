#pragma once

#include"point.h"

class Point : public Vector3 {
 public:
  void set(float nX, float nY, float nZ);
  void setlerp(const Point& p, float b);
  void setlerp(const Point& p, const Point& q, float b);
  Point lerp(const Point& p, float b);
  Point lerp(const Point& p, const Point& q, float b);
};
