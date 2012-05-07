#pragma once


#include "phys.h"

class body: public phys {
 public:
  body(egg* b);
  float length;
  float width;
  float height;

};
