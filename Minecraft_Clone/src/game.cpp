#include "game.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

Game::Game()
    : world(64, 8, 64),
      camera(glm::vec3(32.f, 8.f + PLAYER_HEIGHT, 32.f))
{
    physics.setGame(this);
}

void Game::setShader(Shader shaderProg) {
    shader = shaderProg;
}

void Game::setTexture(const unsigned int tex[3])
{
    texture[0] = tex[0];
    texture[1] = tex[1];
    texture[2] = tex[2];
}

void Game::run(GLFWwindow *window) {
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(1.f, 1.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto currFrame = static_cast<float>(glfwGetTime());
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;
        physics.breakTimer += deltaTime;
        physics.placeTimer += deltaTime;

        physics.targetBlock(camera.position, camera.front);
        processInput(window);
        camera.position.y += physics.applyGravity(camera.position, PLAYER_RADIUS, PLAYER_HEIGHT, deltaTime);

        shader.use();
        glm::mat4 view = camera.getViewMatrix();
        shader.setMat4("view", view);

        for (int x = 0; x < world.WORLD_X; x++) {
            for (int y = 0; y < world.WORLD_Y; y++) {
                for (int z = 0; z < world.WORLD_Z; z++) {
                    if(!world.isBlockSolid(x, y, z))
                        continue;

                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(x, y, z));
                    shader.setMat4("model", model);

                    renderBlock(x, y, z);
                }
            }
        }

        if(physics.hasTarget)
        {
            glEnable(GL_POLYGON_OFFSET_LINE);
            glPolygonOffset(-1.0f, -1.0f);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(3.0f);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(physics.targetedBlock));
            model = glm::scale(model, glm::vec3(1.01f));
            shader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDisable(GL_POLYGON_OFFSET_LINE);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Game::renderBlock(int x, int y, int z) const
{
    if (y == world.WORLD_Y - 1)
    {
        glBindTexture(GL_TEXTURE_2D, texture[2]);

        if (x == 0 || !world.isBlockSolid(x-1, y, z))
            glDrawArrays(GL_TRIANGLES, 12, 6);
        if (x == world.WORLD_X-1 || !world.isBlockSolid(x+1, y, z))
            glDrawArrays(GL_TRIANGLES, 18, 6);
        if (z == 0 || !world.isBlockSolid(x, y, z-1))
            glDrawArrays(GL_TRIANGLES, 0, 6);
        if (z == world.WORLD_Z-1 || !world.isBlockSolid(x, y, z+1))
            glDrawArrays(GL_TRIANGLES, 6, 6);

        if (!world.isBlockSolid(x, y-1, z)) {
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            glDrawArrays(GL_TRIANGLES, 24, 6);
        }

        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glDrawArrays(GL_TRIANGLES, 30, 6);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, texture[0]);

        if (x == 0 || !world.isBlockSolid(x-1, y, z))
            glDrawArrays(GL_TRIANGLES, 12, 6);

        if (x == world.WORLD_X-1 || !world.isBlockSolid(x+1, y, z))
            glDrawArrays(GL_TRIANGLES, 18, 6);

        if (y == 0 || !world.isBlockSolid(x, y-1, z))
            glDrawArrays(GL_TRIANGLES, 24, 6);

        if (!world.isBlockSolid(x, y+1, z))
            glDrawArrays(GL_TRIANGLES, 30, 6);

        if (z == 0 || !world.isBlockSolid(x, y, z-1))
            glDrawArrays(GL_TRIANGLES, 0, 6);

        if (z == world.WORLD_Z-1 || !world.isBlockSolid(x, y, z+1))
            glDrawArrays(GL_TRIANGLES, 6, 6);
    }
}

void Game::processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        physics.breakBlock();

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        physics.placeBlock(camera.position, PLAYER_HEIGHT, PLAYER_RADIUS);


    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        physics.jump();
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

    if(physics.checkCollision(PLAYER_HEIGHT, PLAYER_RADIUS, glm::vec3(camera.position.x, lastPos.y, lastPos.z)))
        camera.position.x = lastPos.x;
    if(physics.checkCollision(PLAYER_HEIGHT, PLAYER_RADIUS, glm::vec3(lastPos.x, lastPos.y, camera.position.z)))
        camera.position.z = lastPos.z;
}