/*
 *   Vertex Shader
 *
 *   CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 330 core

// TODO #B
in vec3 vPosition;
in vec2 texCoordIn;

// TODO #F1
out vec3 theColor;
out vec2 texCoordOut;

// TODO #A
uniform mat4 mvpMatrix;



void main() {
    //*****************************************
    //********* Vertex Calculations  **********
    //*****************************************


    // TODO #C
    gl_Position = mvpMatrix * vec4(vPosition, 1.0);

    texCoordOut = texCoordIn;

    // TODO #F2
    theColor = vPosition;
}
