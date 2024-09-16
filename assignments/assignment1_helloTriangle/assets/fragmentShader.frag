#version 330 core

in vec3 color;
out vec4 FragColor;

uniform float _time;

void main() {

    vec3 newColor = vec3(cos(_time * 1.0), sin(_time * 3.0), sin(_time));
    
    vec3 finalColor = mix(color, newColor, 0.2);

    FragColor = vec4(finalColor, 1.0);
}
