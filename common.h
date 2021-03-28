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

#include <GL/glew.h>
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

#define THROW(str) throw std::runtime_error(__FILE__ " " + std::to_string(__LINE__) + ": " + str);

using Format = uint32_t;
using ShaderHandle = uint32_t;
using ProgramHandle = uint32_t;
using PipelineHandle = uint32_t;
using BufferHandle = uint32_t;
using VertexArrayHandle = uint32_t;
using SamplerHandle = uint32_t;
using ImageHandle = uint32_t;
using ImageViewHandle = uint32_t;
using BufferViewHandle = uint32_t;
using MemoryHandle = uint32_t;	  //need GL_EXT_memory_object
using SemaphoreHandle = uint32_t; //need GL_EXT_semaphore
using FramebufferHandle = uint32_t;
using RenderbufferHandle = uint32_t;

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

union ClearColorValue
{
	float float32[4];
	int32_t int32[4];
	uint32_t uint32[4];
};

struct ClearDepthStencilValue
{
	float depth;
	uint32_t stencil;
};

struct ClearValue
{
	ClearColorValue color;
	ClearDepthStencilValue depthStencil;
};

struct BorderColor
{
	DataType dataType;
	ClearColorValue color;
};

struct AttachmentDescription
{
	SampleCountFlagBits samples;
	AttachmentLoadOp loadOp;   //glclear
	AttachmentStoreOp storeOp; //opengl default store
	AttachmentLoadOp stencilLoadOp;
	AttachmentStoreOp stencilStoreOp;
};

struct AttachmentReference
{
	uint32_t attachment;
	//ImageLayout layout; //TODO:
};

struct SubpassDescription
{
	PipelineBindPoint pipelineBindPoint;
	std::shared_ptr<std::vector<AttachmentReference>> spColorAttachments;
	std::shared_ptr<AttachmentReference> spResolveAttachment;
	std::shared_ptr<AttachmentReference> spDepthStencilAttachment;
};

struct RenderPassCreateInfo
{
	std::shared_ptr<std::vector<AttachmentDescription>> spAttachments;
	std::shared_ptr<std::vector<SubpassDescription>> spSubpasses;
	//	std::shared_ptr<std::vector<SubpassDependencies>> spSubpassDependencies;	//TODO:spSubpassDependencies
};

class RenderPass
{
	std::unique_ptr<RenderPassCreateInfo> _createInfo;
public:
	RenderPass(const RenderPassCreateInfo &createInfo)
		: _createInfo(std::make_unique<RenderPassCreateInfo>(createInfo)){

		  };
	RenderPassCreateInfo* GetCreateInfoPtr(){
		return _createInfo.get();
	}
};

struct FramebufferCreateInfo
{
	RenderPass *renderpass;
	std::shared_ptr<std::vector<ImageHandle>> spAttachments; //image views
	uint32_t width;
	uint32_t height;
	uint32_t layers;
};

struct RenderPassBeginInfo
{
	RenderPass *renderPass;
	FramebufferHandle framebuffer;
	Rect2D renderArea;
	std::vector<ClearValue> clearValues;
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

struct RenderBufferCreateInfo{
	Format format;
	Extent2D extent;
	SampleCountFlagBits samples;
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
//	ImageTiling tiling; 
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
	uint32_t binding; //shader binding
	DescriptorType descriptorType;
	uint32_t descriptorCount;	 //array
	ShaderStageFlags stageFlags; //vulkan
	std::vector<SamplerHandle> immutableSamplers;
};

struct DescriptorImageInfo
{
	//SamplerHandle sampler;
	SamplerCreateInfo samplerCreateInfo;
	ImageHandle imageView;
	//ImageLayout imageLayout;	//TODO:
};
struct DescriptorBufferInfo
{
	BufferHandle buffer;
	uint32_t offset;
	uint32_t range;
};

struct DescriptorSetLayoutCreateInfo
{
	std::vector<DescriptorSetLayoutBinding> descriptorSetLayoutBindings;
};
struct DescriptorSetLayout_T
{
	DescriptorSetLayoutCreateInfo _createInfo;
};
typedef DescriptorSetLayout_T *DescriptorSetLayout;

struct DescriptorSet_T
{
	struct BufferObj
	{
		DescriptorSetLayoutBinding binding;
		std::vector<DescriptorBufferInfo> buffersInfo;
	};
	struct ImageObj
	{
		DescriptorSetLayoutBinding binding;
		std::vector<ImageHandle> imageViews;
		ImageViewType imageViewType;
		bool multisample;
	};
	std::vector<BufferObj> _buffers;
	std::vector<ImageObj> _images;
	DescriptorSet_T(const std::vector<DescriptorSetLayout> &layouts)
	{
		for (auto layout : layouts)
		{
			for (const auto &layoutBinding : layout->_createInfo.descriptorSetLayoutBindings)
			{
				if (layoutBinding.descriptorType == DESCRIPTOR_TYPE_UNIFORM_BUFFER ||
					layoutBinding.descriptorType == DESCRIPTOR_TYPE_STORAGE_BUFFER)
				{
					_buffers.emplace_back(BufferObj{layoutBinding, std::vector<DescriptorBufferInfo>(layoutBinding.descriptorCount)});
				}
				else if (
					layoutBinding.descriptorType == DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ||
					layoutBinding.descriptorType == DESCRIPTOR_TYPE_SAMPLED_IMAGE ||
					layoutBinding.descriptorType == DESCRIPTOR_TYPE_STORAGE_IMAGE ||
					layoutBinding.descriptorType == DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER ||
					layoutBinding.descriptorType == DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER)
				{
					_images.emplace_back(ImageObj{layoutBinding, std::vector<ImageHandle>(layoutBinding.descriptorCount)});
				}
				else
				{
					throw std::runtime_error("descriptor not supported yet");
				}
			}
		}
	}
	void Bind()
	{
		for (auto &buffer : _buffers)
		{
			for (uint32_t i = 0; i < buffer.binding.descriptorCount; ++i)
				glBindBufferRange(
					GL_UNIFORM_BUFFER,
					buffer.binding.binding + i,
					buffer.buffersInfo[i].buffer,
					buffer.buffersInfo[i].offset,
					buffer.buffersInfo[i].range);
		}
		for (auto &image : _images)
		{
			for (uint32_t i = 0; i < image.binding.descriptorCount; ++i)
			{
				glActiveTexture(GL_TEXTURE0 + image.binding.binding + i);
				glBindTexture(Map(image.imageViewType, image.multisample), image.imageViews[i]);
			}
		}
	}
};
typedef DescriptorSet_T *DescriptorSet;

struct WriteDescriptorSet
{
	DescriptorSet dstSet;
	uint32_t dstBinding;
	uint32_t dstArrayElement;
	DescriptorType descriptorType;
	std::vector<DescriptorImageInfo> imagesInfo;
	ImageViewType viewType;
	bool multisample;
	std::vector<DescriptorBufferInfo> buffersInfo;
	std::vector<BufferViewHandle> texelBufferView; //TODO: buffer texture
};
struct CopyDescriptorSet
{
	//TODO:
};
//vulkan only
struct PushConstantRange
{
	ShaderStageFlags stageFlags;
	uint32_t offset;
	uint32_t size;
};

struct PipelineLayoutCreateInfo
{
	std::vector<DescriptorSetLayout> setLayouts; //opengl must be 1
	std::vector<PushConstantRange> pushConstantRanges;
};
struct PipelineLayout_T
{
	PipelineLayoutCreateInfo _createInfo;
};
typedef PipelineLayout_T *PipelineLayout;

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
	std::vector<uint32_t> constantIDs;
	std::vector<uint32_t> constantValues;
};

struct PipelineShaderStageCreateInfo
{
	ShaderStageFlagBits stage;
	ShaderHandle shaderHandle;
	std::string entryName;
	SpecializationInfo specializationInfo;
};

struct ShaderCreateInfo
{
	ShaderStageFlagBits stage;
	std::string code;
};

struct ProgramCreateInfo
{
	std::vector<ShaderHandle> shaders;
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
	uint32_t stride;
	uint32_t divisor; //attributes advance once per divior instances,when 0, advance per vertex
};

struct VertexInputStateCreateInfo
{
	std::vector<VertexBindingDescription> vertexBindingDescriptions;
	std::vector<VertexAttributeDescription> vertexAttributeDescriptions;
};

struct GraphicsPipelineCreateInfo
{
	std::vector<PipelineShaderStageCreateInfo> stages;
	VertexInputStateCreateInfo vertexInputState;
	PipelineLayout layout;
};

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
	static VertexBindingDescription getVertexBindingDescription()
	{
		return {
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

struct InstanceAttribute
{
	glm::mat4 translation;
	static VertexBindingDescription getVertexBindingDescription()
	{
		return {sizeof(InstanceAttribute), 1};
	}
	static std::vector<VertexAttributeDescription> getVertexAttributeDescription(uint32_t startLocation, uint32_t binding)
	{
		return {
			{startLocation + 0,
			 binding,
			 4,
			 DATA_TYPE_FLOAT,
			 false,
			 0},
			{startLocation + 1,
			 binding,
			 4,
			 DATA_TYPE_FLOAT,
			 false,
			 16},
			{startLocation + 2,
			 binding,
			 4,
			 DATA_TYPE_FLOAT,
			 false,
			 32},
			{startLocation + 3,
			 binding,
			 4,
			 DATA_TYPE_FLOAT,
			 false,
			 48},
		};
	}
	static uint32_t getLocationCount()
	{
		return 4;
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
void createGraphicsPipeline(const GraphicsPipelineCreateInfo &createInfo, PipelineHandle *pPipeline, std::vector<ProgramHandle> *pPrograms);

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
void createVertexArray2(const VertexInputStateCreateInfo &vertexInputStateCreateInfo, VertexArrayHandle &vao);

ImageTiling findSupportedTilingType(const std::vector<ImageTiling> &candidateTilings, ImageType imageType, bool multisample, GLenum format);

void createMemory(MemoryHandle& memory);

void createImage(const ImageCreateInfo &createInfo, ImageHandle *pImage);

void createImage2(const ImageCreateInfo &createInfo, ImageHandle &image, const MemoryHandle &memory);

void createImageView(const ImageViewCreateInfo &createInfo, bool multisample, ImageViewHandle *pImageViewHandle);

void updateImageSubData(ImageHandle image, ImageType imageType, const ImageSubData &imageSubData);

void setImageSampler(const SamplerCreateInfo &createInfo, ImageViewHandle image, ImageViewType imageViewType, bool multisample = false);

void createDescriptorSetLayout(const DescriptorSetLayoutCreateInfo &createInfo, DescriptorSetLayout &outSetLayout);

void destroyDescriptorSetLayout(DescriptorSetLayout &setLayout);

void createDescriptorSet(const std::vector<DescriptorSetLayout> &descriptorSetLayouts, DescriptorSet &descriptorSet);

void destroyDescriptorSet(DescriptorSet descriptorSet);

void updateDescriptorSets(const std::vector<WriteDescriptorSet> &descriptorWrites, const std::vector<CopyDescriptorSet> &descriptorCopy);
