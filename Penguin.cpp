//
// Created by dko on 12/7/2019.
//

#include "Penguin.h"

void Penguin::setupShader() {
    // Shader Setup
    this_shader_program = new CSCI441::ShaderProgram("shaders/modelShader.v.glsl","shaders/modelShader.f.glsl");
    this_vpos_model = this_shader_program->getAttributeLocation("vPosition");
    this_norm_attrib_location = this_shader_program->getAttributeLocation("normal");
    this_texel_attrib_location = this_shader_program->getAttributeLocation("texel");
    this_texture_uniform_location = this_shader_program->getUniformLocation("textur");
    this_model_mvp = this_shader_program->getUniformLocation("mvpMatrix");
    this_modelMtxLoc = this_shader_program->getUniformLocation("modelMatrix");
    this_viewMtxLoc = this_shader_program->getUniformLocation("viewMatrix");
    this_lightPosLoc = this_shader_program->getUniformLocation("LightPos");
    this_camPosLoc = this_shader_program->getUniformLocation("viewPos");
    this_timeLoc = this_shader_program->getUniformLocation("time");
    this_change_uniform_location = this_shader_program->getUniformLocation("changePos");
    this_scale_loc = this_shader_program->getUniformLocation("scale");

}

void Penguin::setupBuffers() {
    // Sets up our model
        for(int i = 1; i < 10; i++){
        CSCI441::ModelLoader* thisModel = new CSCI441::ModelLoader();
        thisModel->enableAutoGenerateNormals();
        string num = "models/penguinIdle/penguinIdle_00000" + std::to_string(i) + ".obj";
        thisModel->loadModelFile(num.c_str());
        models.push_back(thisModel);
        cout << i << endl;
    }
    for(int i = 10; i < 25; i++){
        CSCI441::ModelLoader* thisModel = new CSCI441::ModelLoader();
        thisModel->enableAutoGenerateNormals();
        string num = "models/penguinIdle/penguinIdle_0000" + std::to_string(i) + ".obj";
        thisModel->loadModelFile(num.c_str());
        models.push_back(thisModel);
        cout << i << endl;
    }
	texHandle = CSCI441::TextureUtils::loadAndRegisterTexture("textures/Penguin_grp.png");
}

void Penguin::updatePosition() {

//    glm::vec3 directionNormalized = glm::normalize( direction );
//    glm::vec3 rotationAxis = glm::vec3(0,1,0);

//    modelMtx = glm::rotate( modelMtx, (float)0.001, rotationAxis );
}

void Penguin::renderModel(glm::mat4 viewMtx, glm::mat4 projMtx, glm::vec3 eyePoint) {
    updatePosition();
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;;
    // Set our shader's info
    this_shader_program->useProgram();
    glUniformMatrix4fv(this_model_mvp, 1, GL_FALSE, &mvpMtx[0][0]);
    glUniformMatrix4fv(this_modelMtxLoc, 1, GL_FALSE, &modelMtx[0][0]);
    glUniform3fv(this_lightPosLoc, 1, &lightPos[0]);
    glUniform3fv(this_camPosLoc, 1, &eyePoint[0]);
    glUniform1f(this_timeLoc, animateTime);
    glUniform3fv(this_change_uniform_location, 1, &location[0]);
    glUniform1f(this_scale_loc, modelScale);
	glBindTexture(GL_TEXTURE_2D, texHandle);
    models.at(animateTime)->draw( this_vpos_model, this_norm_attrib_location, this_texel_attrib_location );
    
    animateTime++;
    if(animateTime >= 24){
        animateTime = 0;
    }

//    if (animateTime > 100){
//        animateDir = false;
//    } else if (animateTime < 50){
//        animateDir = true;
//    }
//    if (animateDir){
//        animateTime ++;
//    } else {animateTime--;}
}