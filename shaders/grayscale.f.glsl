#version 330 core

in vec2 texCoord;

uniform sampler2D fbo;

out vec4 fragColorOut;

void main() {
    vec4 texel = texture( fbo, texCoord );
    
    vec4 finalColor = texel;
    
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
