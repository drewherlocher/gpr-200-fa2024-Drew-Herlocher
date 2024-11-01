#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexPos;

out vec2 TexPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float _time;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexPos = aTexPos;
}
