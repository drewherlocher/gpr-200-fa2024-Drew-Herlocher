#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexPos;

out vec2 TexPos;

uniform mat4 projection; // Projection matrix
uniform mat4 view;       // View matrix
uniform mat4 model;      // Matrix for transformations
uniform float _time;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexPos = aTexPos;
}
