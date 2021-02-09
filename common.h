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
#include <chrono>
#include <thread>

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

typedef uint32_t Format;
typedef uint32_t ShaderHandle;
typedef uint32_t ProgramHandle;
typedef uint32_t PipelineHandle;
typedef uint32_t BufferHandle;
typedef uint32_t VertexArrayHandle;
typedef uint32_t SamplerHandle;
typedef uint32_t ImageHandle;
typedef uint32_t MemoryHandle;	  //need GL_EXT_memory_object
typedef uint32_t SemaphoreHandle; //need GL_EXT_semaphore

struct Offset2D
{
	int32_t x;
	int32_t y;
};

struct Offset3D
{
	int32_t x;
	int32_t y;
	int32_t z;
};

struct Extent2D
{
	uint32_t width;
	uint32_t height;
};

struct Extent3D
{
	uint32_t width;
	uint32_t height;
	uint32_t depth;
};

struct Rect2D
{
	Offset2D offset;
	Extent2D extent;
};

struct Rect3D
{
	Offset3D offset;
	Extent3D extent;
};

union ClearColor
{
	float float32[4];
	int32_t int32[4];
	uint32_t uint32[4];
};

struct BorderColor
{
	DataType dataType;
	ClearColor color;
};

struct SamplerCreateInfo
{
	Filter magFilter;
	Filter minFilter;
	SamplerMipmapMode mipmapMode;
	SamplerWrapMode wrapModeU;
	SamplerWrapMode wrapModeV;
	SamplerWrapMode wrapModeW;
	float mipLodBias;
	bool anisotopyEnable;
	bool compareEnable;
	CompareOp compareOp;
	float minLod;
	float maxLod;
	BorderColor borderColor;
};

struct ImageCreateInfo
{
	ImageCreateFlags flags;
	ImageType imageType;
	Format format;
	Extent3D extent;
	uint32_t mipLevels;
	uint32_t arrayLayers;
	SampleCountFlagBits samples;
	ImageTiling tiling; //TODO:texture storage must be allocated using TexStorageMem*EXT
};

struct ImageSubresourceLayer
{
	uint32_t mipLevel;
	uint32_t baseArrayLayer;
	uint32_t layerCount;
};
struct ImageSubData
{
	Format format;
	DataType dataType;
	uint32_t mipLevel;
	Rect3D rect;
	void *data;
};

struct ComponentMapping
{
	ComponentSwizzle r;
	ComponentSwizzle g;
	ComponentSwizzle b;
	ComponentSwizzle a;
};
struct ImageSubresourceRange
{
	ImageAspectFlags aspectMask;
	uint32_t baseMipLevel;
	uint32_t levelCount;
	uint32_t baseArrayLayer;
	uint32_t layerCount;
};

struct ImageViewCreateInfo
{
	ImageHandle image;
	ImageViewType viewType;
	Format format;
	ComponentMapping components;
	ImageSubresourceRange subresourceRange;
};

struct DescriptorSetLayoutBinding
{
	uint32_t binding;
	DescriptorType descriptorType;
	uint32_t descriptorCount; //array
	ShaderStageFlags stageFlags;
	const SamplerHandle *pImmutableSamplers;
};

struct DescriptorSetLayout
{
	uint32_t bindingCount;
	const DescriptorSetLayout *pDescriptorSetLayouts;
};

//vulkan only
struct PushConstantRange
{
};

struct PipelineLayoutCreateInfo
{
	uint32_t setLayoutCount; //opengl must be 1
	const DescriptorSetLayout *pSetLayouts;
	uint32_t pushConstantRangeCount;
	const PushConstantRange *pPushConstantRanges;
};

struct DrawIndirectCommand
{
	uint32_t vertexCount;
	uint32_t instanceCount;
	uint32_t firstVertex;
	uint32_t firstInstance;
};

struct DrawIndexedIndirectCommand
{
	uint32_t indexCount;
	uint32_t instanceCount;
	uint32_t firstIndex;
	int32_t vertexOffset;
	uint32_t firstInstance;
};

struct BufferCreateInfo
{
	BufferCreateFlags flags;
	size_t size;
	union
	{
		BufferStorageFlags storageFlags;
		BufferMutableStorageUsage storageUsage;
	};
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

struct VertexInputStateCreateInfo
{
	uint32_t vertexBindingDescriptionCount;
	const VertexBindingDescription *pVertexBindingDescriptions;
	uint32_t vertexAttributeDescriptionCount;
	const VertexAttributeDescription *pVertexAttributeDescriptions;
};

struct GraphicsPipelineCreateInfo
{
	uint32_t stageCount;
	const PipelineShaderStageCreateInfo *pStages;
	const VertexInputStateCreateInfo *pVertexInputState;
};

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
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
			{startLocation + 2,
			 binding,
			 2,
			 DATA_TYPE_FLOAT,
			 false,
			 offsetof(Vertex, texCoord)},
		};
	}
	static uint32_t getLocationsNum()
	{
		return 3;
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

void createBuffer(const BufferCreateInfo &createInfo, const void *pData, BufferHandle *pBuffer);

/**
 * @brief Create a Vertex Array object
 * 
 * @param vertexInputDescription 
 * @param vertexBuffers 
 * @param indexBuffer  if indexBuffer is 0, then should call draw index
 * @param pVertexArray 
 */
void createVertexArray(const VertexInputStateCreateInfo &vertexInputDescription,
					   const std::vector<BufferHandle> &vertexBuffers,
					   const BufferHandle indexBuffer,
					   VertexArrayHandle *pVertexArray);

GLenum findSupportedTilingType(const std::vector<ImageTiling> &candidateTilings, ImageType imageType, bool multisample, GLenum format);

void createImage(const ImageCreateInfo &createInfo, ImageHandle *pImage);

void createImageView(const ImageViewCreateInfo &createInfo, bool multisample, ImageHandle *pImageViewHandle);

void updateImageSubData(ImageHandle image, ImageType imageType, const ImageSubData &imageSubData);

void setImageSampler(const SamplerCreateInfo &createInfo, ImageHandle image, ImageViewType imageViewType);