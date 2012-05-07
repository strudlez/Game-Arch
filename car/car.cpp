#include "car.h"


car::car(GLuint s1, GLuint s2) {
  ModelMatrix = glm::mat4(1.0f);
  ModelMatrix = glm::translate(ModelMatrix, glm::vec3(1000,1000,10));
  ModelPitch = 0;
  ModelYaw = 0;

  maxSpeed = 0.1;

  acc = 0.001;
  rollingC = 0.0005;
  weight = 1;

  speed = 0;
  g = -0.8;

  k = 0.005;

  first = true;

  onGround = true;
  vel = glm::vec3(0,0,0);
  egg* bodMod = new egg("body.egg");
  egg* wheelMod = new egg("wheel.egg");
  bodMod->load(s1, s2);
  wheelMod->load(s1, s2);

  bod = new body(bodMod);
  for(int i=0; i<4; i++) {
    wheels[i] = new wheel(wheelMod);
    wheels[i]->MoveMatrix = glm::translate(wheels[i]->MoveMatrix, glm::vec3(-0.4, -0.25, -0.4));
    susp[i] = 0;
  }
  float xMov = 2.0f;
  float yMov = 1.75f;
  zMov = -1;

  wheels[0]->LocalMatrix = glm::translate(wheels[0]->LocalMatrix, glm::vec3(xMov, yMov, zMov));
  wheels[1]->LocalMatrix = glm::translate(wheels[1]->LocalMatrix, glm::vec3(-xMov, yMov, zMov));
  wheels[2]->LocalMatrix = glm::translate(wheels[2]->LocalMatrix, glm::vec3(xMov, -yMov, zMov));
  wheels[3]->LocalMatrix = glm::translate(wheels[3]->LocalMatrix, glm::vec3(-xMov, -yMov, zMov));
}

void car::jump() {
  if(true) {
    vel.z=15;
  for(int i = 0; i < 4; i++) {
    //wheels[i]->speed=4;
  }
  }
}
void car::step(int move, int turn, terrain* world) {
    vel.z += g;
  for(int i = 0; i < 4; i++) {
    //wheels[i]->speed+=g;
  }
  checkCollide(world);
  //ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0,0,vel.z));

  for(int i=0; i<4; i+=2) {
    float turnAmt = 0;
    if(wheels[i]->rot != 0 && wheels[i]->rot*turn>=0) {
      if(abs(wheels[i]->rot) <= wheels[i]->turnSpeed) {
        turnAmt = -wheels[i]->rot;
      } else if(wheels[i]->rot > 0) {
        turnAmt = -wheels[i]->turnSpeed;
      } else {
        turnAmt = wheels[i]->turnSpeed;
      }
    }
    if(turn) {
      turnAmt += -wheels[i]->turnSpeed*turn;
      if(abs(wheels[i]->rot+turnAmt) > wheels[i]->maxTurn) {
        if(turnAmt>0) {
          turnAmt = wheels[i]->maxTurn - wheels[i]->rot;
        } else {
          turnAmt = -wheels[i]->maxTurn - wheels[i]->rot;
        }
      }
    }
    wheels[i]->rot+=turnAmt;

  }

  glm::vec4 loc = glm::vec4(vel.x,vel.y,vel.z,-1);
  loc = bod->TurnMatrix * loc;
  //speed = loc.x;
  if(onGround) {
    float friction = 0;
    float stepSpeed = 0;
    if(speed > 0) {
      friction = weight * rollingC;
    } else if (speed < 0) {
      friction = -weight * rollingC;
    }
    if(abs(friction)>abs(speed)) {
      stepSpeed = -speed;
    } else {
      stepSpeed = -friction;
    }


    if(move) {
      stepSpeed += acc*move;
      if(speed*stepSpeed > 0 && abs(speed+stepSpeed)>maxSpeed) {
        if(abs(speed)<=maxSpeed) {
          stepSpeed = 0;
        }
        else {
          stepSpeed = move*maxSpeed - speed;
        }
      }

    }
    speed += stepSpeed;
    if(stepSpeed != 0) {
      glm::vec4 m = glm::vec4(stepSpeed, 0, 0, 1);
      m = bod->TurnMatrix * m;
      vel.x+=m.x;
      vel.y+=m.y;
      vel.z+=m.z;
    }
    if(speed == 0) {
      vel.x = 0;
      vel.y = 0;
    }

    ModelMatrix = glm::rotate(ModelMatrix, speed/15*wheels[0]->rot, glm::vec3(0.0, 0.0, 1.0));

  } 


  bod->calc(&ModelMatrix, &TurnMatrix);

  for(int i=0; i<4; i++) {
    if(move || (onGround && speed!=0)) {
      if(onGround) {
        wheels[i]->spin+=speed*10;
      } else {
        wheels[i]->spin+=maxSpeed*10*move;
      }

      if (wheels[i]->spin < 0) {
        wheels[i]->spin+=360;
      } else if(wheels[i]->spin>=360) {
        wheels[i]->spin-=360;
      }
    }
    if(wheels[i]->spin || wheels[i]->rot) {
      wheels[i]->TurnMatrix = glm::mat4(1.0f);
      wheels[i]->TurnMatrix = glm::rotate(wheels[i]->TurnMatrix, wheels[i]->rot, glm::vec3(0, 0, 1));
      wheels[i]->TurnMatrix = glm::rotate(wheels[i]->TurnMatrix, wheels[i]->spin, glm::vec3(0, 1, 0));
    }
  }
  float max = glm::length(vel);
  glm::vec3 v = glm::normalize(vel);
  float j;
  onGround = false;
  for(j = 0; j < 20; j+=1) {
    for(int i =0; i<4; i++) {
      //float sp = wheels[i]->speed;
      //if(!sp)sp = g;
      //wheels[i]->LocalMatrix = glm::translate(wheels[i]->LocalMatrix, glm::vec3(0,0,sp/20));
    }
    ModelMatrix = glm::translate(ModelMatrix, vel/20.0f);
    calcWheels();
    onGround = onGround | checkCollide(world);
  }
  if(false) { //if(j!=max) {
    float last = max-j+0.5f;
    for(int i =0; i<4; i++) {
      float sp = wheels[i]->speed;
      wheels[i]->LocalMatrix = glm::translate(wheels[i]->LocalMatrix, glm::vec3(0,0,wheels[i]->speed)*last/max);
    }
    ModelMatrix = glm::translate(ModelMatrix, vel*last);
    calcWheels();
    onGround = onGround | checkCollide(world);
  }

}

void car::calcWheels() {
  for(int i = 0; i < 4; i++) {
    wheels[i]->calc(&ModelMatrix, &TurnMatrix);
  }
}

void car::Draw(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix, bool wire) {

  bod->Draw(ViewMatrix, ProjectionMatrix, wire);
  for(int i=0; i<4; i++) {
    //wheels[i]->ModelMatrix = glm::translate(wheels[i]->ModelMatrix, glm::vec3(0, 0, wheels[i]->radius));
    wheels[i]->Draw(ViewMatrix, ProjectionMatrix, wire);
    //wheels[i]->ModelMatrix = glm::translate(wheels[i]->ModelMatrix, glm::vec3(0, 0, -wheels[i]->radius));
  }
}

bool car::checkCollide(terrain* world) {
  float w[4];
  float d[4];
  bool lastGround = onGround;
  bool ground = false;

  for(int i =0; i<4;i++) {
    //wheels[i]->LocalMatrix = glm::translate(wheels[i]->LocalMatrix, glm::vec3(0, 0, g));
    wheels[i]->calc(&ModelMatrix, &TurnMatrix);
    if(wheels[i]->checkCollide(world)) {
      if(vel.z<0) {
        vel.z = 0;
      }
      if(wheels[i]->speed<0) {
        wheels[i]->speed=0;
      }
      ground = true;
      first = false;
    } else {
    }
    glm::vec4 loc = glm::vec4(0,0,0,1);
    loc = wheels[i]->LocalMatrix * loc;
    float diff = loc.z - zMov;
    d[i] = diff;
    susp[i]+= diff;
    if(first) {
      w[i] = susp[i];
    } else {
      w[i] = k * susp[i] + weight*g;
    }
  }
  if(lastGround) {

  }

  float zAdj = (w[0]+w[1]+w[2]+w[3])/4.0f;
  float dAdj = (d[0]+d[1]+d[2]+d[3])/4.0f;

  float frontZ = (w[0]+w[2])/2.0f;
  float backZ = (w[1]+w[3])/2.0f;

  float leftZ = (w[0]+w[1])/2.0f;
  float rightZ = (w[2]+w[3])/2.0f;


  TurnMatrix = glm::mat4(1);
  TurnMatrix2 = glm::mat4(1);
  ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0, 0, zAdj));

  float xTurn = glm::degrees(glm::atan((backZ-frontZ), bod->length));
  float yTurn = glm::degrees(glm::atan((leftZ-rightZ), bod->width*2));
  TurnMatrix = glm::rotate(bod->TurnMatrix, xTurn, glm::vec3(0, 1, 0));
  TurnMatrix = glm::rotate(bod->TurnMatrix, yTurn, glm::vec3(1, 0, 0));
  for(int i=0; i<4;i++) {
    float length = bod->length/2;
    float width = bod->width/2;
    susp[i]-= w[i];
    //    if(d[i])
    //      wheels[i]->LocalMatrix = glm::translate(wheels[i]->LocalMatrix, glm::vec3(0, 0, -dAdj));

    //wheels[i]->LocalMatrix = glm::translate(wheels[i]->LocalMatrix, glm::vec3(0, 0, -w[i]));
    wheels[i]->LocalMatrix = glm::translate(wheels[i]->LocalMatrix, glm::vec3(0, 0, -zAdj));

    //float xTurn = glm::degrees(glm::atan(w[i], length));
    //if(i%2==0) xTurn*=-1;

    //printf("A[%d]: %f\n", i, xTurn);



  }
  calcWheels();
  bod->calc(&ModelMatrix, &TurnMatrix);
  return ground;

}
