//Zachary Strudler
//Game Architechture HW2

#include "cube.h"
#include <string.h>

#include <iostream>
#include <stdlib.h>

#define DEBUG 0

using namespace std;

int main()
{
  Cube c = Cube();
  char* s = (char*)malloc(256*sizeof(char));
  
  char* fun = (char*)malloc(256*sizeof(char));
  float* params = (float*)malloc(10*sizeof(float));
  int len=0;
  
  char* word;

  c.reset();

  do {
    cout<<"$ ";
    cin.getline(s, 256, '\n');

    if (!strcmp(s,"quit"))break;

    if (strcmp(s,"r"))
    {
      word = strtok (s," ");
      
      if (word == NULL) continue;
      
      len = 0;

      if (strcmp(word,"l"))
      {
        strcpy(fun, word);
      }


      while (len<9 && word != NULL)
      {
        word = strtok (NULL, " ");
        if (word!=NULL)
        {
          params[len++] = atof(word);
        }
      }
    }

    
    
    if (!strcmp(fun, "reset")) {
      c.reset();
    } else if (!strcmp(fun, "translateX") && len>=1) {
      c.translateX(params[0]);
    } else if (!strcmp(fun, "translateY") && len>=1) {
      c.translateY(params[0]);
    } else if (!strcmp(fun, "translateZ") && len>=1) {
      c.translateZ(params[0]);
    } else if (!strcmp(fun, "rotateH") && len>=1) {
      c.rotateH(params[0]);
    } else if (!strcmp(fun, "rotateP") && len>=1) {
      c.rotateP(params[0]);
    } else if (!strcmp(fun, "rotateR") && len>=1) {
      c.rotateR(params[0]);
    }
    else if (!strcmp(fun, "scale") && len>=1) {
      c.scale(params[0]);
    } else if (!strcmp(fun, "rotateHPR") && len>=3) {
      c.rotateHPR(params[0], params[1], params[2]);
    } else if (!strcmp(fun, "translateXYZ") && len>=3) {
      c.translateXYZ(params[0], params[1], params[2]);
    } else if (!strcmp(fun, "transformXYZHPRS") && len>=7) {
      c.transformXYZHPRS(params[0], params[1], params[2],
          params[3], params[4], params[5], params[6]);
    } else if (!strcmp(fun, "toQuat")) {
      c.mat->toQuat().print();
      continue;
    } else if (!strcmp(fun, "slerp_mat") && len>=10) {
      Matrix m;
      m.data[0][0] = params[0];
      m.data[1][0] = params[1];
      m.data[2][0] = params[2];
      m.data[0][1] = params[3];
      m.data[1][1] = params[4];
      m.data[2][1] = params[5];
      m.data[0][2] = params[6];
      m.data[1][2] = params[7];
      m.data[2][2] = params[8];
      m.slerp(*(c.mat), params[9]).print();
      continue;
    } else if (!strcmp(fun, "slerp_quat") && len>=5) {
      Quaternion q;
      q.x = params[0];
      q.y = params[1];
      q.z = params[2];
      q.w = params[3];

      q.slerp(*(c.mat), params[4]).print();
      continue;
    } else if (strcmp(fun, "display")) {
      cout << "Commands:\n" <<
          "\treset\n" <<
          "\tdisplay\n" <<
          "\ttranslateX [x]\n" <<
          "\ttranslateY [y]\n" <<
          "\ttranslateZ [z]\n" <<
          "\trotateH [h]\n" <<
          "\trotateP [p]\n" <<
          "\trotateR [r]\n" <<
          "\tscale [s]\n" <<
          "\trotateHPR [h] [p] [r]\n" <<
          "\ttranslateXYZ [x] [y] [z]\n" <<
          "\ttransformXYZHPRS [x] [y] [z] [h] [p] [r] [s]\n" <<
          "\ttoQuat\n" <<
          "\tslerp_mat [0,0] [1,0] [2,0] [0,1] [1,1] [2,1] " <<
              "[0,2] [1,2] [2,2] B\n" <<
          "\tslerp_quat [q1] [q2] [q3] [q0] B\n";
      continue;
    }

    cout<<"\n";
    c.print();
    c.printMat();
    cout<<"\n";

  }while(1);

  return 0;
}
