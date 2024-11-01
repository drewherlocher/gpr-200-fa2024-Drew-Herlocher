#version 330 core

in vec2 texPos;
out vec4 FragColor;

uniform sampler2D background;

void main() {
    //vec2 tiledTexPos = fract(texPos);
    //FragColor = texture(background, tiledTexPos);
}
