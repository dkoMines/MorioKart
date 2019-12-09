#version 330 core

uniform vec3 LightPos;
uniform vec3 viewPos;
uniform sampler2D textur;

in vec3 theColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 tex;

out vec4 fragColorOut;

void main() {
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    // Ambient calculations
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Normal and light vectors
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);

    // Diffuse calculations
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular variable
    float specularStrength = 0.5;

    // Specular requisite vectorsmak
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    // Specular calculations
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Result
    vec3 result = (ambient + diffuse+ specular);
    fragColorOut = vec4(result * vec3(texture2D(textur, tex)) + specular, 1.0);
    //    fragColorOut = vec4(1.0, 1.0, 1.0, 1.0);
}