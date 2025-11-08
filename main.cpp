#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"

const unsigned int width = 800;
const unsigned int height = 800;

// Cube vertex data
GLfloat vertices[] = {
    // Positions           // Colors           // Texture Coords
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // 0
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // 1
     0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // 2
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, // 3

    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,   0.0f, 0.0f, // 4
     0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f, // 5
     0.5f,  0.5f,  0.5f,   1.0f, 0.5f, 0.2f,   1.0f, 1.0f, // 6
    -0.5f,  0.5f,  0.5f,   0.2f, 0.8f, 0.4f,   0.0f, 1.0f  // 7
};

// Cube indices (6 faces × 2 triangles)
GLuint indices[] = {
    0, 1, 2,  2, 3, 0,  // back
    4, 5, 6,  6, 7, 4,  // front
    4, 5, 1,  1, 0, 4,  // bottom
    7, 6, 2,  2, 3, 7,  // top
    4, 7, 3,  3, 0, 4,  // left
    5, 6, 2,  2, 1, 5   // right
};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Textured Cube", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, width, height);

    Shader shaderProgram("default.vert", "default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    // Texture
    Texture blockTex("d.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    blockTex.texUnit(shaderProgram, "tex0", 0);

    glEnable(GL_DEPTH_TEST);

    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.Activate();

        double crntTime = glfwGetTime();
        if (crntTime - prevTime >= 1 / 60)
        {
            rotation += 0.15f;
            prevTime = crntTime;
        }

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.5f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        proj = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

        glUniform1f(uniID, 1.0f);

        blockTex.Bind();
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    blockTex.Delete();
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
