#include <cstdlib>
#include <exception>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifdef NDEBUG
#define LOG(str)
#else
#define LOG(str) \
    std::cout << __FILE__ << " " << __LINE__ << ":  " << #str << ": " << str << std::endl
#endif

constexpr GLuint WIDTH = 800, HEIGHT = 600;

class Hello
{

    GLFWwindow *window;

    void initWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#if 0
        //set gl versiont
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //use core
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

        listGLInfo();
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

    void listGLInfo()
    {
        //check opengl informations
        LOG(glGetString(GL_VENDOR));
        LOG(glGetString(GL_RENDERER));
        LOG(glGetString(GL_VERSION));
        LOG(glGetString(GL_SHADING_LANGUAGE_VERSION));

        constexpr GLenum glCapabilityEnum[] = {
            GL_BLEND,
            GL_COLOR_LOGIC_OP,
            GL_CULL_FACE,
            GL_DEPTH_CLAMP,
            GL_DEBUG_OUTPUT,
            GL_DEBUG_OUTPUT_SYNCHRONOUS,
            GL_DEPTH_TEST,
            GL_DITHER,
            GL_FRAMEBUFFER_SRGB,
            GL_LINE_SMOOTH,
            GL_MULTISAMPLE,
            GL_POLYGON_SMOOTH,
            GL_POLYGON_OFFSET_FILL,
            GL_POLYGON_OFFSET_LINE,
            GL_POLYGON_OFFSET_POINT,
            GL_PROGRAM_POINT_SIZE,
            GL_PRIMITIVE_RESTART,
            GL_SAMPLE_ALPHA_TO_COVERAGE,
            GL_SAMPLE_ALPHA_TO_ONE,
            GL_SAMPLE_COVERAGE,
            GL_SAMPLE_MASK,
            GL_SCISSOR_TEST,
            GL_STENCIL_TEST,
            GL_TEXTURE_CUBE_MAP_SEAMLESS,
        };
        constexpr char *glCapabilityEnumNames[]{
            "GL_BLEND",
            "GL_COLOR_LOGIC_OP",
            "GL_CULL_FACE",
            "GL_DEPTH_CLAMP",
            "GL_DEBUG_OUTPUT",
            "GL_DEBUG_OUTPUT_SYNCHRONOUS",
            "GL_DEPTH_TEST",
            "GL_DITHER",
            "GL_FRAMEBUFFER_SRGB",
            "GL_LINE_SMOOTH",
            "GL_MULTISAMPLE",
            "GL_POLYGON_SMOOTH",
            "GL_POLYGON_OFFSET_FILL",
            "GL_POLYGON_OFFSET_LINE",
            "GL_POLYGON_OFFSET_POINT",
            "GL_PROGRAM_POINT_SIZE",
            "GL_PRIMITIVE_RESTART",
            "GL_SAMPLE_ALPHA_TO_COVERAGE",
            "GL_SAMPLE_ALPHA_TO_ONE",
            "GL_SAMPLE_COVERAGE",
            "GL_SAMPLE_MASK",
            "GL_SCISSOR_TEST",
            "GL_STENCIL_TEST",
            "GL_TEXTURE_CUBE_MAP_SEAMLESS",
        };

        for (int i = 0, len = sizeof(glCapabilityEnum) / sizeof(glCapabilityEnum[0]); i < len; ++i)
        {
            LOG(glCapabilityEnumNames[i]);
            LOG(bool(glIsEnabled(glCapabilityEnum[i])));
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