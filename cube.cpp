#include<stdlib.h>
#include"cube.h"
#include"matrix.h"
#include"point.h"
#include<iostream>

Cube::Cube() {
  mat = new Matrix();
  data = (Point*) malloc(8*sizeof(Vector3));

  for(int i=0; i<8; i++) data[i] = Point();
}

void Cube::apply(Matrix* m) {
  for(int i=0; i<8; i++) data[i] *= m;
}

void Cube::reset() {
  data[0].set(1,1,1);
  data[1].set(1,1,-1);
  data[2].set(1,-1,1);
  data[3].set(1,-1,-1);
  data[4].set(-1,1,1);
  data[5].set(-1,1,-1);
  data[6].set(-1,-1,1);
  data[7].set(-1,-1,-1);
}

void Cube::translateX(float x) {
  mat->translateX(x);

  Cube::apply(mat);
}
void Cube::translateY(float y) {
  mat->translateY(y);

  Cube::apply(mat);
}
void Cube::translateZ(float z) {
  mat->translateZ(z);

  Cube::apply(mat);
}
void Cube::rotateH(float h) {
  mat->rotateH(h);

  Cube::apply(mat);
}
void Cube::rotateP(float p) {
  mat->rotateP(p);

  Cube::apply(mat);
}
void Cube::rotateR(float r) {
  mat->rotateR(r);

  Cube::apply(mat);
}
void Cube::scale(float s) {
  mat->scale(s);

  Cube::apply(mat);
}
void Cube::translateXYZ(float x, float y, float z) {
  mat->translateXYZ(x, y, z);

  Cube::apply(mat);
}
void Cube::translateXYZ(Vector3 v) {
  Cube::translateXYZ(v.x, v.y, v.z);
}
void Cube::rotateHPR(float h, float p, float r) {
  Cube::rotateH(h);
  Cube::rotateH(p);
  Cube::rotateH(r);
  //mat->rotateHPR(h, p, r);
  //Cube::apply(mat);
}
void Cube::transformXYZHPRS(float x, float y, float z,
  float h, float p, float r, float s) {

  Cube::translateXYZ(x,y,z);
  Cube::rotateHPR(h,p,r);
  Cube::scale(s);

  //mat->transformXYZHPRS(x,y,z,h,p,r,s);

  //Cube::apply(mat);
}

void Cube::print() {
  for(int i=0; i<8; i++) data[i].print();
}
void Cube::printMat() {
  mat->print();
}
