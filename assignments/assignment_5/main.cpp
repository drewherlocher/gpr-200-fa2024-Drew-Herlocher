#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <ew/external/glad.h>
#include <GLFW/glfw3.h>
#include <ew/ewMath/ewMath.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <drew/Shader.h>
#include <drew/Texture2D.h>
#include <drew/Camera.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

// Vertex data: position (X, Y, Z), texture coordinates (U, V)
float vertices[] = {
    // Front face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    // Back face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     // Bottom face
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     // Top face
     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

// Time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Cube data
const int NUM_CUBES = 25;
std::vector<glm::vec3> cubePositions(NUM_CUBES);
std::vector<glm::vec3> cubeRotations(NUM_CUBES);
std::vector<glm::vec3> cubeScales(NUM_CUBES);

// Cube creator
void CreateCubes() {
    for (int i = 0; i < NUM_CUBES; ++i) {
        cubePositions[i] = glm::vec3(ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f), ew::RandomRange(-10.0f, 10.0f));
        cubeRotations[i] = glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f));
        cubeScales[i] = glm::vec3(ew::RandomRange(0.3f, 3.0f));
    }
}

int main() {
    std::cout << "Initializing...\n";

    srand(static_cast<unsigned int>(time(nullptr)));
    CreateCubes();

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "GLFW failed to initialize!\n";
        return 1;
    }

    // Create window
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cubes with Camera Control", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "GLFW failed to create window\n";
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // Load GLAD
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "GLAD failed to load GL headers\n";
        glfwTerminate();
        return 1;
    }

    // Set up vertex array object (VAO) and vertex buffer object (VBO)
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // UV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load textures
    Texture2D cubeTexture("assets/MissingTextureInventory-3627783748.png", GL_LINEAR, GL_CLAMP_TO_EDGE);
    Shader mainShader("assets/mainVertexShader.vert", "assets/mainFragmentShader.frag");

    // Create camera object
    Camera camera(window);

    // Enable blending for transparent textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mainShader.use();
        camera.ProcessInput(deltaTime);

        // Set camera and transformations
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix(static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT));
        mainShader.setMat4("view", view);
        mainShader.setMat4("projection", projection);

        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < NUM_CUBES; i++) {
            cubeRotations[i] += glm::vec3(30.0f * deltaTime, -30.0f * deltaTime, 30.0f * deltaTime);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(cubeRotations[i].x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(cubeRotations[i].y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(cubeRotations[i].z), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, cubeScales[i]);
            mainShader.setMat4("model", model);
            cubeTexture.Bind(0);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    std::cout << "Shutting down...\n";
    glfwTerminate();
    return 0;
}
