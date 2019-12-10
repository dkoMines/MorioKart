//
// Created by dko on 12/9/2019.
//

#include "GoLight.h"
//
// Created by dko on 12/7/2019.
//


void GoLight::setupShader() {
    // Shader Setup
    // Starting Light //
    light_shader_program = new CSCI441::ShaderProgram("shaders/goLight.v.glsl","shaders/goLight.f.glsl");
    light_vpos_model = light_shader_program->getAttributeLocation("vPosition");
//    light_model_mvp = light_shader_program->getUniformLocation("mvpMatrix");
    light_modelMtxLoc = light_shader_program->getUniformLocation("modelMatrix");
    light_viewMtxLoc = light_shader_program->getUniformLocation("viewProjectionMatrix");
    light_timeLoc = light_shader_program->getUniformLocation("time");
}

void GoLight::setupBuffers() {
    // Sets up our model
    light_model = new CSCI441::ModelLoader();
    light_model->enableAutoGenerateNormals();
    light_model->loadModelFile( model_file_name );

}


void GoLight::renderModel(glm::mat4 viewMtx, glm::mat4 projMtx, glm::vec3 eyePoint) {


    glm::mat4 oldModelMtx = modelMtx;

    modelMtx = glm::translate(modelMtx, location);
    modelMtx = glm::translate(modelMtx, glm::vec3(0,8,0));

    glm::mat4 vp = projMtx * viewMtx;
    // Set our shader's info
    light_shader_program->useProgram();
//    glUniformMatrix4fv(light_model_mvp, 1, GL_FALSE, &mvpMtx[0][0]);
    glUniformMatrix4fv(light_modelMtxLoc, 1, GL_FALSE, &modelMtx[0][0]);
    glUniformMatrix4fv(light_viewMtxLoc, 1, GL_FALSE, &vp[0][0]);
    glUniform1f(light_timeLoc, 1.0);

    light_model->draw( light_vpos_model);


    modelMtx = oldModelMtx;
}


