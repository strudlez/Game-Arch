#include "egg.h"
#include "Vertex.h"
#include "Joint.h"

#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>

#define PI 3.14159265358979323846

using namespace std;

egg::egg(const char* filename) {
  readFile(filename);
}

void egg::readFile(const char* filename) {
  ifstream f(filename);

  if (f) {
    while (!f.eof()) {
      read(f);
    }
  }
}

void egg::load() {

}

void egg::r(ifstream& f, string& r) {
  f >> r;

  while (!f.eof() && r.size()>=2 && r[0]=='/' && r[1]=='/') {
    f.ignore(1000, '\n');
    f >> r;
  }
}

void egg::read(ifstream& f) {
  string word;
  int level = 0;

  while (!f.eof() && level >= 0) {
    r(f, word);

    if (word == "{") {
      level++;
    } else if (word == "}") {
      level--;
      if (!level) level--;
    } else if (word == "<Texture>") {
      readTexture(f);
    } else if (word == "<Vertex>") {
      readVertex(f);
    } else if (word == "<Polygon>") {
      readPolygon(f);
    } else if (word == "<Joint>") {
      readJoint(f, NULL);
    } else if (word == "<Table>") {
      readTable(f);
    } else if (word[0] == '<') {
      read(f);
    }
  }
}



void egg::readTexture(ifstream& f) {
  string texName;
  string texFile;

  string word;

  r(f, texName);
  r(f, word);
  r(f, texFile);

  map<string, string> dat;

  while (word != "}") {
    r(f, word);
    
    if (word == "<Scalar>") {
      readScalar(f, dat);
    } else if (word[0] == '<') {
      read(f);
    }

  }

  //Strip quotes
  texFile = texFile.substr(1,texFile.size()-2);

  texture = texFile;
}

void egg::readScalar(ifstream& f, map<string,string>& dat) {
  string word;

  do {
    r(f, word);
  } while (word != "}");
}

void egg::readVertex(ifstream& f) {
  int level = 0;
  
  GLuint num;
  float x, y, z, uvA, uvB;
  string word;

  f >> num;
  
  //bracket
  r(f, word);
  
  f >> x;
  f >> y;
  f >> z;

  do {
    r(f, word);
    if (word == "<UV>") {
      readUV(f, uvA, uvB);
    } else if (word[0] == '<') read(f);
  } while(word != "}");

  Vertex v;
  float scale = 1;
  v.Position[0] = x*scale;
  v.Position[1] = y*scale;
  v.Position[2] = z*scale;
  v.UV[0] = uvA;
  v.UV[1] = uvB;
  v.jointNum = 0;
  for(int i = 0; i< 4; i++) {
    v.jointInfluence[i] = 0;
  }

  vertices.push_back(v);
}

void egg::readUV(ifstream& f, float& a, float &b) {
  string word;

  r(f, word);

  f >> a;
  f >> b;

  do {
    r(f, word);
  } while(word != "}");
}
void egg::readPolygon(ifstream& f) {
  string word;

  do {
    r(f, word);
    if (word == "<VertexRef>") {
      readIndex(f);
    } else if (word[0] == '<') read(f);
  } while(word != "}");

}

void egg::readIndex(ifstream& f) {
  string word;
  GLuint a, b, c;

  r(f, word);

  f >> a;
  f >> b;
  f >> c;
  
  do {
    r(f, word);

    if (word[0] == '<') read(f);
  } while(word != "}");

  indices.push_back(a-1);
  indices.push_back(b-1);
  indices.push_back(c-1);

}

void egg::readJoint(ifstream& f, Joint* parent) {
  Joint* op = NULL;
  string word;

  if (parent == NULL) parent = &jointTree;
  op = new Joint();
  op->num = jointList.size();
  parent->addChild(op);
  jointList.push_back(op);
  r(f,word);
  map<const string, Joint*>::iterator lb = jointMap.lower_bound(word);
  if (lb != jointMap.end() && !(jointMap.key_comp()(word, lb->first))) {
  } else {
    jointMap.insert(lb, map<const string, Joint*>::value_type(word, op));
  }

  do {
    r(f, word);
    if (word == "<Joint>") {
      readJoint(f, op);
    } else if (word == "<Transform>") {
      readJointTransform(f, op);
    } else if (word == "<VertexRef>") {
      readJointVertex(f, op);
    } else if (word[0] == '<') {
      read(f);
    }
  } while(word != "}");
}

void egg::readJointTransform(ifstream& f, Joint* j) {
  string word;
  do {
    r(f, word);
    if (word == "<Matrix4>") {
      r(f, word);
      int height = j->worldMat.height;
      for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
          f >> j->worldMat.data[x*height + y];
        }
      }
      r(f, word);
      word = "";
    } else if (word[0] == '<') {
      read(f);
    }

  } while (word != "}");

}

void egg::readJointVertex(ifstream& f, Joint* j) {
  string word;
  vector<int> refs;
  float influence;

  do {
    r(f, word);
    if (word!="{" && word[0] != '<') {
      //cout<<word<<endl;
      int ref = atoi(word.c_str());
      refs.push_back(ref);
    }
  } while (word[0] != '<');
  do {
    if (word == "<Scalar>") {
      r(f, word);
      r(f, word);
      f >> influence;
      r(f, word);
    } else if(word[0] == '<') {
      read(f);
    }
    r(f, word);
  } while (word != "}");

  for (int i = 0; i < refs.size(); i++) {
    int ref = refs[i];
    //int jNum = vertices[ref].jointNum;
    Vertex* v = &vertices[ref];
    int jNum = v->jointNum;
    v->joints[jNum] = j->num;
    v->jointInfluence[jNum] = influence;
    v->jointNum = jNum+1;
    /*cout<<"Vertex: "<<ref<<" Num: "<<jNum<<" Joint: "<<v->joints[jNum]
      <<" Inf: "<<v->jointInfluence[jNum]<<endl;*/
  }
}

void egg::readTable(ifstream& f) {
  Joint* j;
  string word;
  string name;
  r(f, word);
  if (word == "{" || word == "") {
    j = NULL;
  } else {
    name = word;
    map<const string, Joint*>::iterator it = jointMap.find(word);
    if (it == jointMap.end()) {
      j = NULL;
    } else {
      j = (*it).second;
    }
  }
  do {
    r(f, word);
    if (word == "<Table>") {
      readTable(f);
    }else if (j != NULL) {
      if (word == "<Xfm$Anim_S$>") {
        readAnim(f, j);
      } else if (word[0] == '<') {
        read(f);
      }
    } else if (word[0] == '<') {
      read(f);
    }
  } while (word != "}" && !f.eof());
  if (j != NULL) {
    j->calculateFrames();
  }
}

void egg::readAnim(ifstream& f, Joint* j) {
  string word;
  
  do {
    r(f, word);
    if (word == "<S$Anim>") {
      readAnimList(f, j);
    } else if (word == "<Scalar>") {
      r(f, word);
      if (word == "fps") {
        r(f, word);
        f >> j->fps;
        r(f, word);
        word = "";
      } else {
        read(f);
      }
    } else if (word[0] == '<') {
      read(f);
    }
  } while (word != "}");

}

void egg::readAnimList(ifstream&f, Joint* j) {
  string word;
  vector<float>* nums = NULL;
  r(f, word);
  bool rad = false;
  if (word == "x") {
    nums = &(j->xList);
  } else if(word == "y") {
    nums = &(j->yList);
  } else if(word == "z") {
    nums = &(j->zList);
  } else if(word == "h") {
    nums = &(j->hList);
    rad = true;
  } else if(word == "p") {
    nums = &(j->pList);
    rad = true;
  } else if(word == "r") {
    nums = &(j->rList);
    rad = true;
  } 

  do {
    r(f, word);
    if (nums != NULL) {
      if (word == "<V>") {
        r(f, word);
        do {
          r(f, word);
          if (word != "}") {
            float f = atof(word.c_str());
            if (rad) {
              f = f*PI/180;
            }
            nums->push_back(f);
          }
        } while (word != "}");
        word = "";
      } else if (word[0] == '<') {
        read(f);
      }
    } else if (word[0] == '<') {
      read(f);
    }
  } while (word != "}");
}
