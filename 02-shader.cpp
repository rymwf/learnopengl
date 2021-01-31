/**
 * @file 02-shader.cpp
 * @author yangzs
 * @brief 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "common.h"

GLint glVersion{0}; //set glversion,such as 33 mean use version 33 , if 0, use latest

constexpr char *vertFile = SHADER_PATH "02.vert";
constexpr char *fragFile = SHADER_PATH "02.frag";

class Hello
{
    GLFWwindow *window;
    GLuint vertShader;
    GLuint fragShader;
    GLuint programId;
    bool framebufferResized{true};

    void initWindow()
    {
        glfwInit();

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
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
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

#ifndef NODEBUG
        EnableDebugOutput();
#endif
        createTestProgram();

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
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
        glDeleteProgram(programId);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    void drawFrame()
    {
        resize();
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(programId);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void resize()
    {
        if (framebufferResized)
        {
            int width = 0, height = 0;
            glfwGetFramebufferSize(window, &width, &height);
            if (width == 0 || height == 0)
            {
                glfwGetFramebufferSize(window, &width, &height);
                glfwWaitEvents();
            }
            glViewport(0, 0, width, height);
            framebufferResized = false;
        }
    }

    static void framebufferResizeCallback(GLFWwindow *window, [[maybe_unused]] int width, [[maybe_unused]] int height)
    {
        auto app = static_cast<Hello *>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
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

    void createTestProgram()
    {
        if (glVersion > 41 && isSupportShaderBinaryFormat(SHADER_BINARY_FORMAT_SPIR_V))
        {
            auto vertCode = readFile((std::string(vertFile) + ".spv").c_str());
            auto fragCode = readFile((std::string(fragFile) + ".spv").c_str());
            vertShader = createShaderBinary(SHADER_STAGE_VERTEX, vertCode.data(), vertCode.size());
            fragShader = createShaderBinary(SHADER_STAGE_FRAGMENT, fragCode.data(), fragCode.size());
        }
        else
        {
            auto vertCode = readFile(vertFile);
            auto fragCode = readFile(fragFile);
            vertShader = createShader(SHADER_STAGE_VERTEX, vertCode.data());
            fragShader = createShader(SHADER_STAGE_FRAGMENT, fragCode.data());
        }
        programId = createProgram({vertShader, fragShader});
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