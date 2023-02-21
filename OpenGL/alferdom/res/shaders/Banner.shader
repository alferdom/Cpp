#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
uniform float time;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    float currTime = time * 0.05;
    vec2 move = vec2((floor(currTime) - currTime) * 4 + 1.0, 0.0);

    TexCoords = aTexCoords + move;
    gl_Position = Projection * View * Model * vec4(aPos, 1.0);
}


#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoords);
}