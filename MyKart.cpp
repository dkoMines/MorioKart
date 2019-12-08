//
// Created by dko on 12/7/2019.
//

#include "MyKart.h"

void MyKart::setupShader() {
    // Shader Setup
    this_ShaderHandle = createShaderProgram("shaders/modelShader.v.glsl","shaders/modelShader.f.glsl");
    this_shader_program = new CSCI441::ShaderProgram("shaders/modelShader.v.glsl","shaders/modelShader.f.glsl");
    this_vpos_model = this_shader_program->getAttributeLocation("vPosition");
    this_norm_attrib_location = this_shader_program->getAttributeLocation("normal");
    this_texel_attrib_location = this_shader_program->getAttributeLocation("texel");
    this_texture_uniform_location = this_shader_program->getUniformLocation("textur");
    this_model_mvp = this_shader_program->getUniformLocation("mvpMatrix");
    this_modelMtxLoc = this_shader_program->getUniformLocation("modelMatrix");
    this_lightPosLoc = this_shader_program->getUniformLocation("lightPos");
    this_camPosLoc = this_shader_program->getUniformLocation("viewPos");
    this_timeLoc = this_shader_program->getUniformLocation("time");
    this_change_uniform_location = this_shader_program->getUniformLocation("changePos");
    this_scale_loc = this_shader_program->getUniformLocation("scale");
}

void MyKart::setupBuffers() {
    // Sets up our model
    this_model = new CSCI441::ModelLoader();
    this_model->loadModelFile( model_file_name );
    this_texture_uniform_handle = CSCI441::TextureUtils::loadAndRegisterTexture(text_file_name);

    // Load Normals
//    ifstream modelFile;
//    modelFile.open(model_file_name);
//    vector<glm::vec3> allNormals;
//    while(!modelFile.eof()){
//        string word;
//        modelFile >> word;
//        if (word=="vn"){
//            glm::vec3 normal;
//            float piece;
//            modelFile >> piece;
//            normal.x = piece;
//            modelFile >> piece;
//            normal.y = piece;
//            modelFile >> piece;
//            normal.z = piece;
//            allNormals.push_back(normal);
//        }
//    }
//    // Shift to an array
//    glm::vec3 normalArray[allNormals.size()];
//    for (int i=0;i<allNormals.size();i++){
//        normalArray[i] = allNormals[i];
//    }
//    // Generate Normals VBO
//    glGenBuffers( 1, &thisNormalVBOs);
//    glBindBuffer( GL_ARRAY_BUFFER, thisNormalVBOs);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(normalArray),normalArray, GL_STATIC_DRAW);
//    glEnableVertexAttribArray( this_norm_attrib_location );
//    glVertexAttribPointer( this_norm_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void MyKart::updatePosition() {

//    glm::vec3 directionNormalized = glm::normalize( direction );
//    glm::vec3 rotationAxis = glm::vec3(0,1,0);

//    modelMtx = glm::rotate( modelMtx, (float)0.001, rotationAxis );
}

void MyKart::renderModel(glm::mat4 viewMtx, glm::mat4 projMtx, glm::vec3 eyePoint) {
    updatePosition();
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;;
    // Set our shader's info
    this_shader_program->useProgram();
    glUseProgram(this_ShaderHandle);
    glUniformMatrix4fv(this_model_mvp, 1, GL_FALSE, &mvpMtx[0][0]);
    glUniformMatrix4fv(this_modelMtxLoc, 1, GL_FALSE, &modelMtx[0][0]);
    glUniform3fv(this_lightPosLoc, 1, &lightPos[0]);
    glUniform3fv(this_camPosLoc, 1, &eyePoint[0]);
    glUniform1f(this_timeLoc, animateTime);
    glUniform3fv(this_change_uniform_location, 1, &location[0]);
    glUniform1f(this_scale_loc, modelScale);
    glUniform1ui(this_texture_uniform_location, GL_TEXTURE0);
    glBindTexture( GL_TEXTURE_2D, this_texture_uniform_handle );
    this_model->draw( this_vpos_model, this_norm_attrib_location, this_texel_attrib_location);

//    if (animateTime > 100){
//        animateDir = false;
//    } else if (animateTime < 50){
//        animateDir = true;
//    }
//    if (animateDir){
//        animateTime ++;
//    } else {animateTime--;}
}