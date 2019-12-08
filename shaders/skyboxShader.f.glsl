/*
 *   Fragment Shader
 *
 *   CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 330 core

// TODO #F3
in vec3 theColor;
in vec2 texCoordOut;
uniform sampler2D textSamp;

// TODO #D
out vec4 fragColorOut;

void main() {

    //*****************************************
    //******* Final Color Calculations ********
    //*****************************************

    fragColorOut = texture(textSamp,texCoordOut);

}
