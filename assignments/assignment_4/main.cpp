#include <stdio.h>
#include <math.h>
#include <ew/external/glad.h>
#include <GLFW/glfw3.h>
#include <ew/ewMath/ewMath.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <drew/Shader.h>
#include <drew/Texture2D.h>
#include <drew/Transform.h>
#include <drew/Camera.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;


// Vertex data: position (X, Y, Z), texture coordinates (U, V)
float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

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
        cubePositions[i] = glm::vec3(ew::RandomRange(-4.0f, 4.0f), ew::RandomRange(-5.0f, 5.0f), ew::RandomRange(-15.0f, 0.0f));
        std::cout << "Cube " << i << " Position: " << cubePositions[i].x << ", " << cubePositions[i].y << ", " << cubePositions[i].z << std::endl;
        cubeRotations[i] = glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f));
    }
}

int main() {
    printf("Initializing...\n");

    srand(static_cast<unsigned int>(time(nullptr)));
    CreateCubes();

    // Initialize GLFW
    if (!glfwInit()) {
        printf("GLFW failed to init!\n");
        return 1;
    }

    // Create window
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Sprite", NULL, NULL);
    if (window == NULL) {
        printf("GLFW failed to create window\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // Load GLAD
    if (!gladLoadGL(glfwGetProcAddress)) {
        printf("GLAD failed to load GL headers\n");
        glfwTerminate();
        return 1;
    }

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // UV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Load textures
   // unsigned int cubeTexture, baackgroundTexture;

    Texture2D cubeTexture("assets/MissingTextureInventory-3627783748.png", GL_LINEAR, GL_CLAMP_TO_EDGE);

    // Load shaders
   /* Shader backgroundShader("assets/backgroundVertexShader.vert", "assets/backgroundFragmentShader.frag");
    Shader spriteShader("assets/spriteVertexShader.vert", "assets/spriteFragmentShader.frag");*/
    Shader mainShader("assets/mainVertexShader.vert", "assets/mainFragmentShader.frag");

    // Create camera object
    Camera camera(window);

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

    // Enable blending for transparent textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    glEnable(GL_DEPTH_TEST);

    // Create transform object
    Transform transform;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
      

        mainShader.use();

        camera.ProcessInput(deltaTime);

        // Set camera and transformations
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
        mainShader.setMat4("view", view);
        mainShader.setMat4("projection", projection);
        mainShader.setMat4("model", model);

        float time = glfwGetTime();
        mainShader.setFloat("_time", time);

        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < NUM_CUBES; i++)
        {
            cubeRotations[i].x += 30.0f * deltaTime;
            cubeRotations[i].y -= 30.0f * deltaTime;
            cubeRotations[i].z += 30.0f * deltaTime;
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(cubeRotations[i].x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(cubeRotations[i].y), glm::vec3(0.0f, 1.0f, 0.0f)); 
            model = glm::rotate(model, glm::radians(cubeRotations[i].z), glm::vec3(0.0f, 0.0f, 1.0f));
            mainShader.setMat4("model", model);
            cubeTexture.Bind(0);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
    }


    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    printf("Shutting down...\n");
    glfwTerminate();
    return 0;
}
