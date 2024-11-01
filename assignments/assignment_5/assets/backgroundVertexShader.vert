#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexPos;

uniform float _time;

out vec2 texPos;

void main() {
   // gl_Position = vec4(aPos, 1.0);
    //texPos = aTexPos + vec2(mod(_time * 0.1, 1.0), mod(_time * 0.1, 1.0));
    }
