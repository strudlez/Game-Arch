#pragma once

#include <vector>
#include <string>
#include <map>
#include <GL/glew.h>
#include <GL/glfw.h>
#include "Vertex.h"

using namespace std;

class egg {
 public:
   egg(const char* filename);
   void load();
   void r(ifstream&f, string& s);
   void read(ifstream& f);
   void readTexture(ifstream& f);
   void readScalar(ifstream& f, map<string,string>& dat);
   void readVertex(ifstream& f);
   void readUV(ifstream& f, float& a, float &b);
   void readPolygon(ifstream& f);
   void readIndex(ifstream& f);
   vector<Vertex> vertices;
   vector<GLuint> indices;
   map<const string, GLuint> textures;
   GLuint TexId;
   string texture;
 private:
};
