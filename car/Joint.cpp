#include <vector>
#include <cstddef>
#include <iostream>
#include <math.h>
#include "Joint.h"

using namespace std;

Joint::Joint() {
  frame = 0;
  parent = NULL;
  xN = -1;
  yN = -1;
  zN = -1;
  hN = -1;
  pN = -1;
  rN = -1;
}

void Joint::calculateFrames() {
  maxFrame = max(xList.size(), yList.size());
  maxFrame = max(maxFrame, zList.size());
  maxFrame = max(maxFrame, hList.size());
  maxFrame = max(maxFrame, pList.size());
  maxFrame = max(maxFrame, rList.size());
}

void Joint::addChild(Joint* j) {
  j->parent = this;
  children.push_back(j);
  
}

void Joint::runJoint(Matrix& math, float dt) {
  float x = 0, y = 0, z = 0, h = 0, p = 0, r = 0;
  float xNew = -1, yNew = -1, zNew = -1, hNew = -1, pNew = -1, rNew = -1;
  frame += dt * (60.0f/fps);
  if (frame >= maxFrame) {
    frame = 0;
  }

  float f = floor(frame);
  
  //cout<<"Joint "<<num<<": "<<f<<" Max: "<<maxFrame<<endl;

  if (xList.size() > 0)
    xNew = f/fps*xList.size();
  if (yList.size() > 0)
    yNew = f/fps*yList.size();
  if (zList.size() > 0)
    zNew = f/fps*zList.size();

  if (hList.size() > 0)
    hNew = f/fps*hList.size();
  if (pList.size() > 0)
    pNew = f/fps*pList.size();
  if (rList.size() > 0)
    rNew = f/fps*rList.size();

  //Animation needs to be updated
  if (parent != NULL &&
      (xN != xNew || yN != yNew || zN != zNew || hN != hNew || pN != pNew ||
      rN != rNew)) {
    localMat.reset();
    xN = xNew;
    yN = yNew;
    zN = zNew;
    hN = hNew;
    pN = pNew;
    rN = rNew;

    if(xN != -1) x = xList[xN];
    if(yN != -1) y = yList[xN];
    if(zN != -1) z = zList[xN];
    if(hN != -1) h = hList[hN];
    if(pN != -1) p = pList[pN];
    if(rN != -1) r = rList[rN];
    
    localMat.data[0] = 1;
    localMat.data[5] = 1;
    localMat.data[10] = 1;
    localMat.data[15] = 1;

    math.rotateH(h);
    localMat = math*localMat;

    math.rotateH(p);
    localMat = math*localMat;

    math.rotateH(r);
    localMat = math*localMat;
    
    math.translateXYZ(x,y,z);
    localMat = math * localMat;
  }

  if (parent != NULL) {
    worldMat = localMat * parent->worldMat;
  } else {
    worldMat.reset();
    worldMat.data[0] = 1;
    worldMat.data[5] = 1;
    worldMat.data[10] = 1;
    worldMat.data[15] = 1;
  }
  for (int i = 0; i < children.size(); i++) {
    children[i]->runJoint(math, dt);
  }
}
