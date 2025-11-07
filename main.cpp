#include <iostream>
#include <cmath>          // for sqrt()
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

// Vertex data
GLfloat vertices[] =
{   //       COORDINATES                 COLORS
    -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,   0.8f, 0.3f, 0.02f,   // Lower left corner
     0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,   0.8f, 0.3f, 0.02f,   // Lower right corner
     0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,   1.0f, 0.6f, 0.32f,   // Upper corner
    -0.25f, 0.5f * float(sqrt(3)) / 6, 0.0f,   0.9f, 0.45f, 0.17f,   // Inner left
     0.25f, 0.5f * float(sqrt(3)) / 6, 0.0f,   0.9f, 0.45f, 0.17f,   // Inner right
     0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f,   0.8f, 0.3f, 0.02f    // Inner down
};

// Index data (optional for simple triangles)
GLuint indices[] = {
    0, 1, 2
};
int main(void)
{
    /* Initialize GLFW */
    if (!glfwInit())
        return -1;

    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a window */
    GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Triangle", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    /* Load GLAD (must happen after context creation) */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // Set viewport
    glViewport(0, 0, 800, 800);

    Shader shaderProgram("default.vert", "default.frag");
    
    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
    /* Render loop */
    while (!glfwWindowShouldClose(window))
    {
        // Background color
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw triangle
        shaderProgram.Activate();
        glUniform1f(uniID, 0.5f);
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
