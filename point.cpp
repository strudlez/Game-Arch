#include<stdlib.h>
#include"vector3.h"

#include"point.h"

void Point::set(float nX, float nY, float nZ) {
  x = nX;
  y = nY;
  z = nZ;
  w = 1;
}
