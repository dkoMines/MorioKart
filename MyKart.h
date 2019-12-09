//
// Created by dko on 12/7/2019.
//

#ifndef A7_MYKART_H
#define A7_MYKART_H

#include "Model_Base.h"

class MyKart : public Model_Base{
public:
    // Constructor
    MyKart(glm::vec3 startingLocation, vector<glm::vec3> platformLayout, int platformSize):location(startingLocation),groundSize(platformSize),platformLayout(platformLayout){
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
    void noAccel();
    void rotate(float rotate);

    bool checkFall();



    // Public variables
    glm::vec3 location;
    glm::vec3 direction = glm::vec3(0,0,-1);
    float speed = 0.0;

private:

    int maxSpeed = 1.5;

    // World Info
    float groundSize;
    vector<glm::vec3> platformLayout;
    float heroSize;
    float animateTime = 0;
    glm::vec3 lightPos = glm::vec3(0,15,0);
    bool animateDir;


    float theta;


    float rotationTick = 0.03;
    bool alive = true;


    // Model / Buffers
    const char* model_file_name = "models/Kart.obj";
    const char* text_file_name = "textures/crashcar.png";
    CSCI441::ModelLoader* this_model = NULL;
    CSCI441::ShaderProgram* this_shader_program = NULL;
    GLuint thisNormalVBOs;
    float modelScale = 0.07;


    // Shader Locations/ Handles
    GLuint this_ShaderHandle = 0;
    GLint this_model_mvp = -1;
    GLint this_lightPosLoc = -1;
    GLint this_modelMtxLoc = -1;
    GLint this_camPosLoc = -1;
    GLint this_timeLoc = -1;
	GLint this_color_attrib_location;
    GLuint this_change_uniform_location;
    GLint this_norm_attrib_location;
    GLint this_texel_attrib_location;
    GLint this_texture_uniform_location;
    GLint this_texture_uniform_handle;
    GLint this_viewMtxLoc;
    GLint this_vpos_model;
    GLint this_scale_loc;

    // Misc
    glm::mat4 modelMtx = glm::mat4(1.0f);


};


#endif //A7_MYKART_H
