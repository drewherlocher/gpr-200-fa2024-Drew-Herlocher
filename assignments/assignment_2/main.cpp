#include <stdio.h>
#include <math.h>
#include <ew/external/glad.h>
#include <GLFW/glfw3.h>
#include <ew/ewMath/ewMath.h>
#include <glm/glm.hpp>
#include <drew/Shader.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[] = {
    //   X      Y     Z      R     G     B
        -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // TOP LEFT
        0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f, // TOP RIGHT
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, // BOTTOM LEFT
        0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, // BOTTOM RIGHT
};

unsigned int indices[] = {
    0, 1, 2,
    1, 2, 3
};

int main() {
    printf("Initializing...\n");
    if (!glfwInit()) {
        printf("GLFW failed to init!\n");
        return 1;
    }
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
    if (window == NULL) {
        printf("GLFW failed to create window\n");
        return 1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGL(glfwGetProcAddress)) {
        printf("GLAD Failed to load GL headers\n");
        return 1;
    }

    Shader shaderProgram("assets/vertexShader.vert", "assets/fragmentShader.frag");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); 


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();
        
        float time = glfwGetTime();
        shaderProgram.setFloat("_time", time);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    printf("Shutting down...\n");
    glfwTerminate();
    return 0;
}
