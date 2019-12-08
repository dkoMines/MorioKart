//
// Created by dko on 12/7/2019.
//

#include "MyKart.h"

void MyKart::setupShader() {
    // Shader Setup
    this_ShaderHandle = createShaderProgram("shaders/modelShader.v.glsl","shaders/modelShader.f.glsl");
    this_model_mvp = glGetUniformLocation(this_ShaderHandle, "mvpMatrix");
    this_vpos_model = glGetAttribLocation(this_ShaderHandle, "vPosition");
    this_norm_attrib_location = glGetAttribLocation(this_ShaderHandle, "normal");
    this_modelMtxLoc = glGetUniformLocation(this_ShaderHandle, "modelMatrix");
    this_lightPosLoc = glGetUniformLocation(this_ShaderHandle, "lightPos");
    this_camPosLoc = glGetUniformLocation(this_ShaderHandle, "viewPos");
    this_timeLoc = glGetUniformLocation(this_ShaderHandle, "time");
    this_change_uniform_location = glGetUniformLocation(this_ShaderHandle, "changePos");
    this_scale_loc = glGetUniformLocation(this_ShaderHandle, "scale");
}

void MyKart::setupBuffers() {
    // Sets up our model
    this_model = new CSCI441::ModelLoader();
    this_model->loadModelFile( model_file_name );

    // Load Normals
    ifstream modelFile;
    modelFile.open(model_file_name);
    vector<glm::vec3> allNormals;
    while(!modelFile.eof()){
        string word;
        modelFile >> word;
        if (word=="vn"){
            glm::vec3 normal;
            float piece;
            modelFile >> piece;
            normal.x = piece;
            modelFile >> piece;
            normal.y = piece;
            modelFile >> piece;
            normal.z = piece;
            allNormals.push_back(normal);
        }
    }
    // Shift to an array
    glm::vec3 normalArray[allNormals.size()];
    for (int i=0;i<allNormals.size();i++){
        normalArray[i] = allNormals[i];
    }
    // Generate Normals VBO
    glGenBuffers( 1, &thisNormalVBOs);
    glBindBuffer( GL_ARRAY_BUFFER, thisNormalVBOs);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normalArray),normalArray, GL_STATIC_DRAW);
    glEnableVertexAttribArray( this_norm_attrib_location );
    glVertexAttribPointer( this_norm_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
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
    glUseProgram(this_ShaderHandle);
    glUniformMatrix4fv(this_model_mvp, 1, GL_FALSE, &mvpMtx[0][0]);
    glUniformMatrix4fv(this_modelMtxLoc, 1, GL_FALSE, &modelMtx[0][0]);
    glUniform3fv(this_lightPosLoc, 1, &lightPos[0]);
    glUniform3fv(this_camPosLoc, 1, &eyePoint[0]);
    glUniform1f(this_timeLoc, animateTime);
    glUniform3fv(this_change_uniform_location, 1, &location[0]);
    glUniform1f(this_scale_loc, modelScale);
    this_model->draw( this_vpos_model );

//    if (animateTime > 100){
//        animateDir = false;
//    } else if (animateTime < 50){
//        animateDir = true;
//    }
//    if (animateDir){
//        animateTime ++;
//    } else {animateTime--;}
}