//cc -o lighting lighting.cpp -l glew -l glfw -framework Cocoa -framework OpenGL
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <GL/glew.h> //MUST come before GLFW!
#include <GL/glfw.h>
#include <glm/glm.hpp> //feel free to use your own math library!
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "morph.h"

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
	dProjectionMatrixUniformLocation,
	dViewMatrixUniformLocation,
	dModelMatrixUniformLocation,
	dNormalMatrixUniformLocation,
	pProjectionMatrixUniformLocation,
	pViewMatrixUniformLocation,
	pModelMatrixUniformLocation,
	pNormalMatrixUniformLocation,
	sProjectionMatrixUniformLocation,
	sViewMatrixUniformLocation,
	sModelMatrixUniformLocation,
	sNormalMatrixUniformLocation,
	psProjectionMatrixUniformLocation,
	psViewMatrixUniformLocation,
	psModelMatrixUniformLocation,
	psNormalMatrixUniformLocation,
	shProjectionMatrixUniformLocation,
	shViewMatrixUniformLocation,
	shModelMatrixUniformLocation,
	shNormalMatrixUniformLocation,
	TimeLocation,
	samplerLoc,
	vaoId,
	BufferIds[2] = { 0 }, //vertex (pos, UV, normal), index
	ShaderIds[12] = { 0 }, //dir vs, dir fs, point vs, point fs, spot vs, spot fs, none va, none fs, pspot vs, pspot fs
	noneId,
	directionId,
	pointId,
	spotId,
	pspotId,
	sharmonicsId,
    TexId,
    activeShader;
	
float PandaRotation = 0;
double LastTime = 0;

void Initialize(int, char*[]);
void InitWindow(void);
void RenderFunction(void);
void CreatePanda(void);
void DrawPanda(void);
void game_loop(void);
void checkShader(GLuint);
GLuint LoadShader(const char*, GLenum);
void OnGLError(const char*);
void handleKeys(int, int);

int main(int argc, char* argv[])
{
	Initialize(argc, argv);

    game_loop();
    	
	exit(EXIT_SUCCESS);
}

void Initialize(int argc, char* argv[])
{
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
    CreatePanda();
}

void InitWindow(void)
{
	// Initialize GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit( EXIT_FAILURE );
    }
    
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);
    glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	if( !glfwOpenWindow( CurrentWidth, CurrentHeight, 0,0,0,0, 16,0, GLFW_WINDOW ) )
    {
        fprintf( stderr, "Failed to open GLFW window\n" );
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    
    fprintf(stderr, "GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void game_loop(void){
    int playing;
    playing = GL_TRUE;
    while( playing && glfwGetWindowParam( GLFW_OPENED ) )
    {
        glfwEnable(GLFW_STICKY_KEYS);
        // Frame timer
        oldtime = thistime;
        thistime = glfwGetTime();
        dt = thistime - oldtime;
        
        //Key events
        // Did the user press ESC?
        if( glfwGetKey( GLFW_KEY_ESC ) )
        {
            playing = GL_FALSE;
        }
       
        // Display
        RenderFunction();
        glfwSwapBuffers();
        glfwDisable(GLFW_STICKY_KEYS);
    }
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawPanda();
}

void handleKeys(int key, int pressed)
{
    if (pressed == GLFW_PRESS)
    {
        switch (key)
        {
            case 'q':
            {
                glUseProgram(noneId);
                activeShader = noneId;
                break;
            }
            case 'w':
            {
                glUseProgram(directionId);
                activeShader = directionId;
                break;
            }
            case 'e':
            {
                glUseProgram(pointId);
                activeShader = pointId;
                break;
            }
            case 'r':
            {
                glUseProgram(spotId);
                activeShader = spotId;
                break;
            }
            case 't':
            {
                glUseProgram(pspotId);
                activeShader = pspotId;
                break;
            }
            case 'y':
            {
                glUseProgram(sharmonicsId);
                activeShader = sharmonicsId;
            }
        }
    }
}

void CreatePanda()
{
    printf("Create\n");

    noneId = glCreateProgram();
    OnGLError("ERROR: Could not create the shader program");
	
	ShaderIds[8] = LoadShader("uber.fs", GL_FRAGMENT_SHADER);
    checkShader(ShaderIds[8]);
	ShaderIds[9] = LoadShader("uber.vs", GL_VERTEX_SHADER);
	checkShader(ShaderIds[9]);
	glAttachShader(noneId, ShaderIds[8]);
	glAttachShader(noneId, ShaderIds[9]);
	
	//if not using "location" in shader
	glBindAttribLocation(noneId, 0, "in_Position");
	glBindAttribLocation(noneId, 1, "in_Tex");
	glBindAttribLocation(noneId, 2, "in_Normal");
	
	glLinkProgram(noneId);
    OnGLError("ERROR: Could not link the shader program");

    ModelMatrixUniformLocation = glGetUniformLocation(noneId, "ModelMatrix");
    OnGLError("ERROR: Could not get Model uniform locations");
    ViewMatrixUniformLocation = glGetUniformLocation(noneId, "ViewMatrix");
    OnGLError("ERROR: Could not get View uniform locations");
    ProjectionMatrixUniformLocation = glGetUniformLocation(noneId, "ProjectionMatrix");
    OnGLError("ERROR: Could not get Projection uniform locations");
    NormalMatrixUniformLocation = glGetUniformLocation(noneId, "NormalMatrix");
    OnGLError("ERROR: Could not get Normal Matrix uniform locations");

    directionId = glCreateProgram();
    OnGLError("ERROR: Could not create the shader program");
	
	ShaderIds[0] = LoadShader("direction.fs", GL_FRAGMENT_SHADER);
    checkShader(ShaderIds[0]);
	ShaderIds[1] = LoadShader("direction.vs", GL_VERTEX_SHADER);
	checkShader(ShaderIds[1]);
	glAttachShader(directionId, ShaderIds[0]);
	glAttachShader(directionId, ShaderIds[1]);
	
	//if not using "location" in shader
	glBindAttribLocation(directionId, 0, "in_Position");
	glBindAttribLocation(directionId, 1, "in_Tex");
	glBindAttribLocation(directionId, 2, "in_Normal");
	
	glLinkProgram(directionId);
    OnGLError("ERROR: Could not link the shader program");

    dModelMatrixUniformLocation = glGetUniformLocation(directionId, "ModelMatrix");
    OnGLError("ERROR: Could not get Model uniform locations");
    dViewMatrixUniformLocation = glGetUniformLocation(directionId, "ViewMatrix");
    OnGLError("ERROR: Could not get View uniform locations");
    dProjectionMatrixUniformLocation = glGetUniformLocation(directionId, "ProjectionMatrix");
    OnGLError("ERROR: Could not get Projection uniform locations");
    dNormalMatrixUniformLocation = glGetUniformLocation(directionId, "NormalMatrix");
    OnGLError("ERROR: Could not get Normal Matrix uniform locations");
    
    
    pointId = glCreateProgram();
    OnGLError("ERROR: Could not create the direction shader program");
      
    ShaderIds[2] = LoadShader("point.fs", GL_FRAGMENT_SHADER);
    checkShader(ShaderIds[2]);
    ShaderIds[3] = LoadShader("point.vs", GL_VERTEX_SHADER);
    checkShader(ShaderIds[3]);
    glAttachShader(pointId, ShaderIds[2]);
    glAttachShader(pointId, ShaderIds[3]);
      
    glBindAttribLocation(pointId, 0, "in_Position");
    glBindAttribLocation(pointId, 1, "in_Tex");
    glBindAttribLocation(pointId, 2, "in_Normal");
      
    glLinkProgram(pointId); 
    OnGLError("ERROR: Could not link the point shader program");
    
    pModelMatrixUniformLocation = glGetUniformLocation(pointId, "ModelMatrix");
    OnGLError("ERROR: Could not get Model uniform locations");
    pViewMatrixUniformLocation = glGetUniformLocation(pointId, "ViewMatrix");
    OnGLError("ERROR: Could not get View uniform locations");
    pProjectionMatrixUniformLocation = glGetUniformLocation(pointId, "ProjectionMatrix");
    OnGLError("ERROR: Could not get Projection uniform locations");
    pNormalMatrixUniformLocation = glGetUniformLocation(pointId, "NormalMatrix");
    OnGLError("ERROR: Could not get Normal Matrix uniform locations");
    
    spotId = glCreateProgram();
    OnGLError("ERROR: Could not create the direction shader program");
   
    ShaderIds[4] = LoadShader("spot.fs", GL_FRAGMENT_SHADER);
    checkShader(ShaderIds[4]);
    ShaderIds[5] = LoadShader("spot.vs", GL_VERTEX_SHADER);
    checkShader(ShaderIds[5]);
    glAttachShader(spotId, ShaderIds[4]);
    glAttachShader(spotId, ShaderIds[5]);
    
    glBindAttribLocation(spotId, 0, "in_Position");
    glBindAttribLocation(spotId, 1, "in_Tex");
    glBindAttribLocation(spotId, 2, "in_Normal");
   
    glLinkProgram(spotId); 
    OnGLError("ERROR: Could not link the point shader program");
   
    sModelMatrixUniformLocation = glGetUniformLocation(spotId, "ModelMatrix");
    OnGLError("ERROR: Could not get Model uniform locations");
    sViewMatrixUniformLocation = glGetUniformLocation(spotId, "ViewMatrix");
    OnGLError("ERROR: Could not get View uniform locations");
    sProjectionMatrixUniformLocation = glGetUniformLocation(spotId, "ProjectionMatrix");
    OnGLError("ERROR: Could not get Projection uniform locations");
    sNormalMatrixUniformLocation = glGetUniformLocation(spotId, "NormalMatrix");
    OnGLError("ERROR: Could not get Normal Matrix uniform locations");
    
    sharmonicsId = glCreateProgram();
    OnGLError("ERROR: Could not create the direction shader program");
   
    ShaderIds[6] = LoadShader("sharmonics.fs", GL_FRAGMENT_SHADER);
    checkShader(ShaderIds[6]);
    ShaderIds[7] = LoadShader("sharmonics.vs", GL_VERTEX_SHADER);
    checkShader(ShaderIds[7]);
    glAttachShader(sharmonicsId, ShaderIds[6]);
    glAttachShader(sharmonicsId, ShaderIds[7]);
    
    glBindAttribLocation(sharmonicsId, 0, "in_Position");
    glBindAttribLocation(sharmonicsId, 1, "in_Tex");
    glBindAttribLocation(sharmonicsId, 2, "in_Normal");
   
    glLinkProgram(sharmonicsId); 
    OnGLError("ERROR: Could not link the point shader program");
   
    shModelMatrixUniformLocation = glGetUniformLocation(sharmonicsId, "ModelMatrix");
    OnGLError("ERROR: Could not get Model uniform locations");
    shViewMatrixUniformLocation = glGetUniformLocation(sharmonicsId, "ViewMatrix");
    OnGLError("ERROR: Could not get View uniform locations");
    shProjectionMatrixUniformLocation = glGetUniformLocation(sharmonicsId, "ProjectionMatrix");
    OnGLError("ERROR: Could not get Projection uniform locations");
    shNormalMatrixUniformLocation = glGetUniformLocation(sharmonicsId, "NormalMatrix");
    OnGLError("ERROR: Could not get Normal Matrix uniform locations");
    
    pspotId = glCreateProgram();
    OnGLError("ERROR: Could not create the pixel spot shader program");
   
    ShaderIds[10] = LoadShader("pspot.fs", GL_FRAGMENT_SHADER);
    checkShader(ShaderIds[10]);
    ShaderIds[11] = LoadShader("pspot.vs", GL_VERTEX_SHADER);
    checkShader(ShaderIds[11]);
    glAttachShader(pspotId, ShaderIds[10]);
    glAttachShader(pspotId, ShaderIds[11]);
    
    glBindAttribLocation(pspotId, 0, "in_Position");
    glBindAttribLocation(pspotId, 1, "in_Tex");
    glBindAttribLocation(pspotId, 2, "in_Normal");
   
    glLinkProgram(pspotId); 
    OnGLError("ERROR: Could not link the point shader program");
   
    psModelMatrixUniformLocation = glGetUniformLocation(pspotId, "ModelMatrix");
    OnGLError("ERROR: Could not get Model uniform locations");
    psViewMatrixUniformLocation = glGetUniformLocation(pspotId, "ViewMatrix");
    OnGLError("ERROR: Could not get View uniform locations");
    psProjectionMatrixUniformLocation = glGetUniformLocation(pspotId, "ProjectionMatrix");
    OnGLError("ERROR: Could not get Projection uniform locations");
    psNormalMatrixUniformLocation = glGetUniformLocation(pspotId, "NormalMatrix");
    OnGLError("ERROR: Could not get Normal Matrix uniform locations");
    
    activeShader = noneId;
    glUseProgram(noneId);
    
    //end shader stuff

	glGenVertexArrays(1, &vaoId);
    OnGLError("ERROR: Could not generate the VAO");
	glBindVertexArray(vaoId);
    OnGLError("ERROR: Could not bind the VAO");

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
    OnGLError("ERROR: Could not enable vertex attributes");

	glGenBuffers(2, BufferIds);
    OnGLError("ERROR: Could not generate the buffer objects");

	glBindBuffer(GL_ARRAY_BUFFER, BufferIds[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
    OnGLError("ERROR: Could not bind the VBO to the VAO");

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), BUFFER_OFFSET(sizeof(float)*4));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), BUFFER_OFFSET(sizeof(float)*6));
    OnGLError("ERROR: Could not set VAO attributes");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIds[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);
    OnGLError("ERROR: Could not bind the IBO to the VAO");

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
    if( !glfwLoadTexture2D( "panda.tga", 0 ) )
        fprintf( stderr, "Failed to load texture" );
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2048, 1536, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels?);
    samplerLoc = glGetUniformLocation(directionId, "s_tex");
    //glActiveTexture(GL_TEXTURE0);
    glUniform1i(samplerLoc, 0);

}

void DrawPanda(void)
{
	float PandaAngle;
	double Now = glfwGetTime();

	if (LastTime == 0)
		LastTime = Now;

	PandaRotation += 450.0f * ((float)(Now - LastTime));
    //fprintf(stderr, "Now: %f, Last: %f, CR: %f\n",Now,LastTime, (45.0f * ((float)(Now - LastTime) / CLOCKS_PER_SEC)));
	PandaAngle = PandaRotation * (float)(PI / 180); //radians
    //fprintf(stderr, "cube angle: %f\n",PandaAngle);
	LastTime = Now;

	ModelMatrix = glm::mat4(1.0f);
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0,0,-2));
    
    ModelMatrix = glm::rotate(ModelMatrix, PandaAngle, glm::vec3(0, 1, 0)); //rotateH
    ModelMatrix = glm::rotate(ModelMatrix, PandaAngle, glm::vec3(1, 0, 0)); //rotateP
    
    //calc normal matrix in eye coordinates
    //transpose of the inverse of the upper 3x3 (sheesh)
    //why not just View * Model * normal? Non-uniform scaling can screw things up
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
    
    // glUseProgram(directionId);
    //     OnGLError("DRAW_ERROR: Could not use the shader program");
    
    if (activeShader == noneId)
    {
        glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    	glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
    	glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
    	glUniformMatrix3fv(NormalMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
    }
    else if (activeShader == directionId)
    {
        glUniformMatrix4fv(dModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    	glUniformMatrix4fv(dViewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
    	glUniformMatrix4fv(dProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
    	glUniformMatrix3fv(dNormalMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
    }
    else if (activeShader == pointId)
    {
        glUniformMatrix4fv(pModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    	glUniformMatrix4fv(pViewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
    	glUniformMatrix4fv(pProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
    	glUniformMatrix3fv(pNormalMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
    }
    else if (activeShader == spotId)
    {
        glUniformMatrix4fv(sModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    	glUniformMatrix4fv(sViewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
    	glUniformMatrix4fv(sProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
    	glUniformMatrix3fv(sNormalMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
    }
    else if (activeShader == pspotId)
    {
        glUniformMatrix4fv(psModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    	glUniformMatrix4fv(psViewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
    	glUniformMatrix4fv(psProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
    	glUniformMatrix3fv(psNormalMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
    }
    else if (activeShader == sharmonicsId)
    {
        glUniformMatrix4fv(shModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    	glUniformMatrix4fv(shViewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
    	glUniformMatrix4fv(shProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
    	glUniformMatrix3fv(shNormalMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
    }
	
    OnGLError("ERROR: Could not set the shader uniforms");

	glBindVertexArray(BufferIds[0]);
    OnGLError("ERROR: Could not bind the VAO for drawing purposes");

    glDrawElements(GL_TRIANGLES, 5412, GL_UNSIGNED_INT, (GLvoid*)0);
}

void checkShader(GLuint shader)
{
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

GLuint LoadShader(const char* filename, GLenum shader_type)
{
	GLuint shader_id = 0;
	FILE* file;
	long file_size = -1;
	GLchar* glsl_source;
    fprintf(stderr, "name: %s\n",filename);

	if (NULL != (file = fopen(filename, "rb")) &&
		0 == fseek(file, 0, SEEK_END) &&
		-1 != (file_size = ftell(file)))
	{
		rewind(file);
		
		if (NULL != (glsl_source = (GLchar*)malloc(file_size + 1)))
		{
			if (file_size == (long)fread(glsl_source, sizeof(GLchar), file_size, file))
			{
				glsl_source[file_size] = '\0';
                const GLchar* glsl_source_c = glsl_source;
				//fprintf(stderr, "Source: %s\n", glsl_source_c);

				if (0 != (shader_id = glCreateShader(shader_type)))
				{
					glShaderSource(shader_id, 1, &glsl_source_c, NULL);
					glCompileShader(shader_id);
					OnGLError("Could not compile the shader");
				}
				else
					fprintf(stderr, "ERROR: Could not create the shader.\n");
			}
			else
				fprintf(stderr, "ERROR: Could not read file %s\n", filename);

			free(glsl_source);
		}
		else
			fprintf(stderr, "ERROR: Could not allocate %li bytes.\n", file_size);

		fclose(file);
	}
	else
		fprintf(stderr, "ERROR: Could not open file %s\n", filename);

	return shader_id;
}

void OnGLError(const char* error_message)
{
	const GLenum ErrorValue = glGetError();

	if (ErrorValue != GL_NO_ERROR)
	{
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
