#include <cstdlib>
#include <exception>
#include <iostream>
#include <sstream>

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

    void listGLInfo()
    {
        //check opengl informations
        LOG(glGetString(GL_VENDOR));
        LOG(glGetString(GL_RENDERER));
        LOG(glGetString(GL_VERSION));
        LOG(glGetString(GL_SHADING_LANGUAGE_VERSION));

        //extensions
        GLint a;
        glGetIntegerv(GL_NUM_EXTENSIONS, &a);
        for (GLint i = 0; i < a; ++i)
            LOG(glGetStringi(GL_EXTENSIONS, static_cast<GLuint>(i)));

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

    static void APIENTRY DebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
    {
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
            return; // ignore these non-significant error codes
        std::stringstream sstr;
        sstr
            << "Debug message id:" << id
            << " length:" << length
            << " useParam:" << userParam
            << " message:" << message
            << "\n";
        //		LOG_DEBUG(":{}, message:{}", id, message);
        switch (source)
        {
        case GL_DEBUG_SOURCE_API:
            sstr << "Source: API :0x" << std::hex << GL_DEBUG_SOURCE_API;
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            sstr << "Source: Window System :0x" << std::hex << GL_DEBUG_SOURCE_WINDOW_SYSTEM;
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            sstr << "Source: Shader Compiler :0x" << std::hex << GL_DEBUG_SOURCE_SHADER_COMPILER;
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            sstr << "Source: Third Party :0x" << std::hex << GL_DEBUG_SOURCE_THIRD_PARTY;
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            sstr << "Source: Application :0x" << std::hex << GL_DEBUG_SOURCE_APPLICATION;
            break;
        case GL_DEBUG_SOURCE_OTHER:
            sstr << "Source: Other :0x" << std::hex << GL_DEBUG_SOURCE_OTHER;
            break;
        }
        sstr << "\n";
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:
            sstr << "Type: Error :0x" << std::hex << GL_DEBUG_TYPE_ERROR;
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            sstr << "Type: Deprecated Behaviour :0x" << std::hex << GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR;
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            sstr << "Type: Undefined Behaviour :0x" << std::hex << GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR;
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            sstr << "Type: Portability :0x" << std::hex << GL_DEBUG_TYPE_PORTABILITY;
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            sstr << "Type: Performance :0x" << std::hex << GL_DEBUG_TYPE_PERFORMANCE;
            break;
        case GL_DEBUG_TYPE_MARKER:
            sstr << "Type: Marker :0x" << std::hex << GL_DEBUG_TYPE_MARKER;
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            sstr << "Type: Push Group :0x" << std::hex << GL_DEBUG_TYPE_PUSH_GROUP;
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            sstr << "Type: Pop Group :0x" << std::hex << GL_DEBUG_TYPE_POP_GROUP;
            break;
        case GL_DEBUG_TYPE_OTHER:
            sstr << "Type: Other :0x" << std::hex << GL_DEBUG_TYPE_OTHER;
            break;
        }

        sstr << "\n";
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            sstr << "Severity: high :0x" << std::hex << GL_DEBUG_SEVERITY_HIGH;
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            sstr << "Severity: medium :0x" << std::hex << GL_DEBUG_SEVERITY_MEDIUM;
            break;
        case GL_DEBUG_SEVERITY_LOW:
            sstr << "Severity: low :0x" << std::hex << GL_DEBUG_SEVERITY_LOW;
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            sstr << "Severity: notification :0x" << std::hex << GL_DEBUG_SEVERITY_NOTIFICATION;
            break;
        }
        LOG(sstr.str());
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