#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec3 FragPos;
out vec4 EyeSpaceVertex;
out vec2 TexCoords;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat3 NormalizationMatrix;


void main()
{
    EyeSpaceVertex = View * Model * vec4(aPos, 1.0);
    Normal = NormalizationMatrix * aNormal;
    FragPos = vec3(Model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
    gl_Position = Projection * EyeSpaceVertex;
}


#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec4 EyeSpaceVertex;

struct Material {
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
};

// nadefinovat svìtlo
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform struct FogParameters
{
    vec4 vFogColor; // Fog color
    float fDensity; // For exp and exp2 equation
} fogParams;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

float getFogFactor(FogParameters params, float fFogCoord)
{
    float fResult = 0.0;
    fResult = exp(-pow(params.fDensity * fFogCoord, 2.0));
    fResult = 1.0 - clamp(fResult, 0.0, 1.0);
    return fResult;
}

void main()
{
    // ambient
    /*vec3 ambient = light.ambient * material.ambient;

    // diffuse 
    vec3 norm = Normal;
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);

    float FogCoord = abs(vEyeSpacePos.z / vEyeSpacePos.w);
    FragColor = mix(FragColor, fogParams.vFogColor, getFogFactor(fogParams, fFogCoord));*/
    FragColor = vec4(1.0, 0.0, 0.0, 0.0);
}