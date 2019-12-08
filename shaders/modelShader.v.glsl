#version 330 core

// TODO #B
in vec3 vPosition;
in vec3 normal;
in vec2 texel;

// TODO #F1
out vec3 theColor;
out vec3 fragPos;
out vec3 normalX;
out vec2 tex;

// TODO #A
uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;
uniform vec3 lightPos;
uniform vec3 viewPos;


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




    gl_Position = mvpMatrix * vec4(newPosition.x*scale+changePos.x,newPosition.y*scale+changePos.y,newPosition.z*scale+changePos.z, 1.0);

    theColor = vec3(1.0, 1.0, 1.0);

    normalX = mat3(transpose(inverse(modelMatrix))) * normal;

    fragPos = vec3(modelMatrix * vec4(vPosition, 1.0));

}
