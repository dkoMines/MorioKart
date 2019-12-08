#version 330 core

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D textur;

in vec3 theColor;
in vec3 normalX;
in vec3 fragPos;
in vec2 tex;


out vec4 fragColorOut;


void main() {
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    // Diffuse
    vec3 norm = normalize(normalX);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    // Specular
    float specularStrength = 0.8;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient+diffuse+specular) * theColor;
    fragColorOut = vec4(result, 1.0);



}