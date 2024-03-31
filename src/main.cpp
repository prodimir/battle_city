#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/ShaderProgram.h" 
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"


/*Задаём vertex (координаты треугольника с оригинальной точкой в центре треугольника)*/
GLfloat point[] = {
    0.0f, 50.f, 0.0f,
    50.f, -50.f, 0.0f,
    -50.f, -50.f, 0.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

// Координаты текстур для каждого вертекса. Две координаты
GLfloat texCoord[] = {
    0.5f, 1.0f, // верхняя точка треугольника
    1.0f, 0.0f, // внизу справа
    0.0f, 0.0f
};

const char* vertex_shader =
"#version 450\n"
"layout(location = 0) in vec3 vertex_position;"
"layout(location = 1) in vec3 vertex_color;"
"out vec3 color;"
"void main() {"
"   color = vertex_color;"
"   gl_Position = vec4(vertex_position, 1.0);"
"}";

const char* fragment_shader = 
"#version 450\n"
"in vec3 color;"
"out vec4 frag_color;"
"void main() {"
"   frag_color = vec4(color, 1.0);"
"}";


glm::ivec2 g_windowSize(640, 480);

//int g_windowSizeX = 640; // вместо этих переменных мы создали двухмерный вектор с помощью библиотеки glm
//int g_windowSizeY = 480;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSize.x = width;
    g_windowSize.y = height;
    glViewport(0, 0, g_windowSize.x, g_windowSize.y); // точка, откуда начинаем рисовать и на какую площадь
}

void glfwKeyCallback(GLFWwindow* pwindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pwindow, GL_TRUE);
    }
}

int main(int argc, char** argv)
{
    

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "glfwInit failed!" << std::endl;
        return -1;
    }
    
    /*Specifi version of OpenGL*/

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5 );
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pwindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "Battle_City", NULL, NULL);
    if (!pwindow)
    {
        std::cout << "GLFWWindow failed" << std::endl;
        glfwTerminate();
        return -1;
    }


    glfwSetWindowSizeCallback(pwindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pwindow, glfwKeyCallback);


    /* Make the window's context current */
    glfwMakeContextCurrent(pwindow);

    if (!gladLoadGL())
    {
        std::cout << "Can't load GLAD" << std::endl;
        return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;


    glClearColor(1,0,1,1);



    { 
        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram)
        {
            std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
            return -1;
        }

        auto pSpriteShaderProgram = resourceManager.loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
        if (!pDefaultShaderProgram)
        {
            std::cerr << "Can't create shader program: " << "SpriteShader" << std::endl;
            return -1;
        }

        auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");
        ////call vertex_shader+compile

        //GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        //glShaderSource(vs, 1, &vertex_shader, nullptr);
        //glCompileShader(vs);

        ////call fragment_shader+compile

        //GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        //glShaderSource(fs, 1, &fragment_shader, nullptr);
        //glCompileShader(fs);

        //// Link vertex_shader and fragmaent_shader

        //GLuint shader_program = glCreateProgram();
        //glAttachShader(shader_program, vs);
        //glAttachShader(shader_program, fs);
        //glLinkProgram(shader_program);

        //glDeleteShader(vs);
        //glDeleteShader(fs);

        // Add data from buffers of point and colors in memory of video card

        auto pSprite = resourceManager.loadSprite("NewSprite", "DefaultTexture", "SpriteShader", 50, 100);
        pSprite->setPosition(glm::vec2(300, 100));

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);               // create buffer points_vbo where put data from buffer point[]
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo); // connect buffer and make it !current!
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW); // specify with what buffer we use and it size. Work only with !current! buffer

        // similarly for buffer colors[]

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        //for buffer texCoord
        GLuint texCoord_vbo = 0;
        glGenBuffers(1, &texCoord_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

        // specify what video card has to do with transmitted data

        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // for coordinates of textures 
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        // делаем шейдер активным, чтобы установить униформу
        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0);

        glm::mat4 modelMatrix_1 = glm::mat4(1.f); // единичная матрица преобрзований
        modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(50.f, 50.f, 0.f)); // задаем матрицу преобразований, чтобы сместить треугольник на указанные координаты

        glm::mat4 modelMatrix_2 = glm::mat4(1.f); // единичная матрица преобрзований
        modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.f, 0.f)); // то же самое, только другие координаты (половина треугольника справа снизу). Ширина равна заданной 640


        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f);

        pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);


        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setInt("tex", 0);

        pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        /* Loop until the user closes the window - RENDERING */
        while (!glfwWindowShouldClose(pwindow))
        {
            // матрица задается в основном цикле, так как объекты движутся и матрицы модульные меняются

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            // DRAWING

            pDefaultShaderProgram->use();
            glBindVertexArray(vao);             // vao - vertex array objects
            tex->bind();

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_1);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_2);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pSprite->render();
            /* Swap front and back buffers */
            glfwSwapBuffers(pwindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}