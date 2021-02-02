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

#ifdef NDEBUG
#define LOG(str)
#define LOG_VAR(str)
#else
#define LOG(str) \
	std::cout << __FILE__ << " " << __LINE__ << ":  " << str << std::endl
#define LOG_VAR(str) \
	std::cout << __FILE__ << " " << __LINE__ << ":  " << #str << ": " << str << std::endl
#endif

constexpr GLuint WIDTH = 800, HEIGHT = 600;

typedef uint32_t ShaderHandle;
typedef uint32_t ProgramHandle;
typedef uint32_t PipelineHandle;
typedef uint32_t BufferHandle;
typedef uint32_t VertexArrayHandle;

struct BufferCreateInfo
{
	size_t size;
	const void *pData;
	BufferStorageFlags storageFlags;
};

struct SpecializationInfo
{
	uint32_t numSpecializationConstants;
	const uint32_t *pConstantIDs;
	const uint32_t *pConstantValues;
};

struct PipelineShaderStageCreateInfo
{
	ShaderStageFlagBits stage;
	ShaderHandle shaderHandle;
	const char *pEntryName;
	const SpecializationInfo *pSecializationInfo;
};

struct ShaderCreateInfo
{
	ShaderStageFlagBits stage;
	size_t codeSize;
	const char *pCode;
};

struct ProgramCreateInfo
{
	uint32_t shaderCount;
	const ShaderHandle *pShaders;
	bool separable;
	bool retrievable;
};

struct VertexAttributeDescription
{
	uint32_t location;	 //location in shader
	uint32_t binding;	 //index in given buffers
	uint32_t components; //1,2,3,4
	DataType dataType;
	bool normalized;
	uint32_t offset;
};

struct VertexBindingDescription
{
	uint32_t binding; //index in given buffers
	uint32_t stride;
	uint32_t divisor; //attributes advance once per divior instances,when 0, advance per vertex
};

struct VertexInputStateDescription
{
	uint32_t vertexBindingDescriptionsCount;
	const VertexBindingDescription *pVertexBindingDescriptions;
	uint32_t vertexAttributeDescriptionsCount;
	const VertexAttributeDescription *pVertexAttributeDescriptions;
};

struct GraphicsPipelineCreateInfo
{
	uint32_t stageCount;
	const PipelineShaderStageCreateInfo *pStages;
};

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 color;
	static VertexBindingDescription getVertexBindingDescription(uint32_t binding)
	{
		return {
			binding,
			sizeof(Vertex),
			0,
		};
	}
	static std::vector<VertexAttributeDescription> getVertexAttributeDescription(uint32_t startLocation, uint32_t binding)
	{
		return {
			{startLocation + 0,
			 binding,
			 3,
			 DATA_TYPE_FLOAT,
			 false,
			 offsetof(Vertex, pos)},
			{startLocation + 1,
			 binding,
			 3,
			 DATA_TYPE_FLOAT,
			 true,
			 offsetof(Vertex, color)},
		};
	}
	static uint32_t getLocationsNum()
	{
		return 2;
	}
};

void EnableDebugOutput(const void *userParam);
void APIENTRY DebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

void listGLInfo();

std::string readFile(const char *filename);

std::vector<GLint> getSupportedShaderBinaryFormat();
std::vector<GLint> getSupportedProgramBinaryFormat();

bool isSupportShaderBinaryFormat(ShaderBinaryFormat format);

void createShader(const ShaderCreateInfo &createInfo, ShaderHandle *pShader);

/**
 * @brief must be specialized beforing linking to a program
 * 
 */
void createShaderBinary(const ShaderCreateInfo &createInfo, ShaderHandle *pShader);
void createProgram(const ProgramCreateInfo &createInfo, ProgramHandle *pProgram);
void createGraphicsPipeline(const GraphicsPipelineCreateInfo &createInfo, PipelineHandle *pPipeline, ProgramHandle *pPrograms);

void createBuffer(const BufferCreateInfo &createInfo, BufferHandle *pBuffer);

void createVertexArray(const VertexInputStateDescription &vertexInputDescription,
					   const std::vector<BufferHandle> &buffers,
					   VertexArrayHandle *pVertexArray);