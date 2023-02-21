#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = Projection * View * Model * vec4(aPos, 1.0);
}


#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform float time;
ivec2 pattern = ivec2(5, 2);
uniform float frameTime = 0.2f;


uniform struct FogParameters
{
    vec4 vFogColor; // Fog color
    float fDensity; // For exp and exp2 equation
} fogParams;


float getFogFactor(FogParameters params, float fFogCoord)
{
    float fResult = 0.0;
    fResult = exp(-pow(params.fDensity * fFogCoord, 2.0));
    fResult = 1.0 - clamp(fResult, 0.0, 1.0);
    return fResult;
}

vec4 textureFromFrame(int frame) {
    vec2 offset = vec2(1.0) / vec2(pattern);

    vec2 texCoordBase = TexCoords / vec2(pattern);
    vec2 TexCoords = texCoordBase + vec2(frame % pattern.x, (frame / pattern.x)) * offset;

    return texture(texture_diffuse1, TexCoords);
}


void main()
{
    int frame = int(time / frameTime);
    FragColor = textureFromFrame(frame);
}