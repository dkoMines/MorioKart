#version 330 core

in vec3 vPosition;
in vec3 normal;
in vec2 texel;

out vec3 theColor;
out vec3 FragPos;
out vec3 Normal;
out vec2 tex;

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

uniform float time;
uniform float scale;

uniform vec3 changePos;


void main() {
    //*****************************************
    //********* Vertex Calculations  **********
    //*****************************************

    vec3 newPosition = vPosition;
    //if (newPosition.y<0.5 && (newPosition.x > 0.5 || newPosition.x < -0.5))
    //    newPosition.y -= time/50;


    tex = texel;

    gl_Position = mvpMatrix * vec4(newPosition.x*scale+changePos.x,newPosition.y*scale+changePos.y,newPosition.z*scale+changePos.z, 1.0);

    theColor = vec3(.20392, .623529, .7294);

    Normal = normal;

    FragPos = vec3(modelMatrix * vec4(vPosition, 1.0));

}
