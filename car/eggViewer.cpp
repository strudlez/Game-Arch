//cc -o morph morph.cpp -l glew -l glfw -framework Cocoa -framework OpenGL
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <GL/glew.h> //MUST come before GLFW!
#include <GL/glfw.h>
#include <glm/glm.hpp> //feel free to use your own math library!
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/core/func_geometric.hpp>
#include <time.h>
#include "egg.h"
#include "car.h"
#include "terrain.h"

int CurrentWidth = 800,
    CurrentHeight = 450,
    WindowHandle = 0;
double thistime, oldtime, dt, starttime; //not floats!


//FOV, aspect ratio, near, far
glm::mat4 ProjectionMatrix = glm::perspective(60.0f, 16.0f / 9.0f, 0.1f, 100.f);
glm::mat4 ViewMatrix = glm::mat4(1.0f);
glm::mat4 ModelMatrix = glm::mat4(1.0f);
glm::mat3 NormalMatrix = glm::mat3(1.0f);

glm::vec4 cam1 = glm::vec4(0,0,0,1);
glm::vec4 cam2 = glm::vec4(0,0,0,1);

unsigned FrameCount = 0;
static const double PI = 3.14159265358979323846;

GLuint
    ShaderIds[3] = { 0 },
    TexId;

car* model;
terrain* world;

float ModelPitch = 0;
float ModelYaw = 0;
double LastTime = 0;

//Mouse vars
int mouseX, mouseY;
bool fwd = false;
bool back = false;
bool left = false;
bool right = false;
bool mouseClick = false;

void Initialize(int, char*[]);
void InitWindow(void);
void RenderFunction(void);
egg* CreateModel(const char* file);
void DrawModel(egg* mod);
void DestroyModel(egg* mod);
void DestroyShaders(void);
void game_loop(void);
void checkShader(GLuint);
GLuint LoadShader(const char*, GLenum);
void LoadShaders();
void OnGLError(const char*);
GLboolean LoadTexture( char* );
void handleMouse();
void handleKeys(int, int);

int main(int argc, char* argv[]) {
  Initialize(argc, argv);

  game_loop();
  DestroyShaders();

  exit(EXIT_SUCCESS);
}

void Initialize(int argc, char* argv[]) {
  if (argc < 1) {
    fprintf(stderr, "Usage: %s filename.egg\n", argv[0]);
    return;
  }
  srand ( time(NULL) );
  GLenum GlewInitResult;

  InitWindow();
  OnGLError("Init window");

  glewExperimental = GL_TRUE;
  GlewInitResult = glewInit();
  OnGLError("GLEW init");

  if (GLEW_OK != GlewInitResult) {
    fprintf(
      stderr,
      "ERROR: %s\n",
      glewGetErrorString(GlewInitResult)
    );
    exit(EXIT_FAILURE);
  }
  OnGLError("GLEW string");

  fprintf(
    stdout,
    "INFO: OpenGL Version: %s\n",
    glGetString(GL_VERSION)
  );
  OnGLError("Get string");

  fprintf(
    stdout,
    "INFO: GLEW Version: %s\n",
    glewGetString(GLEW_VERSION) 
  );

  glClearColor(0.7f, 0.7f, 0.7f, 0.0f);
  OnGLError("Clear color");
  glEnable(GL_DEPTH_TEST);
  OnGLError("Depth test");
  glDepthFunc(GL_LESS);
  OnGLError("Depth Func");
  glEnable(GL_CULL_FACE);
  OnGLError("Enable cull face");
  glCullFace(GL_BACK);
  OnGLError("Cull face");
  glFrontFace(GL_CCW);
  OnGLError("Front face");
  //ViewMatrix = glm::translate(ViewMatrix, glm::vec3(0,0,-2));

  glfwSetKeyCallback( *handleKeys );
  LoadShaders();
  model = new car(ShaderIds[1], ShaderIds[2]);
  world = new terrain(100,100,30, ShaderIds[1], ShaderIds[2]);

}

void InitWindow(void) {
  // Initialize GLFW
  if (!glfwInit()) {
    fprintf( stderr, "Failed to initialize GLFW\n" );
    exit( EXIT_FAILURE );
  }

  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);
  glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  

  if( !glfwOpenWindow (CurrentWidth, CurrentHeight, 0, 0, 0, 0, 
        16, 0, GLFW_WINDOW)) {
    fprintf( stderr, "Failed to open GLFW window\n" );
    glfwTerminate();
    exit( EXIT_FAILURE );
  }

  fprintf(stderr, "GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void game_loop(void) {
  int playing;
  playing = GL_TRUE;
  while (playing && glfwGetWindowParam(GLFW_OPENED)) {

    glfwEnable(GLFW_STICKY_KEYS);
    // Frame timer
    oldtime = thistime;
    thistime = glfwGetTime();
    dt = thistime - oldtime;

    //Key events
    //Mouse checks
    handleMouse();
    // Did the user press ESC?
    if (glfwGetKey( GLFW_KEY_ESC )) {
      playing = GL_FALSE;
    }

    // Display
    //ViewMatrix = glm::rotate(ViewMatrix, ModelYaw, glm::vec3(0, 1, 0)); //rotateH
    //ViewMatrix = glm::rotate(ViewMatrix, ModelPitch, glm::vec3(1, 0, 0)); //rotateP

    RenderFunction();
    glfwSwapBuffers();
    glfwDisable(GLFW_STICKY_KEYS);
  }
}

void handleMouse() {
  int click = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
  if (click && !mouseClick) {
    glfwGetMousePos(&mouseX, &mouseY);
  } else if (click) {
    int mX, mY;
    glfwGetMousePos(&mX, &mY);
    int dX = mX - mouseX;
    int dY = mY - mouseY;
    if(dX || dY) {
      float maxRot = 360;
      
      ModelYaw += dX/3.0f;
      ModelPitch += dY/3.0f;

      if (ModelPitch > maxRot) {
        ModelPitch -= maxRot;
      } else if (ModelPitch < 0) {
        ModelPitch += maxRot;
      }
      
      if (ModelYaw > maxRot) {
        ModelYaw -= maxRot;
      } else if (ModelYaw < 0) {
        ModelYaw += maxRot;
      }
    }

    mouseX = mX;
    mouseY = mY;
  }

  mouseClick = click;

  //glfwSetMousePos();
  //glfwGetWindowParam
}

void handleKeys(int key, int pressed) {
  if (pressed == GLFW_PRESS) {
    switch (key) {
      case 'w': fwd = true; break;
      case 'W': fwd = true; break;
      case 's': back = true; break;
      case 'S': back = true; break;
      case 'a': left = true; break;
      case 'A': left = true; break;
      case 'd': right = true; break;
      case 'D': right = true; break;
      case 'Z': model->jump(); break;
    }
  } else {
    switch (key) {
      case 'w': fwd = false; break;
      case 'W': fwd = false; break;
      case 's': back = false; break;
      case 'S': back = false; break;
      case 'a': left = false; break;
      case 'A': left = false; break;
      case 'd': right = false; break;
      case 'D': right = false; break;
    }
  }



}

void RenderFunction(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  model->step(fwd-back, right-left, world);
  glm::vec4 loc = glm::vec4(0,0,0,1);
  glm::vec4 loc2 = glm::vec4(-15,0,10,1);
  loc = model->ModelMatrix * loc;
  loc2 = model->ModelMatrix * loc2;

  float c1 = cam1.z;
  float c2 = cam2.z;

  float diff = loc.z - cam1.z;
  float diff2 = loc2.z - cam2.z;
  cam1=loc;
  cam2=loc2;
  float speed = 0.1;
  if(abs(diff) < 2) { 
    cam1.z=c1;
    if(abs(diff) > 0.5) {
      if(diff>0) {
        cam1.z+=speed;
      } else {
        cam1.z-=speed;
      }
    }
  }
  if(abs(diff2) < 2) {
    cam2.z=c2;
    if(abs(diff2) > 0.5) {
      if(diff2>0) {
        cam2.z+=speed;
      } else {
        cam2.z-=speed;
      }
    }
  }
  /*
  cam2.z=c2;

  float speed = abs(model->vel.z);
  if (abs(diff) >=0.1) {
    if(diff>0) {
      cam1.z+=speed;
    } else {
      cam1.z-=speed;
    }
  }
  if (abs(diff2) >=0.1) {
    if(diff2>0) {
      cam2.z+=speed;
    } else {
      cam2.z-=speed;
    }
  }*/
  //ViewMatrix = glm::translate(ViewMatrix, glm::vec3(-loc.x, -loc.y, -loc.z)); //rotateP
  
  ViewMatrix = glm::lookAt(glm::vec3(cam2.x, cam2.y, cam2.z), glm::vec3(cam1.x, cam1.y, cam1.z), glm::vec3(0.0, 0.0, 1.0));
  //ViewMatrix = glm::rotate(ViewMatrix, 0.2f, glm::vec3(0, 1, 0)); //rotateP
  //ViewMatrix = glm::rotate(ViewMatrix, 0.2f, glm::vec3(1, 0, 0)); //rotateP
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  model->Draw(ViewMatrix, ProjectionMatrix, 0);
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  model->Draw(ViewMatrix, ProjectionMatrix, 1);

  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  world->Draw(ViewMatrix, ProjectionMatrix, 0, model->ModelMatrix);

  world->Draw(ViewMatrix, ProjectionMatrix, 1, model->ModelMatrix);
}

void LoadShaders() {
  ShaderIds[1] = LoadShader("sharmonics.fs", GL_FRAGMENT_SHADER);
  checkShader(ShaderIds[1]);
  ShaderIds[2] = LoadShader("sharmonics.vs", GL_VERTEX_SHADER);
  checkShader(ShaderIds[2]);
}

egg* CreateModel(const char* file) {
  printf("Create\n");

  egg* mod = new egg(file);
  mod->load(ShaderIds[1], ShaderIds[2]);

  return mod;

}

void DestroyShaders() {
  glDeleteShader(ShaderIds[1]);
  glDeleteShader(ShaderIds[2]);
}

void DestroyModel(egg* mod) {
  mod->DestroyModel();
}

void DrawModel(egg* mod) {
  //mod->DrawModel();
}

void checkShader(GLuint shader) {
  GLint status;

  //lGetShaderiv gets a particular parameter of the shader
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    int loglen;
    char logbuffer[1000];

    //there's also a corresponding glGetProgramInfoLog function for the linked program object
    glGetShaderInfoLog(shader, sizeof(logbuffer), &loglen, logbuffer);
    fprintf(stderr, "OpenGL Shader Compile Error:\n%.*s", loglen, logbuffer);
  } else {
    int loglen;
    char logbuffer[1000];
    glGetShaderInfoLog(shader, sizeof(logbuffer), &loglen, logbuffer);
    fprintf(stderr, "OpenGL Shader Compile OK:\n%.*s", loglen, logbuffer);
  }
}

GLuint LoadShader(const char* filename, GLenum shader_type) {
  GLuint shader_id = 0;
  FILE* file;
  long file_size = -1;
  GLchar* glsl_source;
  fprintf(stderr, "name: %s\n",filename);

  if (NULL != (file = fopen(filename, "rb")) &&
      0 == fseek(file, 0, SEEK_END) &&
      -1 != (file_size = ftell(file))) {
    rewind(file);

    if (NULL != (glsl_source = (GLchar*)malloc(file_size + 1))) {
      if (file_size == (long)fread(glsl_source, sizeof(GLchar), 
            file_size, file)) {
        glsl_source[file_size] = '\0';
        const GLchar* glsl_source_c = glsl_source;
        //fprintf(stderr, "Source: %s\n", glsl_source_c);

        if (0 != (shader_id = glCreateShader(shader_type))) {
          glShaderSource(shader_id, 1, &glsl_source_c, NULL);
          glCompileShader(shader_id);
          OnGLError("Could not compile the shader");
        } else {
          fprintf(stderr, "ERROR: Could not create the shader.\n");
        }
      } else {
        fprintf(stderr, "ERROR: Could not read file %s\n", filename);
      }

      free(glsl_source);
    } else {
      fprintf(stderr, "ERROR: Could not allocate %li bytes.\n", file_size);
    }

    fclose(file);
  } else {
    fprintf(stderr, "ERROR: Could not open file %s\n", filename);
  }

  return shader_id;
}

void OnGLError(const char* error_message) {
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
