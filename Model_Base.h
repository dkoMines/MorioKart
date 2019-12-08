//
// Created by dko on 12/7/2019.
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


#ifndef A7_MODEL_BASE_H
#define A7_MODEL_BASE_H

/**
 * Abstract class to create new models from that will set up the shaders and buffers, draw and update the character.
 */
class Model_Base {
public:

    virtual void setupShader() = 0;
    virtual void setupBuffers() = 0;
    virtual void renderModel(glm::mat4 viewMtx, glm::mat4 projMtx, glm::vec3 eyePoint) = 0;

    float getRand(void){
        float rz = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // Between 0 and 1
        return rz;
    }


private:

};


#endif //A7_MODEL_BASE_H
