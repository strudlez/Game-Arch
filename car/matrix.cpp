#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "matrix.h"

using namespace std;

Matrix::Matrix() {
  width = 4;
  height = 4;
  data[15]=1;
  
}

void Matrix::reset() {
  for (int i=0; i<width; i++) {
    for (int j=0; j<height; j++) {
      data[i*height+j]=0;
    }
    data[15] = 1;
  }
}

Matrix& Matrix::operator =(const Matrix& m) {
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      data[i*height+j] = m.data[i*height+j];
    }
  }
}

Matrix Matrix::operator *(const Matrix& m) const {
  Matrix ret;
  float d;

  for(int i = 0; i < width; i++) {
    for(int j = 0; j < height; j++) {
      ret.data[i*height+j] = 
          data[0*height+j] * m.data[i*height] +
          data[1*height+j] * m.data[i*height+1] +
          data[2*height+j] * m.data[i*height+2] +
          data[3*height+j] * m.data[i*height+3];
    }
  }

  return ret;
}

Matrix Matrix::operator *(const Matrix* m) const {
  operator *(*m);
}

Matrix& Matrix::operator +=(const Matrix& m) {
  for(int i = 0; i < width; i++) {
    for(int j = 0; j < height; j++) {
      data[i*height+j] += m.data[i*height+j];
    }
  }
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

  data[0] = 1;
  data[1] = 0;
  data[2] = 0;
  data[3] = 0;
  
  data[height] = 0;
  data[height+1] = ch;
  data[height+2] = sh;
  data[height+3] = 0;

  data[2*height] = 0;
  data[2*height+1] = -sh;
  data[2*height+2] = ch;
  data[2*height+3] = 0;

  data[3*height] = 0;
  data[3*height+1] = 0;
  data[3*height+2] = 0;
  data[3*height+3] = 1;



}
void Matrix::rotateP(float p) {
  float sp = sin(p);
  float cp = cos(p);

  data[0] = cp;
  data[1] = 0;
  data[2] = -sp;
  data[3] = 0;

  data[1*height] = 0;
  data[1*height+1] = 1;
  data[1*height+2] = 0;
  data[height+3] = 0;


  data[2*height+0] = sp;
  data[2*height+1] = 0;
  data[2*height+2] = cp;
  data[2*height+3] = 0;
  
  data[3*height+0] = 0;
  data[3*height+1] = 0;
  data[3*height+2] = 0;
  data[3*height+3] = 1;
}

void Matrix::rotateR(float r) {
  Matrix::reset();
  float sr = sin(r);
  float cr = cos(r);

  data[0] = cr;
  data[1] = sr;

  data[height] = -sr;
  data[height+1] = cr;

  data[2*height+2] = 1;
}
void Matrix::scale(float s) {
  Matrix::reset();
  data[0*height+0] = s;
  data[1*height+1] = s;
  data[2*height+2] = s;

  data[3*height] = 0;
  data[3*height+1] = 0;
  data[3*height+2] = 0;
}

void Matrix::translateXYZ(float x, float y, float z) {
  Matrix::reset();
  data[0*height+0] = 1;
  data[1*height+1] = 1;
  data[2*height+2] = 1;

  data[3*height+0] = x;
  data[3*height+1] = y;
  data[3*height+2] = z;
  data[3*height+3] = 1;
}
void Matrix::translateXYZ(const Vector3& v) {
  Matrix::translateXYZ(v.x, v.y, v.z);
}

void Matrix::print() {
  for(int i = 0; i<height; i++) {
    cout<<"[";
    for(int j = 0; j<width; j++) {
      cout<<data[j*height+i];
      if(j<width-1) cout<<"\t";
    }
    cout<<"]\n";
  }

}

Quaternion Matrix::toQuat() const {
  Quaternion quat;

  float q[4];
  int nxt[3];

  nxt[0] = 1;
  nxt[1] = 2;

  float trace = data[0*height+0] + data[1*height+1] + data[2*height+2];

  if (trace>0) {
    float s = sqrt(trace + 1);
    quat.w = s / 2;
    s = 0.5f / s;

    quat.x = (data[2*height+1] - data[1*height+2]) * s;
    quat.y = (data[0*height+2] - data[2*height+0]) * s;
    quat.z = (data[1*height+0] - data[0*height+1]) * s;

    return quat;
  } else {
    int i = 0;

    if (data[1*height+1] > data[0*height+0]) i = 1;
    if (data[2*height+2] > data[i*height+i]) i = 2;

    int j = nxt[i];
    int k = nxt[j];

    float s = sqrt((data[i*height+i] - (data[j*height+j] + data[k*height+k])) + 1);
    q[i] = s * 0.5f;
    if( s != 0) s = 0.5f / s;

    quat.w = (data[j*height+k] - data[k*height+j]) * s;
    q[j] = (data[i*height+j] + data[j*height+i]) * s;
    q[k] = (data[i*height+k] + data[k*height+i]) * s;

    quat.x = q[0];
    quat.y = q[1];
    quat.z = q[2];

    return quat;
  }
}

Matrix Matrix::slerp(const Matrix& p, const Matrix& q, float b) {
  return p.toQuat().slerp(q.toQuat(), b);
}

Matrix Matrix::slerp(const Quaternion& q, float b) {
  return this->toQuat().slerp(q, b);
}

Matrix Matrix::slerp(const Matrix& q, float b) {
  return this->toQuat().slerp(q.toQuat(), b);
}
