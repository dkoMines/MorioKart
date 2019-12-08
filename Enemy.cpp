//
// Created by dko on 12/3/2019.
//

#include "Enemy.h"

void Enemy::setupShader() {
    // Model
    enemyShaderHandle = createShaderProgram("shaders/satanShader.v.glsl","shaders/satanShader.f.glsl");
    model_mvp = glGetUniformLocation(enemyShaderHandle, "mvpMatrix");
    vpos_model = glGetAttribLocation(enemyShaderHandle, "vPosition");
    norm_attrib_location = glGetAttribLocation(enemyShaderHandle, "normal");
    modelMtxLoc = glGetUniformLocation(enemyShaderHandle, "modelMatrix");
    lightPosLoc = glGetUniformLocation(enemyShaderHandle, "lightPos");
    camPosLoc = glGetUniformLocation(enemyShaderHandle, "viewPos");
    timeLoc = glGetUniformLocation(enemyShaderHandle, "time");
    enemy_change_uniform_location = glGetUniformLocation(enemyShaderHandle, "changePos");
}

void Enemy::setupBuffers() {
    // ============================================= Model time =================================
    enemy_model = new CSCI441::ModelLoader();
    enemy_model->loadModelFile( enemy_objFileLocation );

    // Load Normals
    ifstream modelFile;
    modelFile.open(enemy_objFileLocation);
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
    glGenBuffers( 1, &enemyNormalVBOs);
    glBindBuffer( GL_ARRAY_BUFFER, enemyNormalVBOs);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normalArray),normalArray, GL_STATIC_DRAW);
    glEnableVertexAttribArray( norm_attrib_location );
    glVertexAttribPointer( norm_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

bool Enemy::updatePosition(glm::vec3 heroPos, vector<Enemy*> all_enemies) {

    // Check enemy collision
    for (auto enemy : all_enemies){
        if (enemy->location == location){
            // This is the exact same enemy
        }
        else {
            if (glm::distance(enemy->location,location)<size+size){
                // Collided
                location = location + direction*speed;
                enemy->location = enemy->location + enemy->direction*enemy->speed;
                // Marble i
                glm::vec3 normalI = glm::normalize(enemy->location-location);
                glm::vec3 dirI = direction;
                direction = dirI -2 * glm::dot(dirI, normalI)*normalI;
                // Marble j
                glm::vec3 normalJ = glm::normalize(location-enemy->location);
                glm::vec3 dirJ = enemy->direction;
                enemy->direction = dirJ -2 * glm::dot(dirJ, normalJ)*normalJ;
                location = location - direction*speed;
                enemy->location = enemy->location - enemy->direction*enemy->speed;
            }

        }
    }

    // Check hero collision
    if (glm::distance(heroPos,location)<heroSize+size){
        // Doesn't do anything to enemy
    }

    if (location.x > groundSize+1 || location.x < -groundSize-1 ||
        location.z > groundSize+1 || location.z < -groundSize-1
        ){
        direction.y += 0.2;
        direction = glm::normalize(direction);
        location = location - direction*speed;
        dying = true;

        if (location.y < -15){
            return false;
        }
    }


    if (!dying){
        glm::vec3 axis = glm::cross(heroPos-location, direction);
        glm::mat4 matrix = glm::rotate(glm::mat4(1.0f),0.025f,axis);
        glm::vec4 direction_4 = matrix*glm::vec4(direction,1);
        direction_4.y = 0;
        direction = glm::normalize(glm::vec3(direction_4.x,0,direction_4.z));
        location = location - direction*speed;

        return true;
    }

    return true;
}

void Enemy::renderModel(glm::mat4 viewMtx, glm::mat4 projMtx, glm::vec3 eyePoint) {
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;;
    // HERO
    glUseProgram(enemyShaderHandle);
    glUniformMatrix4fv(model_mvp, 1, GL_FALSE, &mvpMtx[0][0]);
    glUniformMatrix4fv(modelMtxLoc, 1, GL_FALSE, &modelMtx[0][0]);
    glUniform3fv(lightPosLoc, 1, &lightPos[0]);
    glUniform3fv(camPosLoc, 1, &eyePoint[0]);
    glUniform1f(timeLoc, animateTime);
    glUniform3fv(enemy_change_uniform_location, 1, &location[0]);
    enemy_model->draw( vpos_model );
    if (animateTime > 100){
        animateDir = false;
    } else if (animateTime < 50){
        animateDir = true;
    }
    if (animateDir){
        animateTime ++;
    } else {animateTime--;}


}

float Enemy::getRand(void){
    float rz = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // Between 0 and 1
    return rz;
}