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
  for (int i=0; i<width-1; i++) {
    for (int j=0; j<height-1; j++) {
      data[i][j]=0;
    }
  }
}

Matrix& Matrix::operator =(const Matrix& m) {
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      data[i][j] = m.data[i][j];
    }
  }
}

Matrix Matrix::operator *(const Matrix& m) const {
  Matrix ret;
  float d;

  for(int i = 0; i < width; i++) {
    for(int j = 0; j < height; j++) {
      ret.data[i][j] = data[0][i] * m.data[j][0] +
          data[1][i] * m.data[j][1] +
          data[2][i] * m.data[j][2] +
          data[3][i] * m.data[j][3];
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
      data[i][j] += m.data[i][j];
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
void Matrix::translateXYZ(const Vector3& v) {
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

Quaternion Matrix::toQuat() {
  Quaternion quat;

  float q[4];
  int nxt[3];

  nxt[0] = 1;
  nxt[1] = 2;

  float trace = data[0][0] + data[1][1] + data[2][2];

  if (trace>0) {
    float s = sqrt(trace + 1);
    quat.w = s / 2;
    s = 0.5f / s;

    quat.x = (data[2][1] - data[1][2]) * s;
    quat.y = (data[0][2] - data[2][0]) * s;
    quat.z = (data[1][0] - data[0][1]) * s;

    return quat;
  } else {
    int i = 0;

    if (data[1][1] > data[0][0]) i = 1;
    if (data[2][2] > data[i][i]) i = 2;

    int j = nxt[i];
    int k = nxt[j];

    float s = sqrt((data[i][i] - (data[j][j] + data[k][k])) + 1);
    q[i] = s * 0.5f;
    if( s != 0) s = 0.5f / s;

    quat.w = (data[j][k] - data[k][j]) * s;
    q[j] = (data[i][j] + data[j][i]) * s;
    q[k] = (data[i][k] + data[k][i]) * s;

    quat.x = q[0];
    quat.y = q[1];
    quat.z = q[2];

    return quat;
  }
}
