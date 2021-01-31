/**
 * @file 01-init.cpp
 * @author yangzs
 * @brief 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "common.h"

GLint glVersion{0}; //set glversion,such as 33 mean use version 33 , if 0, use latest

class Hello
{
    GLFWwindow *window;

    void initWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        //set gl versiont
        if (glVersion > 0)
        {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersion / 10);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersion % 10);
        }

#ifndef NDEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

        window = glfwCreateWindow(WIDTH, HEIGHT, __FILE__, nullptr, nullptr);
        if (!window)
        {
            glfwTerminate();
            throw std::runtime_error("failed to create window");
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); //enable v sync
    }
    void initOpengl()
    {
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(reinterpret_cast<uintptr_t>(glfwGetProcAddress))))
            throw std::runtime_error("failed to load glad");

        //update glVersion
        GLint a, b;
        glGetIntegerv(GL_MAJOR_VERSION, &a);
        glGetIntegerv(GL_MINOR_VERSION, &b);
        glVersion = a * 10 + b;
        LOG(glVersion);

        listGLInfo();
        EnableDebugOutput();

        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    }
    void mainLoop()
    {
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            drawFrame();
            glfwSwapBuffers(window);
        }
    }
    void cleanup()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    void drawFrame()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void EnableDebugOutput()
    {
        // enable OpenGL debug context if context allows for debug context
        GLint flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously

            //opengl 4.3
            glDebugMessageCallback(DebugOutputCallback, this);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
    }

public:
    void run()
    {
        initWindow();
        initOpengl();
        mainLoop();
        cleanup();
    }
};

int main()
{
    Hello app;
    try
    {
        app.run();
    }
    catch (const std::exception &e)
    {
        LOG(e.what());
    }
}