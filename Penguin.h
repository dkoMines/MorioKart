#pragma once
#include "Model_Base.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Penguin{
public:
    // Constructor
    Penguin(glm::vec3 startingLocation){
		location = startingLocation;
        setupShader();
        setupBuffers();
    }
	bool walking = false;

    // Functions
    void setupShader();
    void setupBuffers();
    void updatePosition(glm::vec3 heroPosition);
    void renderModel(glm::mat4 viewMtx, glm::mat4 projMtx, glm::vec3 eyePoint, glm::vec3 playerPosition);

    // Public variables
    glm::vec3 location;
    glm::vec3 direction = glm::vec3(1.0f,0.0f,0.0f);

private:
    int animateTimeIdle = 0;
	int animateTimeWalk = 0;
    glm::vec3 lightPos = glm::vec3(0.0f,10.0f,0.0f);
	bool updateFrame = true;

    // Model / Buffers
    const char* model_file_name = "models/riggedPenguin.obj";
    std::vector<CSCI441::ModelLoader*> walkModels;
	std::vector<CSCI441::ModelLoader*> idleModels;
	CSCI441::ShaderProgram* this_shader_program = NULL;

    GLuint thisNormalVBOs;
    float modelScale = 1;
	float angle = 0;


    // Shader Locations/ Handles
   GLuint this_ShaderHandle = 0;
    GLint this_model_mvp = -1;
    GLint this_lightPosLoc = -1;
    GLint this_modelMtxLoc = -1;
    GLint this_camPosLoc = -1;
    GLint this_timeLoc = -1;
    GLuint this_change_uniform_location;
    GLint this_norm_attrib_location;
    GLint this_texel_attrib_location;
    GLint this_texture_uniform_location;
    GLint this_texture_uniform_handle;
    GLint this_viewMtxLoc;
    GLint this_vpos_model;
    GLint this_scale_loc;
	GLint texHandle;
	
    // Misc
    glm::mat4 modelMtx = glm::mat4(1.0f);


};
