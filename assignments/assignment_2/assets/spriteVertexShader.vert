#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexPos;

uniform float _time;


out vec3 color;
out vec2 texPos;

void main() {
    vec3 newPos = aPos;
    newPos.x = cos(_time + aPos.x) * 0.5;
    gl_Position = vec4(newPos, 1.0);
    color = aColor;
    texPos = aTexPos;
}