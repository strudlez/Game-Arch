#pragma once

#include <vector>
#include <string>
#include <map>
#include <string.h>
#include <glm/glm.hpp> //feel free to use your own math library!
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GL/glfw.h>
#include "Vertex.h"

using namespace std;

class egg {
 public:

   egg(const char* filename);
   egg();
   void load(GLuint shader1, GLuint shader2);
   void OnGLError(const char* error_message);
   void DrawModel(glm::mat4& View, glm::mat4& Proj, glm::mat4& Model);
   void DestroyModel();
   GLuint LoadShader(const char* filename, GLenum shader_type);
   void checkShader(GLuint shader);
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
   GLuint subVert;
   GLuint BufferIds[3];
   GLuint ShaderIds[3];
   glm::mat4 ProjectionMatrix;
   glm::mat4 ViewMatrix;
   glm::mat4 ModelMatrix;
   glm::mat3 NormalMatrix;
   
   float ModelPitch;
   float ModelYaw;
   double LastTime;
   bool readVert;
   GLuint
    ProjectionMatrixUniformLocation,
    ViewMatrixUniformLocation,
    ParentMatrixUniformLocation,
    ModelMatrixUniformLocation,
    NormalMatrixUniformLocation,
    TimeLocation,
    samplerLoc;
 private:
};
