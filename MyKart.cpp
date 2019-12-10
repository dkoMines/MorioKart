//
// Created by dko on 12/7/2019.
//

#include "MyKart.h"

void MyKart::setupShader() {
    // Shader Setup
//    this_ShaderHandle = createShaderProgram("shaders/modelShader.v.glsl","shaders/modelShader.f.glsl");
    this_shader_program = new CSCI441::ShaderProgram("shaders/modelShader.v.glsl","shaders/modelShader.f.glsl");
    this_vpos_model = this_shader_program->getAttributeLocation("vPosition");
    this_norm_attrib_location = this_shader_program->getAttributeLocation("normal");
    this_texel_attrib_location = this_shader_program->getAttributeLocation("texel");
    this_texture_uniform_location = this_shader_program->getUniformLocation("textur");
    this_model_mvp = this_shader_program->getUniformLocation("mvpMatrix");
    this_modelMtxLoc = this_shader_program->getUniformLocation("modelMatrix");
    //this_viewMtxLoc = this_shader_program->getUniformLocation("viewMatrix");
    this_lightPosLoc = this_shader_program->getUniformLocation("LightPos");
    this_camPosLoc = this_shader_program->getUniformLocation("viewPos");
    this_timeLoc = this_shader_program->getUniformLocation("time");
    this_change_uniform_location = this_shader_program->getUniformLocation("changePos");
    this_scale_loc = this_shader_program->getUniformLocation("scale");
}

void MyKart::setupBuffers() {
    // Sets up our model
    this_model = new CSCI441::ModelLoader();
    this_model->enableAutoGenerateNormals();
    this_model->loadModelFile( model_file_name );
    this_texture_uniform_handle = CSCI441::TextureUtils::loadAndRegisterTexture(text_file_name);

}

void MyKart::updatePosition() {
    if (checkFall(location)){
        fallingCount ++;
    } else if (alive){
        location.y = normalY;
        fallingCount = 0;
        thetaLR = 0;
    }
    if (fallingCount > 10||!alive){
        location.y -= 0.4;
        alive = false;
        thetaLR += 0.3;
    }
    location = location + direction*speed;

    if (!alive && location.y < -20){
        // Respawn
        alive = true;
        location = mylastPlatform;
        direction = mylastDirection;
        thetaLR = 0;
        speed = 0;
    }

    int num = checkNum();
    if (num>-1 && num < 10){
        if (num == target){
            modelScale = 0.07;
            target += 1;
            if (target == 10){target=0;}
        }
        if (num-1 > target || num +1 < target ) {
            if (target==0 && num ==9){}
            else{
                modelScale = 0.02;
            }
            target = num +1;
            if (target == 10){target=0;}
        }
    }



}

int MyKart::checkNum(){

    for (auto platform : numLayout){
        float xPlus = platform.x*groundSize + groundSize;
        float xMinus = platform.x*groundSize - groundSize;
        float zPlus = platform.z*groundSize + groundSize;
        float zMinus = platform.z*groundSize - groundSize;

        if (location.x < xPlus && location.x > xMinus && location.z < zPlus && location.z > zMinus){
            return platform.a;
        }

    }
    return -1;
}

bool MyKart::checkFall(glm::vec3 position){

    for (auto platform : platformLayout){
        float xPlus = platform.x*groundSize + groundSize;
        float xMinus = platform.x*groundSize - groundSize;
        float zPlus = platform.z*groundSize + groundSize;
        float zMinus = platform.z*groundSize - groundSize;

        if (location.x < xPlus && position.x > xMinus && position.z < zPlus && position.z > zMinus){
            mylastPlatform = glm::vec3(platform.x*groundSize, normalY + 0.2, platform.z*groundSize);
            mylastDirection = direction;
            return false;
        }

    }
    return true;
}

void MyKart::renderModel(glm::mat4 viewMtx, glm::mat4 projMtx, glm::vec3 eyePoint) {

    glm::mat4 oldModelMtx = modelMtx;

    glm::vec3 rotationAxis = glm::vec3(0,1,0);
    modelMtx = glm::translate( modelMtx, location );
    // M_PI/2 = +x
    // 0 = +Z
    modelMtx = glm::rotate( modelMtx, (float)(theta), rotationAxis );

    glm::vec3 rotationAxisFB = glm::cross(direction,rotationAxis);
    modelMtx = glm::rotate( modelMtx, (float)(thetaLR), rotationAxisFB );

//    modelMtx = glm::rotate( modelMtx, (float)(thetaLR), direction );





    glm::vec3 zero = glm::vec3(0,0,0);
    updatePosition();
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // Set our shader's info
    this_shader_program->useProgram();
//    glUseProgram(this_ShaderHandle);
    glUniformMatrix4fv(this_model_mvp, 1, GL_FALSE, &mvpMtx[0][0]);
    glUniformMatrix4fv(this_modelMtxLoc, 1, GL_FALSE, &modelMtx[0][0]);
    //glUniformMatrix4fv(this_viewMtxLoc, 1, GL_FALSE, &viewMtx[0][0]);
    glUniform3fv(this_lightPosLoc, 1, &lightPos[0]);
    glUniform3fv(this_camPosLoc, 1, &eyePoint[0]);
    glUniform1f(this_timeLoc, animateTime);
    glUniform3fv(this_change_uniform_location, 1, &zero[0]);
    glUniform1f(this_scale_loc, modelScale);
//    glUniform1ui(this_texture_uniform_location, GL_TEXTURE0);
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

    modelMtx = oldModelMtx;
}

void MyKart::left() {
    if (!alive){return;}
    if (speed == 0){return;}
    theta += rotationTick+sqrt(abs(speed)*100)/2500;
    glm::vec3 axis = glm::vec3(0,1,0);
    glm::mat4 matrix = glm::rotate(glm::mat4(1.0f),theta,axis);
    glm::vec4 direction_4 = matrix*glm::vec4(0,0,1,1);
    direction_4.y = 0;
    direction = glm::normalize(glm::vec3(direction_4.x,0,direction_4.z));
}
void MyKart::right() {
    if (!alive){return;}
    if (speed == 0){return;}
    theta -= rotationTick+sqrt(abs(speed)*100)/2500;
    glm::vec3 axis = glm::vec3(0,1,0);
    glm::mat4 matrix = glm::rotate(glm::mat4(1.0f),theta,axis);
    glm::vec4 direction_4 = matrix*glm::vec4(0,0,1,1);
    direction_4.y = 0;
    direction = glm::normalize(glm::vec3(direction_4.x,0,direction_4.z));
}

void MyKart::accelUp() {
    if (!alive){return;}
    if (speed<0.2){
        speed += 0.03;
    }
    if (speed<0.4){
        speed += 0.003;
    }
    if (speed<0.7){
        speed += 0.001;
    }
    speed *= 1.0001;
    if (speed > maxSpeed){speed = maxSpeed;}
}
void MyKart::accelDown() {
    if (!alive){return;}
    if (speed > 0){
        speed -= 0.08;
    } else {

        speed -= 0.03;
        if (speed < -0.5){speed = -0.5;}


    }
}

void MyKart::noAccel() {
    if (!alive){return;}
    if (speed > 0){
        speed -= 0.01;
    }
    else if (speed < 0){
        speed += 0.01;
    }
    if (speed < 0.03 && speed > -0.03) {speed=0;}
}
