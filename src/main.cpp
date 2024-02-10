#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer/ShaderProgram.h" 
#include "Resources/ResourceManager.h"


/*Задаём vertex*/
GLfloat point[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
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




int g_windowSizeX = 640;
int g_windowSizeY = 480;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY); // точка, откуда начинаем рисовать и на какую площадь
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
    GLFWwindow* pwindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Battle_City", NULL, NULL);
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

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);               // create buffer points_vbo where put data from buffer point[]
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo); // connect buffer and make it !current!
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW); // specify with what buffer we use and it size. Work only with !current! buffer

        // similarly for buffer colors[]

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

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



        /* Loop until the user closes the window - RENDERING */
        while (!glfwWindowShouldClose(pwindow))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            // DRAWING

            pDefaultShaderProgram->use();
            glBindVertexArray(vao);             // vao - vertex array objects
            glDrawArrays(GL_TRIANGLES, 0, 3);

            /* Swap front and back buffers */
            glfwSwapBuffers(pwindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}