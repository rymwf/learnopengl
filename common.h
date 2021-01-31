/**
 * @file common.h
 * @author yangzs
 * @brief 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#include <cstdlib>
#include <exception>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "types.h"

#define SHADER_PATH "./shaders/"
#define SHADER_BINARY_PATH "./"

#ifdef NDEBUG
#define LOG(str)
#else
#define LOG(str) \
    std::cout << __FILE__ << " " << __LINE__ << ":  " << #str << ": " << str << std::endl
#endif

constexpr GLuint WIDTH = 800, HEIGHT = 600;

void APIENTRY DebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

void listGLInfo();

std::string readFile(const char *filename);

std::vector<GLint> getSupportedShaderBinaryFormat();
std::vector<GLint> getSupportedProgramBinaryFormat();

bool isSupportShaderBinaryFormat(ShaderBinaryFormat format);

GLuint createShader(ShaderStage stage, const GLchar *code);
GLuint createShaderBinary(ShaderStage ShaderStage, const void *binary, GLsizei length);
GLuint createProgram(const std::vector<GLuint> &shaders, bool separable = false, bool retrievable = false);