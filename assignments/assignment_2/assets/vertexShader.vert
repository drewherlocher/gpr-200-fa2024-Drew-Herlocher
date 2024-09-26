#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float _time;

out vec3 color;

void main() {
    vec3 newPos = aPos;
    newPos.x += cos(_time + aPos.x) * 0.5;
    newPos.y += sin(_time + aPos.y) * 0.5;
    gl_Position = vec4(newPos, 1.0);
    color = aColor;
}