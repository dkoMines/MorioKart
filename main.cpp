/*
 *  CSCI 441, Computer Graphics, Fall 2019
 *
 *  Project: lab10
 *  File: main.cpp
 *
 *  Description:
 *      This file contains the basic setup to work with GLSL Geometry shader.
 *
 *  Author: Dr. Paone, Colorado School of Mines, 2019
 *
 */

//*************************************************************************************

#include <GL/glew.h>        // include GLEW to get our OpenGL 3.0+ bindings
#include <GLFW/glfw3.h>			// include GLFW framework header

// include GLM libraries and matrix functions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdio.h>				// for printf functionality
#include <stdlib.h>				// for exit functionality
#include <time.h>					// for time functionality

// note that all of these headers end in *3.hpp
// these class library files will only work with OpenGL 3.0+
#include <CSCI441/modelLoader3.hpp> // to load in OBJ models
#include <CSCI441/objects3.hpp>     // to render our 3D primitives
#include <CSCI441/OpenGLUtils3.hpp> // to print info about OpenGL
#include <CSCI441/ShaderProgram3.hpp>   // our shader helper functions
#include <CSCI441/TextureUtils.hpp>   // our texture helper functions
#include "Shader_Utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <math.h>       /* sin */
#include <algorithm>

#include "Enemy.h"
#include "Enemy.cpp"

#include "MyKart.h"
#include "MyKart.cpp"
#include "Penguin.h"
#include "Penguin.cpp"


//*************************************************************************************
//
// Global Parameters
int windowWidth, windowHeight;
string controlFileName = "ControlFile.txt";
bool controlDown = false;
bool kartCamera = true;
bool leftMouseDown = false;
glm::vec2 mousePosition( -9999.0f, -9999.0f );

glm::vec3 cameraAngles( 1.82f, 2.01f, 15.0f );
glm::vec3 eyePoint(   10.0f, 10.0f, 10.0f );
glm::vec3 lookAtPoint( 0.0f,  0.0f,  0.0f );
glm::vec3 upVector(    0.0f,  1.0f,  0.0f );

GLint vpos_attrib_location;
struct Vertex { GLfloat x, y, z; };

// Skybox
GLuint skyboxVAO, skyboxVBO, skyboxTexVBO;
GLuint skyboxTextureHandles[6];
GLuint skyboxShaderProgramHandle = 0;
CSCI441::ShaderProgram* skyBoxShaderProgram = NULL;
GLint mvp_uniform_location, skybox_vpos, skybox_tloc;

glm::vec3 lightPos = {0.0,10.0,0.0};


// Platform
const GLfloat GROUND_SIZE = 3;
GLuint platformVAOd;
GLuint platformTextureHandle, finishTextureHandle;
CSCI441::ShaderProgram* floorShaderProgram = NULL;
struct TextureShaderUniformLocations {
    GLint modelMtx;
    GLint viewProjectionMtx;
    GLint tex;
    GLint color;
} textureShaderUniforms;
struct TextureShaderAttributeLocations {
    GLint vPos;
    GLint vTextureCoord;
} textureShaderAttributes;

// + More our own platform variables THE ROAD
vector<glm::vec3> platform_layout;
int platformNum;
vector<char> roadChars = {'x','S'};
vector<int> finishIndex;


// MyKart
glm::vec3 myKartPosition = glm::vec3(0,1.5,0);
bool alive = true;
MyKart* myKart;

//Penguin
glm::vec3 penguinPosition = glm::vec3(10, 1.5, 10);
Penguin* penguin;

//******************************************************************************
//
// Helper Functions

GLfloat randNumber( int max ) {
	return rand() / (GLfloat)RAND_MAX * max * 2.0 - max;
}

// convertSphericalToCartesian() ///////////////////////////////////////////////
//
// This function updates the camera's position in cartesian coordinates based
//  on its position in spherical coordinates. Should be called every time
//  cameraAngles is updated.
//
////////////////////////////////////////////////////////////////////////////////
void convertSphericalToCartesian() {
    float kartY = 6.0;
    if (kartCamera){
        eyePoint.x =  myKartPosition.x - myKart->direction.x*10;
        eyePoint.y = kartY + myKartPosition.y;
        eyePoint.z =  myKartPosition.z - myKart->direction.z*10;
    } else {
        eyePoint.x = cameraAngles.z * sinf( cameraAngles.x ) * sinf( cameraAngles.y ) + myKartPosition.x;
        eyePoint.y = cameraAngles.z * -cosf( cameraAngles.y ) + myKartPosition.y;
        eyePoint.z = cameraAngles.z * -cosf( cameraAngles.x ) * sinf( cameraAngles.y )+ myKartPosition.z;
    }

}

//******************************************************************************
//
// Event Callbacks

// error_callback() ////////////////////////////////////////////////////////////
//
//		We will register this function as GLFW's error callback.
//	When an error within GLFW occurs, GLFW will tell us by calling
//	this function.  We can then print this info to the terminal to
//	alert the user.
//
////////////////////////////////////////////////////////////////////////////////
static void error_callback(int error, const char* description) {
	fprintf(stderr, "[ERROR]: %s\n", description);
}

// key_callback() //////////////////////////////////////////////////////////////
//
//		We will register this function as GLFW's keypress callback.
//	Responds to key presses and key releases
//
////////////////////////////////////////////////////////////////////////////////
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if ((key == GLFW_KEY_ESCAPE || key == 'Q') && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

  if( action == GLFW_PRESS ) {
    switch( key ) {
      case GLFW_KEY_ESCAPE:
      case GLFW_KEY_Q:
        glfwSetWindowShouldClose( window, GLFW_TRUE );
        break;
      case GLFW_KEY_P:
          if (kartCamera){
              kartCamera = false;
          } else {
              kartCamera = true;
          }

        break;
    }
  }
}

// mouse_button_callback() /////////////////////////////////////////////////////
//
//		We will register this function as GLFW's mouse button callback.
//	Responds to mouse button presses and mouse button releases.  Keeps track if
//	the control key was pressed when a left mouse click occurs to allow
//	zooming of our arcball camera.
//
////////////////////////////////////////////////////////////////////////////////
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && alive) {
		leftMouseDown = true;
	} else {
		leftMouseDown = false;
		mousePosition.x = -9999.0f;
		mousePosition.y = -9999.0f;
	}
  controlDown = mods & GLFW_MOD_CONTROL;
}

// cursor_callback() ///////////////////////////////////////////////////////////
//
//		We will register this function as GLFW's cursor movement callback.
//	Responds to mouse movement.  When active motion is used with the left
//	mouse button an arcball camera model is followed.
//
////////////////////////////////////////////////////////////////////////////////
static void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
	// make sure movement is in bounds of the window
	// glfw captures mouse movement on entire screen
	if( xpos > 0 && xpos < windowWidth ) {
		if( ypos > 0 && ypos < windowHeight ) {
			// active motion
			if( leftMouseDown ) {
				if( (mousePosition.x - -9999.0f) < 0.001f ) {
					mousePosition.x = xpos;
					mousePosition.y = ypos;
				} else {
					if( !controlDown ) {
						cameraAngles.x += (xpos - mousePosition.x)*0.005f;
						cameraAngles.y += (ypos - mousePosition.y)*0.005f;

						if( cameraAngles.y < 0 ) cameraAngles.y = 0.0f + 0.001f;
						if( cameraAngles.y >= M_PI ) cameraAngles.y = M_PI - 0.001f;
					} else {
						double totChgSq = (xpos - mousePosition.x) + (ypos - mousePosition.y);
						cameraAngles.z += totChgSq*0.01f;

						if( cameraAngles.z <= 2.0f ) cameraAngles.z = 2.0f;
						if( cameraAngles.z >= 50.0f ) cameraAngles.z = 50.0f;
					}
					convertSphericalToCartesian();

					mousePosition.x = xpos;
					mousePosition.y = ypos;
				}
			}
			// passive motion
			else {

			}
		}
	}
}

// scroll_callback() ///////////////////////////////////////////////////////////
//
//		We will register this function as GLFW's scroll wheel callback.
//	Responds to movement of the scroll where.  Allows zooming of the arcball
//	camera.
//
////////////////////////////////////////////////////////////////////////////////
static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset ) {
	double totChgSq = yOffset;
	cameraAngles.z += totChgSq*0.01f;

	if( cameraAngles.z <= 2.0f ) cameraAngles.z = 2.0f;
	if( cameraAngles.z >= 50.0f ) cameraAngles.z = 50.0f;

	convertSphericalToCartesian();
}

//******************************************************************************
//
// Setup Functions

// setupGLFW() /////////////////////////////////////////////////////////////////
//
//		Used to setup everything GLFW related.  This includes the OpenGL context
//	and our window.
//
////////////////////////////////////////////////////////////////////////////////
GLFWwindow* setupGLFW() {
	// set what function to use when registering errors
	// this is the ONLY GLFW function that can be called BEFORE GLFW is initialized
	// all other GLFW calls must be performed after GLFW has been initialized
	glfwSetErrorCallback(error_callback);

	// initialize GLFW
	if (!glfwInit()) {
		fprintf( stderr, "[ERROR]: Could not initialize GLFW\n" );
		exit(EXIT_FAILURE);
	} else {
		fprintf( stdout, "[INFO]: GLFW initialized\n" );
	}

	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );						// request forward compatible OpenGL context
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );	// request OpenGL Core Profile context
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );		// request OpenGL 3.x context
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );		// request OpenGL 3.3 context

	// create a window for a given size, with a given title
	GLFWwindow *window = glfwCreateWindow(640, 480, "A6: Fireworks", NULL, NULL);
	if( !window ) {						// if the window could not be created, NULL is returned
		fprintf( stderr, "[ERROR]: GLFW Window could not be created\n" );
		glfwTerminate();
		exit( EXIT_FAILURE );
	} else {
		fprintf( stdout, "[INFO]: GLFW Window created\n" );
	}

	glfwMakeContextCurrent(	window );	// make the created window the current window
	glfwSwapInterval( 1 );				    // update our screen after at least 1 screen refresh

	glfwSetKeyCallback( 			  window, key_callback				  );	// set our keyboard callback function
	glfwSetMouseButtonCallback( window, mouse_button_callback );	// set our mouse button callback function
	glfwSetCursorPosCallback(	  window, cursor_callback  			);	// set our cursor position callback function
	glfwSetScrollCallback(			window, scroll_callback			  );	// set our scroll wheel callback function

	return window;										// return the window that was created
}

// setupOpenGL() ///////////////////////////////////////////////////////////////
//
//      Used to setup everything OpenGL related.
//
////////////////////////////////////////////////////////////////////////////////
void setupOpenGL() {
	glEnable( GL_DEPTH_TEST );					// enable depth testing
	glDepthFunc( GL_LESS );							// use less than depth test

	glEnable(GL_BLEND);									// enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// use one minus blending equation

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	// clear the frame buffer to black

	glEnable( GL_TEXTURE_2D );					// enable 2D texturing

	glPointSize( 4.0 );									// make our points bigger
}

// setupGLEW() /////////////////////////////////////////////////////////////////
//
//      Used to initialize GLEW
//
////////////////////////////////////////////////////////////////////////////////
void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum glewResult = glewInit();

	/* check for an error */
	if( glewResult != GLEW_OK ) {
		printf( "[ERROR]: Error initalizing GLEW\n");
		/* Problem: glewInit failed, something is seriously wrong. */
  	fprintf( stderr, "[ERROR]: %s\n", glewGetErrorString(glewResult) );
		exit(EXIT_FAILURE);
	} else {
		 fprintf( stdout, "[INFO]: GLEW initialized\n" );
		 fprintf( stdout, "[INFO]: Status: Using GLEW %s\n", glewGetString(GLEW_VERSION) );
	}

	if( !glewIsSupported( "GL_VERSION_2_0" ) ) {
		printf( "[ERROR]: OpenGL not version 2.0+.  GLSL not supported\n" );
		exit(EXIT_FAILURE);
	}
}

// setupShaders() //////////////////////////////////////////////////////////////
//
//      Create our shaders.  Send GLSL code to GPU to be compiled.  Also get
//  handles to our uniform and attribute locations.
//
////////////////////////////////////////////////////////////////////////////////
void setupShaders() {
	// Skybox set up
    skyboxShaderProgramHandle = createShaderProgram("shaders/skyboxShader.v.glsl","shaders/skyboxShader.f.glsl");
    skyBoxShaderProgram = new CSCI441::ShaderProgram("shaders/skyboxShader.v.glsl","shaders/skyboxShader.f.glsl");
    mvp_uniform_location = skyBoxShaderProgram->getUniformLocation("mvpMatrix");
    skybox_vpos = skyBoxShaderProgram->getAttributeLocation("vPosition");
    skybox_tloc = skyBoxShaderProgram->getAttributeLocation("texCoordIn");


    // Ground //
    floorShaderProgram = new CSCI441::ShaderProgram( "shaders/textureShader.v.glsl", "shaders/textureShader.f.glsl" );
    textureShaderUniforms.modelMtx          = floorShaderProgram->getUniformLocation( "modelMtx" );
    textureShaderUniforms.viewProjectionMtx = floorShaderProgram->getUniformLocation( "viewProjectionMtx" );
    textureShaderUniforms.tex               = floorShaderProgram->getUniformLocation( "tex" );
    textureShaderUniforms.color             = floorShaderProgram->getUniformLocation( "color" );

    textureShaderAttributes.vPos            = floorShaderProgram->getAttributeLocation( "vPos" );
    textureShaderAttributes.vTextureCoord   = floorShaderProgram->getAttributeLocation( "vTextureCoord" );
    // Objective

}

// particleBuffers() //////////////////////////////////////////////////////////////
//
//      Create our VAOs & VBOs. Send vertex data to the GPU for future rendering
//
////////////////////////////////////////////////////////////////////////////////
void setupBuffersSky(){
    Vertex skyPoints[8];
    // SKYBOX BUFFERS
    // Sides for my cube
    float scale = 500.0;
    float yLow = -1*scale;
    float yHigh = scale;
    float xLow = -1*scale;
    float xHigh = scale;
    float zLow = -1*scale;
    float zHigh = scale;
    // Left | Bottom | Close
    skyPoints[0] = {xLow, yLow, zLow};
    // Left | Bottom | Far
    skyPoints[1] = {xLow, yLow, zHigh};
    // Right | Bottom | Far
    skyPoints[2] = {xHigh, yLow, zHigh};
    // Right | Bottom | Close
    skyPoints[3] = {xHigh, yLow, zLow};
    // Left | High | Close
    skyPoints[4] = {xLow, yHigh, zLow};
    // Left | High | Far
    skyPoints[5] = {xLow, yHigh, zHigh};
    // Right | High | Far
    skyPoints[6] = {xHigh, yHigh, zHigh};
    // Right | High | Close
    skyPoints[7] = {xHigh, yHigh, zLow};
    // Fill new array
    Vertex pointsOrdered[36];
    int order[36] = {0,2,1,3,2,0,   // bot side
                     6,4,5,6,7,4,   // Top Side
                     2,5,1,2,6,5,   // Back Side
                     0,7,3,0,4,7,   // Front side
                     3,6,2,3,7,6,   // Right Side
                     1,4,0,1,5,4};  // Left Side
    for (int i=0; i<36;i++){
        pointsOrdered[i] = skyPoints[order[i]];
    }
    glm::vec2 texCoords[4];
    texCoords[0] = {0,0};
    texCoords[1] = {1,0};
    texCoords[3] = {0,1};
    texCoords[2] = {1,1};
    int orderTex[36] = {
            0,2,1,3,2,0, // bot side
            1,3,2,1,0,3, // top side
            0,2,1,0,3,2, // back side
            0,2,1,0,3,2, // another side
            0,2,1,0,3,2, // another side
            0,2,1,0,3,2, // another side

    };
    glm::vec2 texCoordsOrdered[36];
    for (int i=0;i<36;i++){
        texCoordsOrdered[i] = texCoords[orderTex[i]];
    }
    glGenVertexArrays( 1, &skyboxVAO );
    glBindVertexArray( skyboxVAO );

    // skybox vbos
    // Generates my points to the skybox VBOs
    glGenBuffers( 1, &skyboxVBO );
    glBindBuffer( GL_ARRAY_BUFFER, skyboxVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(pointsOrdered), pointsOrdered, GL_STATIC_DRAW );
    glEnableVertexAttribArray( skybox_vpos );
    glVertexAttribPointer( skybox_vpos, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    //     Generates my texture skybox VBO
    glGenBuffers( 1, &skyboxTexVBO);
    glBindBuffer( GL_ARRAY_BUFFER, skyboxTexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoordsOrdered),texCoordsOrdered, GL_STATIC_DRAW);
    glEnableVertexAttribArray( skybox_tloc );
    glVertexAttribPointer( skybox_tloc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // ==================================================================== Ground =================================================================
    platformNum = platform_layout.size();
    const int NUM_PLATFORMS = platformNum;

//    platform_layout.push_back(glm::vec3(2,0,0));
//    platform_layout.push_back(glm::vec3(0,0,0));
//    platform_layout.push_back(glm::vec3(0,-1,2));

    float vertScale = 5.0;

    struct VertexTextured {
        float x, y, z;
        float s, t;
    };

    GLfloat platformSize = GROUND_SIZE;

    VertexTextured platformVertices[NUM_PLATFORMS*4];
    unsigned short platformIndices[NUM_PLATFORMS*6];

    for (int i=0;i<NUM_PLATFORMS;i++){
        glm::vec3 this_layout = platform_layout[i];


        platformVertices[i*4] = { -platformSize + platformSize*this_layout.x, 0.0f + vertScale*this_layout.y, -platformSize + platformSize*this_layout.z,   0.0f,  0.0f }; // 0 - BL
        platformVertices[i*4+1] ={  platformSize + platformSize*this_layout.x, 0.0f + vertScale*this_layout.y, -platformSize + platformSize*this_layout.z,   1.0f,  0.0f }; // 1 - BR
        platformVertices[i*4+2] ={ -platformSize + platformSize*this_layout.x, 0.0f + vertScale*this_layout.y,  platformSize + platformSize*this_layout.z,   0.0f,  1.0f }; // 2 - TL
        platformVertices[i*4+3] ={  platformSize + platformSize*this_layout.x, 0.0f + vertScale*this_layout.y,  platformSize + platformSize*this_layout.z,   1.0f,  1.0f };  // 3 - TR

        platformIndices[i*6] = 0+i*4;
        platformIndices[i*6+1] = 1+i*4;
        platformIndices[i*6+2] = 2+i*4;
        platformIndices[i*6+3] = 1+i*4;
        platformIndices[i*6+4] = 3+i*4;
        platformIndices[i*6+5] = 2+i*4;

    }

    glGenVertexArrays( 1, &platformVAOd );
    glBindVertexArray( platformVAOd );

    GLuint vbods[2];
    glGenBuffers( 2, vbods );

    glBindBuffer( GL_ARRAY_BUFFER, vbods[0] );
    glBufferData( GL_ARRAY_BUFFER, sizeof( platformVertices ), platformVertices, GL_STATIC_DRAW );

    glEnableVertexAttribArray( textureShaderAttributes.vPos );
    glVertexAttribPointer( textureShaderAttributes.vPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTextured), (void*) 0 );

    glEnableVertexAttribArray( textureShaderAttributes.vTextureCoord );
    glVertexAttribPointer( textureShaderAttributes.vTextureCoord, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTextured), (void*) (sizeof(float) * 3) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbods[1] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( platformIndices ), platformIndices, GL_STATIC_DRAW );
}


void setupTextures() {
    platformTextureHandle = CSCI441::TextureUtils::loadAndRegisterTexture( "textures/metal.jpg" );
    finishTextureHandle = CSCI441::TextureUtils::loadAndRegisterTexture( "textures/Finish-Line.jpg" );

    skyboxTextureHandles[0] = CSCI441::TextureUtils::loadAndRegisterTexture( "textures/skybox/negy.jpg" );
    skyboxTextureHandles[1] = CSCI441::TextureUtils::loadAndRegisterTexture( "textures/skybox/posy.jpg" );
    skyboxTextureHandles[2] = CSCI441::TextureUtils::loadAndRegisterTexture( "textures/skybox/posx.jpg" );
    skyboxTextureHandles[3] = CSCI441::TextureUtils::loadAndRegisterTexture( "textures/skybox/negx.jpg" );
    skyboxTextureHandles[4] = CSCI441::TextureUtils::loadAndRegisterTexture( "textures/skybox/posz.jpg" );
    skyboxTextureHandles[5] = CSCI441::TextureUtils::loadAndRegisterTexture( "textures/skybox/negz.jpg" );
}



//******************************************************************************
//
// Rendering / Drawing Functions - this is where the magic happens!

// renderScene() ///////////////////////////////////////////////////////////////
//
//		This method will contain all of the objects to be drawn.
//
////////////////////////////////////////////////////////////////////////////////
void renderScene( glm::mat4 viewMtx, glm::mat4 projMtx ) {
    penguin->location = penguinPosition;
    myKartPosition = myKart->location;

    // Skybox
    // stores our model matrix
    glm::mat4 modelMtx = glm::mat4(1.0f);
    // use our shader program
    // TODO #9A
    skyBoxShaderProgram->useProgram();
//    glUseProgram(skyboxShaderProgramHandle);
    // precompute our MVP CPU side so it only needs to be computed once
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;;
    // send MVP to GPU
    // TODO #9B
    glUniformMatrix4fv(mvp_uniform_location, 1, GL_FALSE, &mvpMtx[0][0]);
    glBindVertexArray( skyboxVAO );
    // LOOKHERE #4
    int count = 0;
    for (int i=0;i<36;i=i+6){
        glBindTexture( GL_TEXTURE_2D, skyboxTextureHandles[count] );
        glDrawArrays(GL_TRIANGLES, i, 6);
        count ++;
    }

    myKart->renderModel(viewMtx,projMtx,eyePoint);
    penguin->renderModel(viewMtx, projMtx, eyePoint);




    // draw the platform
    // set all our uniforms
    floorShaderProgram->useProgram();

    glm::mat4 modelMatrix(1.0f), vp = projMtx * viewMtx;
    glm::vec4 white(1,1,1,1);

    glUniformMatrix4fv(textureShaderUniforms.modelMtx, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(textureShaderUniforms.viewProjectionMtx, 1, GL_FALSE, &vp[0][0]);
    glUniform1ui(textureShaderUniforms.tex, GL_TEXTURE0);
    glUniform4fv(textureShaderUniforms.color, 1, &white[0]);


    glBindVertexArray( platformVAOd );
    for (int i=0;i<platformNum*12+1;i+=12){

        glBindTexture( GL_TEXTURE_2D, platformTextureHandle );
        if (find(finishIndex.begin(), finishIndex.end(), i/12) != finishIndex.end()) {
            glBindTexture(GL_TEXTURE_2D, finishTextureHandle);
        }

        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)i );

    }



    // Fix camera
    glm::vec3 camDir = glm::normalize(eyePoint - lookAtPoint);
}

///*****************************************************************************
//
// Our main function

// main() ///////////////////////////////////////////////////////////////
//
//		Really you should know what this is by now.
//
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char *argv[] ) {
	srand( time(0) );									// seed our RNG

    // Read in our control file
    ifstream control(controlFileName);
    if (!control){
        cout << "Control File Does not exist";
        exit(1);
    }
    else {
        int count = 0;
        int z = -20;
        string line;
        // Create Map
        while (getline (control, line)){
            int x = -20;
            for (auto c : line){
                x += 2;
                if ( find(roadChars.begin(), roadChars.end(), c) != roadChars.end() ){ // Build normal road
                    platform_layout.push_back(glm::vec3(x,0,z));
                    count ++;
                }
                if (c == 'S'){ // Starting Position
                    myKartPosition = glm::vec3(x*GROUND_SIZE, 1.0, z*GROUND_SIZE);
                }
                if (c=='O'){ // Finish Line
                    platform_layout.push_back(glm::vec3(x,0,z));
                    finishIndex.push_back(count);
                    count++;
                }
            }
            z+=2;
        }
    }




  // GLFW sets up our OpenGL context so must be done first
	GLFWwindow *window = setupGLFW();	// initialize all of the GLFW specific information releated to OpenGL and our window
	setupOpenGL();										// initialize all of the OpenGL specific information
	setupGLEW();											// initialize all of the GLEW specific information

  CSCI441::OpenGLUtils::printOpenGLInfo();
	setupShaders(); 									// load our shader program into memory
    setupBuffersSky();
	setupTextures();									// load all our textures into memory

    // Generate any models that start in the game here
    myKart = new MyKart(myKartPosition);
    penguin = new Penguin(penguinPosition);



  // needed to connect our 3D Object Library to our shader
  CSCI441::setVertexAttributeLocations( vpos_attrib_location );

	convertSphericalToCartesian();		// set up our camera position

  //  This is our draw loop - all rendering is done here.  We use a loop to keep the window open
	//	until the user decides to close the window and quit the program.  Without a loop, the
	//	window will display once and then the program exits.
	while( !glfwWindowShouldClose(window) ) {	// check if the window was instructed to be closed
    glDrawBuffer( GL_BACK );				// work with our back frame buffer
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// clear the current color contents and depth buffer in the window
		bool wsKeys = false;
        if (glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS){
            myKart->accelUp();
            wsKeys = true;
        }
        if (glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS){
            myKart->accelDown();
            wsKeys = true;
        }
        if (glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS){
            myKart->left();
        }
        if (glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS){
            myKart->right();
        }
        if (!wsKeys){
            myKart->noAccel();
        }
        myKartPosition = myKart->location;
        convertSphericalToCartesian();
		// Get the size of our framebuffer.  Ideally this should be the same dimensions as our window, but
		// when using a Retina display the actual window can be larger than the requested window.  Therefore
		// query what the actual size of the window we are rendering to is.
		glfwGetFramebufferSize( window, &windowWidth, &windowHeight );

		// update the viewport - tell OpenGL we want to render to the whole window
		glViewport( 0, 0, windowWidth, windowHeight );

		// set the projection matrix based on the window size
		// use a perspective projection that ranges
		// with a FOV of 45 degrees, for our current aspect ratio, and Z ranges from [0.001, 1000].
		glm::mat4 projectionMatrix = glm::perspective( 45.0f, windowWidth / (float) windowHeight, 0.001f, 1000.0f );

		// set up our look at matrix to position our camera
		glm::mat4 viewMatrix = glm::lookAt( eyePoint,glm::vec3(myKartPosition.x,myKartPosition.y+2.0,myKartPosition.z), upVector );

		// draw everything to the window
		// pass our view and projection matrices as well as deltaTime between frames
		renderScene( viewMatrix, projectionMatrix );

		glfwSwapBuffers(window);// flush the OpenGL commands and make sure they get rendered!
		glfwPollEvents();				// check for any events and signal to redraw screen
	}

  glfwDestroyWindow( window );// clean up and close our window
	glfwTerminate();						// shut down GLFW to clean up our context

	return EXIT_SUCCESS;
}