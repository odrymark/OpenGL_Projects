#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"

float vertices[] = {
    -0.5, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
    0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Study", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD";
        return -1;
    }

    glViewport(0, 0, 800, 600);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    Shader shader("/home/odrymark/CLionProjects/first/shader_src/vertex", "/home/odrymark/CLionProjects/first/shader_src/fragment");

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(1.f, 1.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);

        float timeValue = glfwGetTime();
        float redValue = (std::sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLoc = glGetUniformLocation(shader.ID, "color");
        glUniform4f(vertexColorLoc, redValue, 0.0f, 0.0f, 1.0f);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}