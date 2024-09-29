#include <stdio.h>
#include <math.h>
#include <ew/external/glad.h>
#include <GLFW/glfw3.h>
#include <ew/ewMath/ewMath.h>
#include <glm/glm.hpp>
#include <drew/Shader.h>
#include <drew/Texture2D.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[] = {
//    X     Y     Z       R     G     B       U     V
    -1.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,  // TOP LEFT
     1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  // TOP RIGHT
    -1.0f,-1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // BOTTOM LEFT
     1.0f,-1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f   // BOTTOM RIGHT
};

unsigned int indices[] = {
    0, 1, 2,
    1, 3, 2
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

    Texture2D spriteTexture("assets/pixil-frame-0.png", GL_NEAREST, GL_CLAMP_TO_EDGE);
    Texture2D backgroundTexture("assets/bottleshark-1371155747.png", GL_NEAREST, GL_REPEAT);

    Shader backgroundShader("assets/backgroundVertexShader.vert", "assets/backgroundFragmentShader.frag");
    Shader spriteShader("assets/spriteVertexShader.vert", "assets/spriteFragmentShader.frag");
    


    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); 

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        backgroundTexture.Bind();
        backgroundShader.use();
        float time = glfwGetTime();
        backgroundShader.setFloat("_time", time);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

       
        spriteShader.use();
        spriteShader.setFloat("_time", time);
        spriteTexture.Bind();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    printf("Shutting down...\n");
    glfwTerminate();
    return 0;
}
