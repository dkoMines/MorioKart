//
// Created by dko on 12/9/2019.
//

#ifndef A7_GOLIGHT_H
#define A7_GOLIGHT_H

#include "Model_Base.h"


class GoLight : public Model_Base{

public:

    GoLight(glm::vec3 startingLocation):location(startingLocation){
        time = 3.0;
        setupShader();
        setupBuffers();
    }

    void setupShader();
    void setupBuffers();
    void renderModel(glm::mat4 viewMtx, glm::mat4 projMtx, glm::vec3 eyePoint);

    int time;
    glm::vec3 location;

private:
    // Model / Buffers
    const char* model_file_name = "models/ItmPokeBall.obj";
    CSCI441::ModelLoader* light_model = NULL;
    CSCI441::ShaderProgram* light_shader_program = NULL;


    // Shader Locations/ Handles
    GLint light_modelMtxLoc = -1;
    GLint light_timeLoc = -1;

    GLint light_viewMtxLoc;
    GLint light_vpos_model;

    // Misc
    glm::mat4 modelMtx = glm::mat4(1.0f);
};


#endif //A7_GOLIGHT_H
