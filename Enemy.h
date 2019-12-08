//
// Created by dko on 12/3/2019.
//

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
#include <Shader_Utils.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <math.h>       /* sin */

#ifndef A7_ENEMY_H
#define A7_ENEMY_H


class Enemy {
public:
    
    Enemy(glm::vec3 position, float groundSize, float heroSize):location(position), groundSize(groundSize), heroSize(heroSize){
        setupShader();
        setupBuffers();
        direction.x = getRand()*2-1;
        direction.z = getRand()*2-1;
        direction.y=0;
        direction = glm::normalize(direction);
        animateTime = 50;
        animateDir = true;

    }
    
    void setupShader();
    void setupBuffers();
    bool updatePosition(glm::vec3 heroPos, vector<Enemy*> all_enemies);
    void renderModel(glm::mat4 viewMtx, glm::mat4 projMtx, glm::vec3 eyePoint);
    float getRand(void);
    glm::vec3 location;
    glm::vec3 direction;
    float speed = 0.1;
    float size = 1.0;

private:
    // World Info
    float groundSize;
    float heroSize;
    float animateTime;
    bool animateDir;
    // Enemy Info

    bool dying = false;

    // Shader and VBOs and VAOs

    // Model
    const char* enemy_objFileLocation = "models/suzanne.obj";

    GLuint enemyNormalVBOs;
    GLuint enemy_change_uniform_location;

    GLint vpos_model;
    glm::mat4 modelMtx = glm::mat4(1.0f);

    GLuint enemyShaderHandle = 0;
    GLint model_mvp = -1;
    GLint lightPosLoc = -1;
    GLint modelMtxLoc = -1;
    GLint camPosLoc = -1;
    GLint timeLoc = -1;
    CSCI441::ModelLoader* enemy_model = NULL;

    glm::vec3 lightPos = {0.0,10.0,0.0};

    GLint norm_attrib_location;

};


#endif //A7_ENEMY_H
