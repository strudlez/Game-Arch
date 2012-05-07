#include "terrain.h"

#include<math.h>

terrain::terrain(int w, int h, float gSize, GLuint s1, GLuint s2) {
  width = w;
  height = h;
  off = 0;
  offDiff = 0;
  gridSize = gSize;
  ModelMatrix = glm::mat4(1.0f);
  model = new egg();
  model->texture = "terrain.tga";
  model->indices.resize((height-1)*(width-1)*6);
  GLuint i = 0;
  int randMax = 1000;
  float max = 15;
  float div = randMax / max;
  for (GLuint y = 0; y < height; y++) {
    for (GLuint x = 0; x < width; x++) {
      Vertex v;
      v.Position[0] = x*gSize;
      v.Position[1] = y*gSize;
      int zN = 0;
      float z = 0;
      if(x!=0) {
        z += model->vertices[y*width+x-1].Position[2];
        zN++;
      }
      if(y!=0) {
        z += model->vertices[y*width+x-width].Position[2];
        zN++;
      }
      /*if(y!=height-1) {
        z += model->vertices[y*width+x+width].Position[2];
        zN++;
      }*/
      if(zN!=0)z/=zN;
      float zT = rand()%randMax;
      zT -= randMax/2;

      zT/=div;
      z+=zT;
      v.Position[2] = z;
      z +=zT;
      v.UV[0] = (zT+max/2)/max;
      v.UV[1] = 0;
      model->vertices.push_back(v);

      if(x != width-1 && y != height-1) {
        GLuint num = y*width + x;
        model->indices[i++] = num;
        model->indices[i++] = num+1;
        model->indices[i++] = num+width+1;

        model->indices[i++] = num;
        model->indices[i++] = num+width+1;
        model->indices[i++] = num+width;
      }
    }
  }

  model->load(s1, s2);
  tex = model->TexId;
  // Generate texture objects
  glGenTextures( 1, &texWire );

  // Make texture object active
  glBindTexture( GL_TEXTURE_2D, texWire );

  // Set texture parameters
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

  // Upload texture from file to texture memory, autouses glTexImage2D, needs TGA
  if (!glfwLoadTexture2D( "car.tga", 0 ))
    fprintf( stderr, "Failed to load texture\n" );

  glBindBuffer(GL_ARRAY_BUFFER, model->BufferIds[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * model->vertices.size(),
      &model->vertices[0], GL_STATIC_DRAW);

}

void terrain::getGridPos(float x, float y, int& gridX, int& gridY) {
  gridX = x/gridSize;
  gridY = y/gridSize;
}

float terrain::getHeight(int x, int y) {
  return model->vertices[x+y*width].Position[2];
}

void terrain::Draw(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix, bool wire, glm::mat4& CarMat) {

  if(!wire) {
    int mid = 4;
    model->TexId = tex;
    glm::vec4 loc = glm::vec4(0,0,0,1);
    loc = CarMat * loc;
    int newOff = loc.y/gridSize;
    if(newOff>=mid && off+mid!=newOff)  {
      off = newOff-mid;
      glBindBuffer(GL_ARRAY_BUFFER, model->BufferIds[1]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * model->vertices.size(),
          &model->vertices[width*off], GL_STATIC_DRAW);

    }
  } else {
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    model->TexId = texWire;
  }
  model->DrawModel(ViewMatrix, ProjectionMatrix, ModelMatrix);
}
