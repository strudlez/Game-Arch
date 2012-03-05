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
#include "egg.h"

int CurrentWidth = 800,
    CurrentHeight = 450,
    WindowHandle = 0;
double thistime, oldtime, dt, starttime; //not floats!


//FOV, aspect ratio, near, far
glm::mat4 ProjectionMatrix = glm::perspective(60.0f, 16.0f / 9.0f, 0.1f, 100.f);
glm::mat4 ViewMatrix = glm::mat4(1.0f);
glm::mat4 ModelMatrix = glm::mat4(1.0f);
glm::mat3 NormalMatrix = glm::mat3(1.0f);

unsigned FrameCount = 0;
static const double PI = 3.14159265358979323846;

GLuint
    ProjectionMatrixUniformLocation,
    ViewMatrixUniformLocation,
    ModelMatrixUniformLocation,
    NormalMatrixUniformLocation,
    TimeLocation,
    samplerLoc,
    BufferIds[3] = { 0 },
    ShaderIds[3] = { 0 },
    TexId;

egg* model;

float ModelPitch = 0;
float ModelYaw = 0;
double LastTime = 0;

//Mouse vars
int mouseX, mouseY;
bool mouseClick = false;

void Initialize(int, char*[]);
void InitWindow(void);
void RenderFunction(void);
void CreateModel(char* file);
void DrawModel(void);
void DestroyModel(void);
void game_loop(void);
void checkShader(GLuint);
GLuint LoadShader(const char*, GLenum);
void OnGLError(const char*);
GLboolean LoadTexture( char* );
void handleMouse();
void handleKeys(int, int);

int main(int argc, char* argv[]) {
  Initialize(argc, argv);

  game_loop();

  DestroyModel();

  exit(EXIT_SUCCESS);
}

void Initialize(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s filename.egg\n", argv[0]);
    return;
  }
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
  ViewMatrix = glm::lookAt(glm::vec3(0.0, 0.0, 15.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
  //ViewMatrix = glm::translate(ViewMatrix, glm::vec3(0,0,-2));

  glfwSetCharCallback( *handleKeys );

  CreateModel(argv[1]);
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
      case GLFW_KEY_UP: printf("AAAAA\n"); break;
    }
  }

}

void RenderFunction(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  DrawModel();
}

void CreateModel(char* file) {
  printf("Create\n");

  model = new egg(file);

  ShaderIds[0] = glCreateProgram();
  printf("%d\n",ShaderIds[0]);
  OnGLError("ERROR: Could not create the shader program");

  ShaderIds[1] = LoadShader("egg.fs", GL_FRAGMENT_SHADER);
  checkShader(ShaderIds[1]);
  ShaderIds[2] = LoadShader("egg.vs", GL_VERTEX_SHADER);
  checkShader(ShaderIds[2]);
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * model->vertices.size(),
      &model->vertices[0], GL_STATIC_DRAW);
  OnGLError("ERROR: Could not bind the VBO to the VAO");

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 
      sizeof(model->vertices[0]), (GLvoid*)0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 
      sizeof(model->vertices[0]),
      (GLvoid*)sizeof(model->vertices[0].Position));

  OnGLError("ERROR: Could not set VAO attributes");

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIds[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
      sizeof(GLuint) * model->indices.size(), 
      &model->indices[0], GL_STATIC_DRAW);
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
  if (!glfwLoadTexture2D( model->texture.c_str(), 0 ))
    fprintf( stderr, "Failed to load texture" );

  samplerLoc = glGetUniformLocation(ShaderIds[0], "s_tex");
  glUniform1i(samplerLoc, 0);
}

void DestroyModel() {
  glDetachShader(ShaderIds[0], ShaderIds[1]);
  glDetachShader(ShaderIds[0], ShaderIds[2]);
  glDeleteShader(ShaderIds[1]);
  glDeleteShader(ShaderIds[2]);
  glDeleteProgram(ShaderIds[0]);
  OnGLError("ERROR: Could not destroy the shaders");

  glDeleteBuffers(2, &BufferIds[1]);
  glDeleteVertexArrays(1, &BufferIds[0]);
  OnGLError("ERROR: Could not destroy the buffer objects");
}

void DrawModel(void) {
  ModelMatrix = glm::mat4(1.0f);
  
  ModelMatrix = glm::rotate(ModelMatrix, ModelYaw, glm::vec3(0, 1, 0)); //rotateH
  ModelMatrix = glm::rotate(ModelMatrix, ModelPitch, glm::vec3(1, 0, 0)); //rotateP
  
  NormalMatrix[0][0] = ModelMatrix[0][0];
  NormalMatrix[0][1] = ModelMatrix[0][1];
  NormalMatrix[0][2] = ModelMatrix[0][2];
  NormalMatrix[1][0] = ModelMatrix[1][0];
  NormalMatrix[1][1] = ModelMatrix[1][1];
  NormalMatrix[1][2] = ModelMatrix[1][2];
  NormalMatrix[2][0] = ModelMatrix[2][0];
  NormalMatrix[2][1] = ModelMatrix[2][1];
  NormalMatrix[2][2] = ModelMatrix[2][2];
  NormalMatrix = glm::transpose(glm::inverse(NormalMatrix));

  glUseProgram(ShaderIds[0]);
  OnGLError("DRAW_ERROR: Could not use the shader program");

  glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE,
      glm::value_ptr(ModelMatrix));
  glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE,
      glm::value_ptr(ViewMatrix));
  glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE,
      glm::value_ptr(ProjectionMatrix));
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
