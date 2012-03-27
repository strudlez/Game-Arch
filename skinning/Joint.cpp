#include <vector>
#include <cstddef>
#include <iostream>
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

  //cout<<"Max frame: "<<maxFrame<<endl;
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

  if (xList.size() > 0)
    xNew = frame/fps*xList.size();
  if (yList.size() > 0)
    yNew = frame/fps*yList.size();
  if (zList.size() > 0)
    zNew = frame/fps*zList.size();

  if (hList.size() > 0)
    hNew = frame/fps*hList.size();
  if (pList.size() > 0)
    pNew = frame/fps*pList.size();
  if (rList.size() > 0)
    rNew = frame/fps*rList.size();

  //Animation needs to be updated
  if (1) {
    //(xN != xNew || yN != yNew || zN != zNew || hN != hNew || pN != pNew ||
      //rN != rNew) {
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
    localMat.translateXYZ(x,y,z);

    if(hN != -1) {
      math.rotateH(hList[hN]);
      localMat = math*localMat;
    }
    if(pN != -1) {
      math.rotateH(pList[pN]);
      localMat = math*localMat;
    }
    if(rN != -1) {
      math.rotateH(rList[rN]);
      localMat = math*localMat;
    }
  }

  if (parent != NULL) {
    //cout<<num<<" "<<parent->num<<" "<<parent->fps<<endl;
    worldMat = parent->worldMat*localMat;
  } else {
    //worldMat.reset();
    worldMat = localMat;
  }
  for (int i = 0; i < children.size(); i++) {
    children[i]->runJoint(math, dt);
  }
}
