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
