#include "egg.h"
#include "Vertex.h"

#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

egg::egg() {
}

egg::egg(const char* filename) {
  readVert = false;
  ViewMatrix = glm::mat4(1.0f);
  ModelMatrix = glm::mat4(1.0f);
  NormalMatrix = glm::mat3(1.0f);
  
  ModelPitch = 0;
  ModelYaw = 0;
  ifstream f(filename);

  if (f) {
    while (!f.eof()) {
      read(f);
    }
  }
}

void egg::load(GLuint shader1, GLuint shader2) {

  ShaderIds[0] = glCreateProgram();


  ShaderIds[1] = shader1;
  ShaderIds[2] = shader2;

/*  ShaderIds[1] = LoadShader("egg2.fs", GL_FRAGMENT_SHADER);
  checkShader(ShaderIds[1]);
  ShaderIds[2] = LoadShader("egg2.vs", GL_VERTEX_SHADER);
  checkShader(ShaderIds[2]);*/
  glAttachShader(ShaderIds[0], ShaderIds[1]);
  glAttachShader(ShaderIds[0], ShaderIds[2]);

  //if not using "location" in shader
  glBindAttribLocation(ShaderIds[0], 0, "in_Position");
  glBindAttribLocation(ShaderIds[0], 1, "in_Tex");
  glBindAttribLocation(ShaderIds[0], 2, "in_Normal");

  glLinkProgram(ShaderIds[0]);

  OnGLError("ERROR: Could not link the shader program");

  TimeLocation = glGetUniformLocation(ShaderIds[0], "time");
  OnGLError("ERROR: Could not get time uniform locations");
  ModelMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ModelMatrix");
  OnGLError("ERROR: Could not get Model uniform locations");
  ViewMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ViewMatrix");
  OnGLError("ERROR: Could not get View uniform locations");
  ProjectionMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ProjectionMatrix");
  OnGLError("ERROR: Could not get Projection uniform locations");
  NormalMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "NormalMatrix");
  OnGLError("ERROR: Could not get Normal Matrix uniform locations");

  TimeLocation = glGetUniformLocation(ShaderIds[0], "time");

  glGenVertexArrays(1, &BufferIds[0]);
  OnGLError("ERROR: Could not generate the VAO");
  glBindVertexArray(BufferIds[0]);
  OnGLError("ERROR: Could not bind the VAO");

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  OnGLError("ERROR: Could not enable vertex attributes");

  glGenBuffers(2, &BufferIds[1]);
  OnGLError("ERROR: Could not generate the buffer objects");

  glBindBuffer(GL_ARRAY_BUFFER, BufferIds[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(),
      &vertices[0], GL_STATIC_DRAW);
  OnGLError("ERROR: Could not bind the VBO to the VAO");

  //Vertex
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 
      sizeof(vertices[0]), (GLvoid*)0);

  //UV
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 
      sizeof(vertices[0]),
      (GLvoid*)sizeof(vertices[0].Position));

  OnGLError("ERROR: Could not set VAO attributes");

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIds[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
      sizeof(GLuint) * indices.size(), 
      &indices[0], GL_STATIC_DRAW);
  OnGLError("ERROR: Could not bind the IBO to the VAO");

  glBindVertexArray(0);

  // Generate texture objects
  glGenTextures( 1, &TexId );

  // Make texture object active
  glBindTexture( GL_TEXTURE_2D, TexId );

  // Set texture parameters
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

  // Upload texture from file to texture memory, autouses glTexImage2D, needs TGA
  if (!glfwLoadTexture2D( texture.c_str(), 0 ))
    fprintf( stderr, "Failed to load texture %s\n", texture.c_str() );

  samplerLoc = glGetUniformLocation(ShaderIds[0], "s_tex");
  glUniform1i(samplerLoc, 0);

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
  
  if(!readVert) {
    subVert = num;
    readVert = true;
  }

  
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

  float scale = 1;

  Vertex v = {{x/scale, y/scale, z/scale}, {uvA, uvB}};

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

  indices.push_back(a-readVert);
  indices.push_back(b-readVert);
  indices.push_back(c-readVert);

  //cout << a-1 << "," << b-1 << "," << c-1 << ",";
}

void egg::OnGLError(const char* error_message) {
  const GLenum ErrorValue = glGetError();

  if (ErrorValue != GL_NO_ERROR) {
    const char* APPEND_DETAIL_STRING = ": %s\n";
    const size_t APPEND_LENGTH = strlen(APPEND_DETAIL_STRING) + 1;
    const size_t message_length = strlen(error_message);
    char* display_message = (char*)malloc(message_length + APPEND_LENGTH);

    memcpy(display_message, error_message, message_length);
    memcpy(&display_message[message_length], APPEND_DETAIL_STRING, APPEND_LENGTH);

    fprintf(stderr, display_message, gluErrorString(ErrorValue));

    free(display_message);
  }
}

void egg::DrawModel(glm::mat4& View, glm::mat4& Proj, glm::mat4& Model) {
  glBindTexture( GL_TEXTURE_2D, TexId );

  NormalMatrix = glm::transpose(glm::inverse(NormalMatrix));

  glUseProgram(ShaderIds[0]);
  OnGLError("DRAW_ERROR: Could not use the shader program");

  glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE,
      glm::value_ptr(Model));
  glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE,
      glm::value_ptr(View));
  glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE,
      glm::value_ptr(Proj));
  glUniformMatrix3fv(NormalMatrixUniformLocation, 1, GL_FALSE, 
      glm::value_ptr(NormalMatrix));
  OnGLError("ERROR: Could not set the shader uniforms");

  glBindVertexArray(BufferIds[0]);
  OnGLError("ERROR: Could not bind the VAO for drawing purposes");

  glDrawElements(GL_TRIANGLES, 5412, GL_UNSIGNED_INT, (GLvoid*)0);
  OnGLError("ERROR: Could not draw the cube");

  glBindVertexArray(0);
  glUseProgram(0);
}

void egg::DestroyModel() {
  glDetachShader(ShaderIds[0], ShaderIds[1]);
  glDetachShader(ShaderIds[0], ShaderIds[2]);
  glDeleteProgram(ShaderIds[0]);
  OnGLError("ERROR: Could not destroy the shaders");

  glDeleteBuffers(2, &BufferIds[1]);
  glDeleteVertexArrays(1, &BufferIds[0]);
  OnGLError("ERROR: Could not destroy the buffer objects");
}

