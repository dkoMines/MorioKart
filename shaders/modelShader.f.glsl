#version 330 core

uniform vec3 light_1_pos;
uniform vec3 light_2_pos;
uniform vec4 light_1_color;
uniform vec4 light_2_color;

uniform vec3 viewPos;
uniform sampler2D textur;

in vec3 theColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 tex;

out vec4 fragColorOut;

void main() {

    vec3 lightColor1 = vec3(light_1_color.x,light_1_color.y,light_1_color.z);

    // Ambient calculations
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor1;

    // Normal and light vectors
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_1_pos - FragPos);

    // Diffuse calculations
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor1;

    // Specular variable
    float specularStrength = 0.5;

    // Specular requisite vectorsmak
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    // Specular calculations
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor1;

    vec3 lightColor2 = vec3(light_2_color.x,light_2_color.y,light_2_color.z);

    // Ambient calculations
    ambientStrength = 0.03;
    vec3 ambient2 = ambientStrength * lightColor2;

    // Normal and light vectors
    norm = normalize(Normal);
    lightDir = normalize(light_2_pos - FragPos);

    // Diffuse calculations
    diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse2 = diff * lightColor2;

    // Specular variable
    specularStrength = 0.7;

    // Specular requisite vectorsmak
    viewDir = normalize(viewPos - FragPos);
    reflectDir = reflect(-lightDir, norm);

    // Specular calculations
    spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular2 = specularStrength * spec * lightColor2;



    // Result
    vec3 result = (ambient + diffuse+ specular + ambient2 + diffuse2+ specular2);
    fragColorOut = vec4(result * vec3(texture2D(textur, tex)) + specular, 1.0);
    //    fragColorOut = vec4(1.0, 1.0, 1.0, 1.0);
}