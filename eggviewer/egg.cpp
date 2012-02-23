#include "egg.h"
#include "vertex.h"

#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

egg::egg(const char* filename) {
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

  Vertex v = {{x/100, y/100, z/100}, {uvA, uvB}};

  vertices.push_back(v);

  /*cout << "{{" << x/100 << ", " << y/100 << ", " << z/100 << "}, {" <<
    uvA << ", " << uvB << "}},\n";*/

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

  //cout << a-1 << "," << b-1 << "," << c-1 << ",";
}
