#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec3 FragPos;
out vec4 EyeSpace;
out vec2 TexCoords;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat3 NormalizationMatrix;


void main()
{
    EyeSpace = View * Model * vec4(aPos, 1.0);
    Normal = NormalizationMatrix * aNormal;
    FragPos = vec3(Model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
    gl_Position = Projection * EyeSpace;
}


#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec4 EyeSpace;
in vec2 TexCoords;

struct Material {
  // vec3 ambient;
  // vec3 diffuse;
    vec3 specular;
    float shininess;
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

struct FogParameters
{
    vec4 fogColor; // Fog color
    float fDensity; // For exp and exp2 equation
};

uniform Material material;

uniform DirLight dirLight;
uniform PointLight pointLights[2];
uniform SpotLight flashLight;
uniform bool useFlashLight;
uniform vec3 viewPos;

uniform FogParameters fogParam;

float getFogFactor(FogParameters params, float fFogCoord)
{
    float fResult = 0.0;
    fResult = exp(-pow(params.fDensity * fFogCoord, 2.0));
    fResult = 1.0 - clamp(fResult, 0.0, 1.0);
    return fResult;
}

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;


vec3 getDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // get light direction
    vec3 lightDir = normalize(-light.direction);
    // diffuse part
    float diff = max(dot(normal, lightDir), 0.0);
    // specular part
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // assign results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 getPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse part
    float diff = max(dot(normal, lightDir), 0.0);
    // specular part
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // assign results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 getSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse part
    float diff = max(dot(normal, lightDir), 0.0);
    // specular part
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // flashlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // assign results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * material.specular;
    
    //ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main()
{
    /*// ambient
    vec3 ambient = light.ambient * material.ambient;

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
    FragColor = vec4(result, 1.0);*/

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = getDirLight(dirLight, Normal, viewDir);

    // lights calc
    for (int i = 0; i < 2; i++)
        result += getPointLight(pointLights[i], Normal, FragPos, viewDir);

    if (useFlashLight) {
        result += getSpotLight(flashLight, Normal, FragPos, viewDir);
    }

     FragColor = vec4(result, 1.0);

     float FogCoord = abs(EyeSpace.z / EyeSpace.w);
     FragColor = mix(FragColor, fogParam.fogColor, getFogFactor(fogParam, FogCoord));
}