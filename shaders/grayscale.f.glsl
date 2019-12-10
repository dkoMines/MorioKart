#version 330 core

in vec2 texCoord;

uniform sampler2D fbo;
uniform int isMap;
uniform int hitPenguin;

out vec4 fragColorOut;

const float off = 1.0 / 300.0;

void main() {
    vec2 offsets[9] = vec2[](
            vec2(-off,  off), // top-left
            vec2( 0.0f,    off), // top-center
            vec2( off,  off), // top-right
            vec2(-off,  0.0f),   // center-left
            vec2( 0.0f,    0.0f),   // center-center
            vec2( off,  0.0f),   // center-right
            vec2(-off, -off), // bottom-left
            vec2( 0.0f,   -off), // bottom-center
            vec2( off, -off)  // bottom-right
        );

    float kernel[9] = float[](
            1.0/16.0, 2.0/16.0, 1.0/16.0,
            2.0/16.0, 4.0/16.0, 2.0/16.0,
            1.0/16.0, 2.0/16.0, 1.0/16.0
        );


    vec3 sample[9];
        for(int i = 0; i < 9; i++)
        {
            sample[i] = vec3(texture(fbo, texCoord.st + offsets[i]));
        }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sample[i] * kernel[i]/1.6;

    vec4 texel = texture( fbo, texCoord );
    vec4 finalColor;

    if (isMap == 1 || hitPenguin == 0) {
        finalColor = texel;
    }else {
    finalColor = vec4(col, 1.0);//texel;
    }
    
    // TODO #A

    //float color_c = (finalColor.x + finalColor.y + finalColor.z)/3.0f;

    //finalColor = vec4(color_c * 0.1, color_c * 0.95, color_c * 0.2, 1.0);

    //if(length(vec2(0.5,0.5) - texCoord) > .48){
    //        finalColor = vec4(0.0,0.0,0.0,1);
    //}

    //if(texCoord.s > 0.495 && texCoord.s < 0.505 && texCoord.t > 0.405 && texCoord.t < 0.595)
    //    finalColor = vec4(1,0,0,1);
    //if(texCoord.t > 0.495 && texCoord.t < 0.505 && texCoord.s > 0.405 && texCoord.s < 0.595)
    //    finalColor = vec4(1,0,0,1);


    fragColorOut = finalColor;//clamp(finalColor, 0.0, 1.0);
}
