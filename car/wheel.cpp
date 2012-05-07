#include "wheel.h"

wheel::wheel(egg* w) {
  radius = 1;
  model = w;
  weight = 0.1;
  oldHeight = 0;
  newHeight = 0;
  speed = 0;
  turnSpeed = 1;
  maxTurn = 60;
  rot = 0;
  spin = 0;
  ModelMatrix = glm::mat4(1.0f);
  LocalMatrix = glm::mat4(1.0f);
  TurnMatrix = glm::mat4(1.0f);
  MoveMatrix = glm::mat4(1.0f);
  Tex = model->TexId;

}

void wheel::calc(glm::mat4* parent, glm::mat4* parent2) {
  oldHeight = getLoc().z;
  if(parent == NULL) {
    ModelMatrix = LocalMatrix * TurnMatrix * MoveMatrix;
  } else {
    ModelMatrix = (*parent) * (*parent2) * LocalMatrix * TurnMatrix * MoveMatrix;
  }
  newHeight = getLoc().z;
}


glm::vec4 wheel::getLoc() {
  glm::vec4 loc = glm::vec4(0,0,0,1);
  loc = ModelMatrix * loc;

  return loc;
}

bool wheel::checkCollide(terrain* world, tritest::Sphere& s, Vertex* a, Vertex* b, Vertex* c) {
  glm::vec3 va = glm::vec3(a->Position[0],a->Position[1],a->Position[2]);
  glm::vec3 vb = glm::vec3(b->Position[0],b->Position[1],b->Position[2]);
  glm::vec3 vc = glm::vec3(c->Position[0],c->Position[1],c->Position[2]);
  glm::vec3 collide;
  if(tritest::TestSphereTriangle(s, va, vb, vc, collide)) {
    float move = glm::length(s.c - collide);
    //float move = s.c.z - collide.z;
    LocalMatrix = glm::translate(LocalMatrix, glm::vec3(0,0, -move+radius));
    return true;
  }
  else {
    return false;
  }
}

bool wheel::checkCollide(terrain* world) {
  tritest::Sphere s;
  glm::vec4 loc = getLoc();
  s.c = glm::vec3(loc.x, loc.y, loc.z);
  s.r = radius;
  int gridX, gridY;
  world->getGridPos(loc.x, loc.y, gridX, gridY);
  for(int x = gridX-1; x<=gridX+1; x++) {
    if(x<0)continue;
    if(x>=world->width-1)break;
    for(int y = gridY-1; y<=gridY+1; y++) {
      if(y<0)continue;
      if(y>=world->height-1)break;
      Vertex* a = &world->model->vertices[x+y*world->width];
      Vertex* b = &world->model->vertices[x+(y+1)*world->width];
      Vertex* c = &world->model->vertices[x+1+y*world->width];
      Vertex* d = &world->model->vertices[x+1+(y+1)*world->width];
/*      glm::vec3 a = glm::vec3(x*world->gridSize, y*world->gridSize, world->getHeight(x,y));
      glm::vec3 b = glm::vec3(x*world->gridSize, (y+1)*world->gridSize, world->getHeight(x,y+1));
      glm::vec3 c = glm::vec3((x+1)*world->gridSize, y*world->gridSize, world->getHeight(x+1,y));
      glm::vec3 d = glm::vec3((x+1)*world->gridSize, (y+1)*world->gridSize, world->getHeight(x+1,y+1));*/

      if(checkCollide(world, s, a, b, d)) {
        return true;
      }
      if(checkCollide(world, s, a, c, d)) {
        return true;
      }
    }
  }
  return false;
}
