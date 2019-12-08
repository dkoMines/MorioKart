#version 330 core

// TODO #B
in vec3 vPosition;
in vec3 normal;

// TODO #F1
out vec3 theColor;
out vec3 FragPos;
out vec3 normalX;

// TODO #A
uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 changePos;


void main() {
    //*****************************************
    //********* Vertex Calculations  **********
    //*****************************************
    float scale = 1; // Change this to see the object better

    gl_Position = mvpMatrix * vec4(vPosition.x*scale+changePos.x,vPosition.y*scale+changePos.y,vPosition.z*scale+changePos.z, 1.0);

    theColor = vec3(1.0, 1.0, 1.0);

    normalX = mat3(transpose(inverse(modelMatrix))) * normal;

    FragPos = vec3(modelMatrix * vec4(vPosition, 1.0));

}
