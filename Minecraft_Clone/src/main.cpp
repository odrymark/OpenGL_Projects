#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "shader.h"
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void processInput(GLFWwindow *window);
int getIndex(int x, int y, int z);
void renderBlock(int x, int y, int z);
void applyGravity();
bool checkGrounded();
bool checkCollision();
void targetBlock();

float winWidth = 1920.f;
float winHeight = 1080.f;

const int WORLD_X = 64;
const int WORLD_Y = 8;
const int WORLD_Z = 64;
const float PLAYER_HEIGHT = 0.75f;
const float PLAYER_RADIUS = 0.2f;

std::vector<int> blocks(WORLD_X * WORLD_Y * WORLD_Z);

const float MAX_RAY_DIST = 8.f;
const float RAY_STEP = 0.1f;
const float BREAK_COOLDOWN = 0.5f;
float breakTimer = BREAK_COOLDOWN;
glm::ivec3 targetedBlock(-1);
bool hasTarget = false;

Camera camera(glm::vec3(WORLD_X / 2, WORLD_Y + PLAYER_HEIGHT, WORLD_Z / 2));

float deltaTime = 0.f;
float lastFrame = 0.f;

float lastX = winWidth / 2.f;
float lastY = winHeight / 2.f;
bool isFirstMouse = true;

float yVelocity = 0.f;
const float GRAVITY = -10.f;
const float TERMINAL_VELOCITY = -20.f;
const float JUMP_FORCE = 5.5f;
bool isGrounded = false;

unsigned int texture[3];

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

     0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(static_cast<int>(winWidth), static_cast<int>(winHeight), "Minecraft Clone", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, static_cast<int>(winWidth), static_cast<int>(winHeight));

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    int img_w[3], img_h[3], nrChannels[3];
    unsigned char *data[3];
    data[0] = stbi_load("../images/dirt.jpg", &img_w[0], &img_h[0], &nrChannels[0], 0);
    data[1] = stbi_load("../images/grass.jpg", &img_w[1], &img_h[1], &nrChannels[1], 0);
    data[2] = stbi_load("../images/grass_side.jpg", &img_w[2], &img_h[2], &nrChannels[2], 0);

    glGenTextures(3, texture);
    glActiveTexture(GL_TEXTURE0);

    for(int i = 0; i < 3; i++)
    {
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_w[i], img_h[i], 0, GL_RGB, GL_UNSIGNED_BYTE, data[i]);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data[i]);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader shader("../src_shader/vertex", "../src_shader/fragment");
    shader.use();
    shader.setInt("tex", 0);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(60.f), winWidth/winHeight, 0.01f, 100.f);

    shader.setMat4("projection", projection);

    for (int x = 0; x < WORLD_X; x++)
        for (int y = 0; y < WORLD_Y; y++)
            for (int z = 0; z < WORLD_Z; z++)
                if(y < 1)
                    blocks[getIndex(x, y, z)] = 0;
                else
                    blocks[getIndex(x,y,z)] = 1;

    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(1.f, 1.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto currFrame = static_cast<float>(glfwGetTime());
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;
        breakTimer += deltaTime;

        targetBlock();
        processInput(window);
        applyGravity();

        shader.use();
        glBindVertexArray(VAO);
        glm::mat4 view = camera.getViewMatrix();
        shader.setMat4("view", view);

        for (int x = 0; x < WORLD_X; x++) {
            for (int y = 0; y < WORLD_Y; y++) {
                for (int z = 0; z < WORLD_Z; z++) {
                    if(blocks[getIndex(x, y, z)] == 0)
                        continue;

                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(x, y, z));
                    shader.setMat4("model", model);

                    renderBlock(x, y, z);
                }
            }
        }

        if(hasTarget)
        {
            glEnable(GL_POLYGON_OFFSET_LINE);
            glPolygonOffset(-1.0f, -1.0f);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(3.0f);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(targetedBlock));
            model = glm::scale(model, glm::vec3(1.01f));
            shader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDisable(GL_POLYGON_OFFSET_LINE);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();

    return 0;
}

void renderBlock(int x, int y, int z)
{
    if (y == WORLD_Y - 1)
    {
        glBindTexture(GL_TEXTURE_2D, texture[2]);

        if (x == 0 || blocks[getIndex(x-1,y,z)] == 0) glDrawArrays(GL_TRIANGLES, 12, 6);
        if (x == WORLD_X-1 || blocks[getIndex(x+1,y,z)] == 0) glDrawArrays(GL_TRIANGLES, 18, 6);
        if (z == 0 || blocks[getIndex(x,y,z-1)] == 0) glDrawArrays(GL_TRIANGLES, 0, 6);
        if (z == WORLD_Z-1 || blocks[getIndex(x,y,z+1)] == 0) glDrawArrays(GL_TRIANGLES, 6, 6);

        if (blocks[getIndex(x,y-1,z)] == 0) {
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            glDrawArrays(GL_TRIANGLES, 24, 6);
        }

        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glDrawArrays(GL_TRIANGLES, 30, 6);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, texture[0]);

        if (x == 0 || blocks[getIndex(x-1,y,z)] == 0)
            glDrawArrays(GL_TRIANGLES, 12, 6);

        if (x == WORLD_X-1 || blocks[getIndex(x+1,y,z)] == 0)
            glDrawArrays(GL_TRIANGLES, 18, 6);

        if (y == 0 || blocks[getIndex(x,y-1,z)] == 0)
            glDrawArrays(GL_TRIANGLES, 24, 6);

        if (blocks[getIndex(x,y+1,z)] == 0)
            glDrawArrays(GL_TRIANGLES, 30, 6);

        if (z == 0 || blocks[getIndex(x,y,z-1)] == 0)
            glDrawArrays(GL_TRIANGLES, 0, 6);

        if (z == WORLD_Z-1 || blocks[getIndex(x,y,z+1)] == 0)
            glDrawArrays(GL_TRIANGLES, 6, 6);
    }
}

void applyGravity()
{
    if(checkGrounded())
    {
        isGrounded = true;
        yVelocity = 0.f;

        camera.position.y = floor(camera.position.y - PLAYER_HEIGHT) + 1.f + PLAYER_HEIGHT;
    }
    else
        isGrounded = false;

    if(!isGrounded)
    {
        yVelocity += GRAVITY * deltaTime;

        if(yVelocity < TERMINAL_VELOCITY)
            yVelocity = TERMINAL_VELOCITY;
    }

    camera.position.y += yVelocity * deltaTime;
}

bool checkGrounded()
{
    int x = static_cast<int>(round(camera.position.x));
    int y = static_cast<int>(floor(camera.position.y - PLAYER_HEIGHT));
    int z = static_cast<int>(round(camera.position.z));

    if (x < 0 || x >= WORLD_X ||
        y < 0 || y >= WORLD_Y ||
        z < 0 || z >= WORLD_Z)
        return false;

    return blocks[getIndex(x, y, z)] != 0;
}

bool checkCollision()
{
    int minX = static_cast<int>(round(camera.position.x - PLAYER_RADIUS));
    int maxX = static_cast<int>(round(camera.position.x + PLAYER_RADIUS));
    int minY = static_cast<int>(round(camera.position.y - PLAYER_HEIGHT));
    int maxY = static_cast<int>(round(camera.position.y));
    int minZ = static_cast<int>(round(camera.position.z - PLAYER_RADIUS));
    int maxZ = static_cast<int>(round(camera.position.z + PLAYER_RADIUS));

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            for (int z = minZ; z <= maxZ; z++)
            {
                if (x < 0 || x >= WORLD_X ||
                    y < 0 || y >= WORLD_Y ||
                    z < 0 || z >= WORLD_Z)
                    continue;

                if (blocks[getIndex(x, y, z)] != 0)
                    return true;
            }
        }
    }

    return false;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xPos, double yPos)
{
    auto x = static_cast<float>(xPos);
    auto y = static_cast<float>(yPos);

    if(isFirstMouse)
    {
        lastX = x;
        lastY = y;
        isFirstMouse = false;
    }

    float xOffset = x - lastX;
    float yOffset = lastY - y;

    lastX = x;
    lastY = y;

    camera.processMouseInput(xOffset, yOffset);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && breakTimer >= BREAK_COOLDOWN)
    {
        blocks[getIndex(targetedBlock.x, targetedBlock.y, targetedBlock.z)] = 0;
        breakTimer = 0.f;
    }


    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && isGrounded)
    {
        yVelocity = JUMP_FORCE;
        isGrounded = false;
    }

    glm::vec3 lastPos = camera.position;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyInput(FORWARD, deltaTime, false);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyInput(BACKWARD, deltaTime, false);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyInput(RIGHT, deltaTime, false);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyInput(LEFT, deltaTime, false);

    if(checkCollision())
        camera.position = lastPos;
}

int getIndex(int x, int y, int z)
{
    return x + y * WORLD_X + z * WORLD_X * WORLD_Y;
}

void targetBlock()
{
    hasTarget = false;

    glm::vec3 rayOrigin = camera.position + glm::vec3(0.f, -0.3f, 0.f);
    glm::vec3 rayDirection = glm::normalize(camera.front);

    for(float d = 0; d < MAX_RAY_DIST; d += RAY_STEP)
    {
        glm::vec3 pos = rayOrigin + rayDirection * d;

        int x = static_cast<int>(round(pos.x));
        int y = static_cast<int>(round(pos.y));
        int z = static_cast<int>(round(pos.z));

        if(x < 0 || x > WORLD_X || y < 0 || y > WORLD_Y || z < 0 || z > WORLD_Z)
            break;

        if(blocks[getIndex(x, y, z)] != 0)
        {
            targetedBlock = glm::ivec3(x,y,z);
            hasTarget = true;
            return;
        }
    }
}
