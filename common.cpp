/**
 * @file common.cpp
 * @author yangzs
 * @brief 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "common.h"

void APIENTRY DebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
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

std::string readFile(const char *filename)
{
    std::fstream file(filename, std::ios::in | std::ios::ate | std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("failed to open file");
    size_t size0 = static_cast<size_t>(file.tellg());
    std::string ret;
    ret.resize(size0);
    file.seekg(0);
    file.read(const_cast<char *>(ret.data()), size0);
    file.close();
    return ret;
}

GLuint createShader(ShaderStage stage, const GLchar *code)
{
    auto id = glCreateShader(Map(stage));
    if (id)
    {
        glShaderSource(id, 1, &code, NULL);
        glCompileShader(id);
        glReleaseShaderCompiler();
#ifndef NODEBUG
        GLint success;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            GLint len;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
            std::string log;
            log.resize(static_cast<size_t>(len));
            glGetShaderInfoLog(id, len, NULL, &log[0]);
            LOG(log);
            throw std::runtime_error("failed to compile shader");
        }
#endif
    }
    else
    {
        throw std::runtime_error("failed to create shader");
    }
    return id;
}

std::vector<GLint> getSupportedShaderBinaryFormat()
{
    GLint count;
    glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &count);
    std::vector<GLint> formats(count);
    glGetIntegerv(GL_SHADER_BINARY_FORMATS, formats.data());
    return formats;
}
std::vector<GLint> getSupportedProgramBinaryFormat()
{
    GLint count;
    glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &count);
    std::vector<GLint> formats(count);
    glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, formats.data());
    return formats;
}

bool isSupportShaderBinaryFormat(ShaderBinaryFormat format)
{
    auto shaderBinaryFormats = getSupportedShaderBinaryFormat();
    for (auto &shaderBinaryFormat : shaderBinaryFormats)
    {
        LOG(shaderBinaryFormat);
        if (static_cast<GLenum>(shaderBinaryFormat) == Map(format))
            return true;
    }
    return false;
}

GLuint createShaderBinary(ShaderStage stage, const void *binary, GLsizei length)
{
    auto id = glCreateShader(Map(stage));
    if (id)
    {
        glShaderBinary(1, &id, GL_SHADER_BINARY_FORMAT_SPIR_V, binary, length);
        glSpecializeShader(id, "main", 0, nullptr, nullptr);
    }
    else
    {
        throw std::runtime_error("failed to create shader");
    }
    return id;
}

GLuint createProgram(const std::vector<GLuint> &shaders, bool separable, bool retrievable)
{
    auto id = glCreateProgram();
    if (id)
    {
        glProgramParameteri(id, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, retrievable);
        glProgramParameteri(id, GL_PROGRAM_SEPARABLE, separable);
        for (auto shader : shaders)
            glAttachShader(id, shader);
        glLinkProgram(id);
#ifndef NODEBUG
        GLint success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            GLint len;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
            std::string log;
            log.resize(static_cast<size_t>(len));
            glGetProgramInfoLog(id, len, NULL, &log[0]);
            LOG(log);
            throw std::runtime_error("failed to link program");
        }
#endif
    }
    else
    {
        throw std::runtime_error("failed to create program");
    }
    return id;
}