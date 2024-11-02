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

// Vertex data: position (X, Y, Z), normal vector (NX, NY, NZ), texture coordinates (U, V)
float vertices[] = {
    // Front face
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  0.0f, 0.0f, // bottom-left
     0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  1.0f, 1.0f, // top-right
    -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  0.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  0.0f, 0.0f, // bottom-left
    // Back face
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,  1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   0.0f, 0.0f,  1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f, 0.0f,  1.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   0.0f, 0.0f,  1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,  1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,  1.0f,  0.0f, 0.0f,
    // Left face
    -0.5f,  0.5f,  0.5f,   -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
    // Right face
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
     // Bottom face
    -0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
    // Top face
   -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
};

// Time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Cube data
const int NUM_CUBES = 2;
glm::vec3 cubePositions[NUM_CUBES] = {
    glm::vec3(1.0f, 1.0f, -2.0f), // Position for light emitter cube
    glm::vec3(0.0f, 0.0f, -2.0f), // Position for regular cube
};

// Light source settings
glm::vec3 lightPos = cubePositions[0];

int main() {
    std::cout << "Initializing...\n";

    srand(static_cast<unsigned int>(time(nullptr)));


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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal vectors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Load textures
    Texture2D cubeTexture("assets/udq9asephmpy-2137226915.png.", GL_LINEAR, GL_CLAMP_TO_EDGE);
    Shader mainShader("assets/mainVertexShader.vert", "assets/mainFragmentShader.frag");
    Shader lightShader("assets/lightVertexShader.vert", "assets/lightFragmentShader.frag");

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

        // Input and rendering setup
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.ProcessInput(deltaTime);

        // Set camera and projection matrices
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix(static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT));

        // Render the light emitter cube
        lightShader.use();
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);

        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        lightShader.setMat4("model", lightModel);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Render the main cube with texture and lighting
        mainShader.use();
        mainShader.setMat4("view", view);
        mainShader.setMat4("projection", projection);
        mainShader.setVec3("lightPos", lightPos);
        mainShader.setVec3("viewPos", camera.GetPosition());

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[1]);
        mainShader.setMat4("model", model);

        cubeTexture.Bind(0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    std::cout << "Shutting down...\n";
    glfwTerminate();
    return 0;
}
