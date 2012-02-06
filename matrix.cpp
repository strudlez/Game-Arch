#include<stdlib.h>
#include<math.h>
#include"matrix.h"
#include<iostream>

using namespace std;

Matrix::Matrix() {
  width = 4;
  height = 4;
  data[3][3]=1;
  
}

void Matrix::reset() {
  for(int i=0; i<width-1; i++) {
    for(int j=0; j<height-1; j++) {
      data[i][j]=0;
    }
  }
}
void Matrix::apply(Vector3* v) {
  float x,y,z,d;
  for(int i=0; i<3; i++) {

    d = data[0][i]*v->x + data[1][i]*v->y + data[2][i]*v->z + data[3][i]*v->w;
    switch(i)
    {
      case 0: x = d;break;
      case 1: y = d;break;
      case 2: z = d;break;
    }
  }

  v->x = x;
  v->y = y;
  v->z = z;
}

void Matrix::translateX(float x) {
  Matrix::translateXYZ(x,0,0);
}
void Matrix::translateY(float y) {
  Matrix::translateXYZ(0,y,0);
}
void Matrix::translateZ(float z) {
  Matrix::translateXYZ(0,0,z);
}
void Matrix::rotateH(float h) {
  float sh = sin(h);
  float ch = cos(h);

  data[0][0] = 1;
  data[1][0] = 0;
  data[2][0] = 0;
  
  data[0][1] = 0;
  data[1][1] = ch;
  data[2][1] = -sh;

  data[0][2] = 0;
  data[1][2] = sh;
  data[2][2] = ch;

  data[3][0] = 0;
  data[3][0] = 0;
  data[3][0] = 0;



}
void Matrix::rotateP(float p) {
  float sp = sin(p);
  float cp = cos(p);

  data[0][0] = cp;
  data[1][1] = 0;
  data[2][0] = sp;

  data[0][1] = 0;
  data[1][1] = 1;
  data[2][1] = 0;


  data[0][2] = 0;
  data[1][2] = -sp;
  data[2][2] = cp;
  
  data[3][0] = 0;
  data[3][1] = 0;
  data[3][2] = 0;
}

void Matrix::rotateR(float r) {
  Matrix::reset();
  float sr = sin(r);
  float cr = cos(r);

  data[0][0] = cr;
  data[1][0] = -sr;

  data[0][1] = sr;
  data[1][1] = cr;

  data[2][2] = 1;
}
void Matrix::scale(float s) {
  Matrix::reset();
  data[0][0] = s;
  data[1][1] = s;
  data[2][2] = s;
}

void Matrix::translateXYZ(float x, float y, float z) {
  Matrix::reset();
  data[0][0] = 1;
  data[1][1] = 1;
  data[2][2] = 1;

  data[3][0] = x;
  data[3][1] = y;
  data[3][2] = z;
}
void Matrix::translateXYZ(Vector3 v) {
  Matrix::translateXYZ(v.x, v.y, v.z);
}

void Matrix::print() {
  for(int i = 0; i<height; i++) {
    cout<<"[";
    for(int j = 0; j<width; j++) {
      cout<<data[j][i];
      if(j<width-1) cout<<"\t";
    }
    cout<<"]\n";
  }
}
