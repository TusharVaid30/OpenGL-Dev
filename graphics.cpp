#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

static const int _WIDTH = 1920;
static const int _HEIGHT = 1080;
static const char* _TITLE = "Game";

Shader* shaderProgram;

VAO* vao1;

GLuint scale;

GLuint texture;

Texture* blend;

float x = 0, y = 0;

void GLFWHints()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void CreateTriangle(float test)
{
    GLfloat vertices[] =
    {
       -0.5f, 0.0f,  0.5f,       /*0.5f, 0.3f, 0.9f,*/       0.0f, 0.0f,
       -0.5f, 0.0f, -0.5f,       /*0.5f, 0.3f, 0.0f,*/       1.0f, 0.0f,
        0.5f, 0.0f, -0.5f,       /*0.5f, 0.0f, 0.9f,*/       1.0f, 1.0f,
        0.5f, 0.0f,  0.5f,       /*0.0f, 0.3f, 0.9f,*/       0.0f, 1.0f
        
    };

    GLuint indices[] =
    {
        0, 1, 2,
        0, 2, 3,
    };
 
    shaderProgram = new Shader("default.vert", "default.frag");

    vao1 = new VAO();

    vao1->Bind();

    VBO vbo1(vertices, sizeof(vertices));
    EBO ebo1(indices, sizeof(indices));

    vao1->LinkAttrib(vbo1, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    //vao1->LinkAttrib(vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao1->LinkAttrib(vbo1, 2, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    vao1->Unbind();
    vbo1.Unbind();
    ebo1.Unbind();

    blend = new Texture("1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    blend->texUnit(*shaderProgram, "tex0", 0);
}

GLFWwindow* CreateWindow(int WIDTH, int HEIGHT, const char* TITLE)
{
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
    if (window == NULL) return NULL;

    glfwMakeContextCurrent(window);

    return window;
}

void SetWindowColor(GLfloat r, GLfloat g, GLfloat b, GLFWwindow* window)
{
    gladLoadGL();

    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
}

int main(void)
{
    glfwInit();

    GLFWHints();

    GLFWwindow* window = CreateWindow(800, 800, _TITLE);

    //SetWindowColor(0.3f, 0.4f, 0.2f, window);
    gladLoadGL();

    glEnable(GL_DEPTH_TEST);

    float rot = 0.0f;

    Camera camera(800, 800, glm::vec3(0.0f, 0.0f, 10.0f));

    CreateTriangle(1.0f);
    
    while (!glfwWindowShouldClose(window))
    {
        rot += .05f;
        glViewport(0, 0, 800, 800);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderProgram->Activate();

        camera.Inputs(window);
        camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

        blend->Bind();
        vao1->Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vao1->Delete();

    shaderProgram->Delete();

    glfwDestroyWindow(window);
    
    glfwTerminate();
    return 0;
}