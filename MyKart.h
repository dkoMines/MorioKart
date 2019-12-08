//
// Created by dko on 12/7/2019.
//

#ifndef A7_MYKART_H
#define A7_MYKART_H

#include "Model_Base.h"

class MyKart : public Model_Base{
public:
    // Constructor
    MyKart(glm::vec3 startingLocation):location(startingLocation){
        setupShader();
        setupBuffers();
        theta = (float) M_PI;
    }

    // Functions
    void setupShader();
    void setupBuffers();
    void updatePosition();
    void renderModel(glm::mat4 viewMtx, glm::mat4 projMtx, glm::vec3 eyePoint);

    void left();
    void right();
    void accelUp();
    void accelDown();
    void rotate(float rotate);


    // Public variables
    glm::vec3 location;
    glm::vec3 direction = glm::vec3(0,0,-1);
    float speed = 1.0;

private:
    // World Info
    float groundSize;
    float heroSize;
    float animateTime = 0;
    glm::vec3 lightPos = glm::vec3(0,10,0);
    bool animateDir;

    float theta;

    float rotationTick = 0.1;
    bool alive = true;


    // Model / Buffers
    const char* model_file_name = "models/Kart.obj";
    CSCI441::ModelLoader* this_model = NULL;
    GLuint thisNormalVBOs;
    float modelScale = 0.1;


    // Shader Locations/ Handles
    GLuint this_ShaderHandle = 0;
    GLint this_model_mvp = -1;
    GLint this_lightPosLoc = -1;
    GLint this_modelMtxLoc = -1;
    GLint this_camPosLoc = -1;
    GLint this_timeLoc = -1;
    GLuint this_change_uniform_location;
    GLint this_norm_attrib_location;
    GLint this_vpos_model;
    GLint this_scale_loc;

    // Misc
    glm::mat4 modelMtx = glm::mat4(1.0f);


};


#endif //A7_MYKART_H
