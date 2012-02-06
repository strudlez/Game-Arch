//Zachary Strudler
//Game Architechture HW2

#include"cube.h"
#include<string.h>

#include<iostream>
#include<stdlib.h>

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

    if(!strcmp(s,"quit"))break;

    if(strcmp(s,"r"))
    {
      word = strtok (s," ");
      
      if(word == NULL) continue;
      
      len = 0;

      if(strcmp(word,"l"))
      {
        strcpy(fun, word);
      }


      while (len<9 && word != NULL)
      {
        word = strtok (NULL, " ");
        if(word!=NULL)
        {
          params[len++] = atof(word);
        }
      }
    }

    if(!strcmp(fun, "reset")) {
      c.reset();
    }
    else if(!strcmp(fun, "translateX") && len>=1) {
      c.translateX(params[0]);
    }
    else if(!strcmp(fun, "translateY") && len>=1) {
      c.translateY(params[0]);
    }
    else if(!strcmp(fun, "translateZ") && len>=1) {
      c.translateZ(params[0]);
    }
    else if(!strcmp(fun, "rotateH") && len>=1) {
      c.rotateH(params[0]);
    }
    else if(!strcmp(fun, "rotateP") && len>=1) {
      c.rotateP(params[0]);
    }
    else if(!strcmp(fun, "rotateR") && len>=1) {
      c.rotateR(params[0]);
    }
    else if(!strcmp(fun, "scale") && len>=1) {
      c.scale(params[0]);
    }
    else if(!strcmp(fun, "rotateHPR") && len>=3) {
      c.rotateHPR(params[0], params[1], params[2]);
    }
    else if(!strcmp(fun, "translateXYZ") && len>=3) {
      c.translateXYZ(params[0], params[1], params[2]);
    }
    else if(!strcmp(fun, "transformXYZHPRS") && len>=7) {
      c.transformXYZHPRS(params[0], params[1], params[2],
          params[3], params[4], params[5], params[6]);
    }

    cout<<"\n\n";
    c.print();
    c.printMat();
    cout<<"\n";

  }while(1);

  return 0;
}
