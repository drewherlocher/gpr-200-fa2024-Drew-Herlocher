#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D cubeTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform float ambientK;
uniform float diffuseK;
uniform float specularK;
uniform float shininess;

void main() {
    vec4 color = texture(cubeTexture, TexCoords);

    vec3 ambient = ambientK * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDirection), 0.0); 
    vec3 diffuse = diffuseK * diff * lightColor;

    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 halfVector = normalize(lightDirection + viewDirection);

    float spec = pow(max(dot(norm, halfVector), 0.0), shininess);
    vec3 specular = specularK * spec * lightColor;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result * color.rgb, 1.0);
}
