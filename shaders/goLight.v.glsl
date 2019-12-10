#version 330 core

// TODO #B
in vec3 vPosition;

// TODO #F1
out vec3 theColor;

// TODO #A
uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

uniform float time;


void main() {
    //*****************************************
    //********* Vertex Calculations  **********
    //*****************************************
    float scale = 0.4;
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(vPosition*scale, 1.0);

    theColor = vec3(1.0, 1.0, 1.0);

    if (time == 3.0){
        theColor = vec3(1.0, 0.0, 0.0);
    }
    if (time == 2.0){
        theColor = vec3(0.0, 1.0, 1.0);
    }
    if (time == 1.0){
        theColor = vec3(0.0, 1.0, 0.0);
    }

}
