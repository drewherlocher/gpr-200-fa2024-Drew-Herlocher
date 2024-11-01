#version 330 core

in vec3 color;
in vec2 texPos;

out vec4 FragColor;

uniform float _time;
uniform sampler2D sprite;

void main() {
    vec4 texColor = texture(sprite, texPos);
    
    FragColor = texColor;
}
