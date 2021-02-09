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

void EnableDebugOutput(const void *userParam)
{
	// enable OpenGL debug context if context allows for debug context
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed
											   // synchronously

		if (GLVersion.major * 10 + GLVersion.minor >= 43)
		{
			glDebugMessageCallback(DebugOutputCallback, userParam);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
								  NULL, GL_TRUE);
		}
		else
		{
#ifdef GL_ARB_debug_output
			glDebugMessageCallbackARB(DebugOutputCallback, userParam);
			glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
									 NULL, GL_TRUE);
#else
			LOG("current opengl version do not support debug output");
#endif
		}
	}
	else
	{
		LOG("current context do not support debug output");
	}
}

void APIENTRY DebugOutputCallback(GLenum source, GLenum type, GLuint id,
								  GLenum severity, GLsizei length,
								  const GLchar *message,
								  const void *userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return; // ignore these non-significant error codes
	std::stringstream sstr;
	sstr << "Debug message id:" << id << " length:" << length
		 << " useParam:" << userParam << " message:" << message << "\n";
	//		LOG_DEBUG(":{}, message:{}", id, message);
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		sstr << "Source: API :0x" << std::hex << GL_DEBUG_SOURCE_API;
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		sstr << "Source: Window System :0x" << std::hex
			 << GL_DEBUG_SOURCE_WINDOW_SYSTEM;
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		sstr << "Source: Shader Compiler :0x" << std::hex
			 << GL_DEBUG_SOURCE_SHADER_COMPILER;
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		sstr << "Source: Third Party :0x" << std::hex
			 << GL_DEBUG_SOURCE_THIRD_PARTY;
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		sstr << "Source: Application :0x" << std::hex
			 << GL_DEBUG_SOURCE_APPLICATION;
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
		sstr << "Type: Deprecated Behaviour :0x" << std::hex
			 << GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR;
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		sstr << "Type: Undefined Behaviour :0x" << std::hex
			 << GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR;
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
		sstr << "Severity: notification :0x" << std::hex
			 << GL_DEBUG_SEVERITY_NOTIFICATION;
		break;
	}
	LOG(sstr.str());
}

void listGLInfo()
{
#ifdef GL_EXT_memory_object
	int NUM_DEVICE_UUIDS_EXT;
	glGetIntegerv(GL_NUM_DEVICE_UUIDS_EXT, &NUM_DEVICE_UUIDS_EXT);
	LOG_VAR(NUM_DEVICE_UUIDS_EXT);
	int DEVICE_UUID_EXT;
	for (int i = 0; i < NUM_DEVICE_UUIDS_EXT; ++i)
	{
		LOG(i);
		glGetIntegeri_v(GL_DEVICE_UUID_EXT, i, &DEVICE_UUID_EXT);
		LOG_VAR(DEVICE_UUID_EXT);
	}
	int DRIVER_UUID_EXT;
	glGetIntegerv(GL_DRIVER_UUID_EXT, &DRIVER_UUID_EXT);
	LOG_VAR(DRIVER_UUID_EXT);
#endif

	LOG_VAR(GLVersion.major);
	LOG_VAR(GLVersion.minor);

	// check opengl informations
	LOG_VAR(glGetString(GL_VENDOR));
	LOG_VAR(glGetString(GL_RENDERER));
	LOG_VAR(glGetString(GL_VERSION));
	LOG_VAR(glGetString(GL_SHADING_LANGUAGE_VERSION));
	int samples;
	glGetIntegerv(GL_SAMPLES, &samples);
	LOG_VAR(samples);

	// extensions
#if 0
	GLint a;
	glGetIntegerv(GL_NUM_EXTENSIONS, &a);
	for (GLint i = 0; i < a; ++i)
		LOG(glGetStringi(GL_EXTENSIONS, static_cast<GLuint>(i)));
#endif

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

	for (int i = 0, len = sizeof(glCapabilityEnum) / sizeof(glCapabilityEnum[0]);
		 i < len; ++i)
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

void createShader(const ShaderCreateInfo &createInfo, ShaderHandle *pShader)
{
	GLenum shaderType{};
	switch (createInfo.stage)
	{
	case SHADER_STAGE_VERTEX_BIT:
		shaderType = GL_VERTEX_SHADER;
		break;
	case SHADER_STAGE_FRAGMENT_BIT:
		shaderType = GL_FRAGMENT_SHADER;
		break;
	case SHADER_STAGE_GEOMETRY_BIT:
		shaderType = GL_GEOMETRY_SHADER;
		break;
	case SHADER_STAGE_TESS_CONTROL_BIT:
		shaderType = GL_TESS_CONTROL_SHADER;
		break;
	case SHADER_STAGE_TESS_EVALUATION_BIT:
		shaderType = GL_TESS_EVALUATION_SHADER;
		break;
	case SHADER_STAGE_COMPUTE_BIT:
		shaderType = GL_COMPUTE_SHADER;
		break;
	case SHADER_STAGE_ALL_BITS:
		break;
	}
	*pShader = glCreateShader(shaderType);
	if (*pShader)
	{
		glShaderSource(*pShader, 1, &createInfo.pCode, NULL);
		glCompileShader(*pShader);
		glReleaseShaderCompiler();
#ifndef NODEBUG
		GLint success;
		glGetShaderiv(*pShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLint len;
			glGetShaderiv(*pShader, GL_INFO_LOG_LENGTH, &len);
			std::string log;
			log.resize(static_cast<size_t>(len));
			glGetShaderInfoLog(*pShader, len, NULL, &log[0]);
			LOG(log);
			glDeleteShader(*pShader);
			throw std::runtime_error("failed to compile shader");
		}
#endif
	}
	else
	{
		throw std::runtime_error("failed to create shader");
	}
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
		LOG_VAR(shaderBinaryFormat);
		if (static_cast<GLenum>(shaderBinaryFormat) == Map(format))
			return true;
	}
	return false;
}

void createShaderBinary(const ShaderCreateInfo &createInfo, ShaderHandle *pShader)
{
	GLenum shaderType{};
	switch (createInfo.stage)
	{
	case SHADER_STAGE_VERTEX_BIT:
		shaderType = GL_VERTEX_SHADER;
		break;
	case SHADER_STAGE_FRAGMENT_BIT:
		shaderType = GL_FRAGMENT_SHADER;
		break;
	case SHADER_STAGE_GEOMETRY_BIT:
		shaderType = GL_GEOMETRY_SHADER;
		break;
	case SHADER_STAGE_TESS_CONTROL_BIT:
		shaderType = GL_TESS_CONTROL_SHADER;
		break;
	case SHADER_STAGE_TESS_EVALUATION_BIT:
		shaderType = GL_TESS_EVALUATION_SHADER;
		break;
	case SHADER_STAGE_COMPUTE_BIT:
		shaderType = GL_COMPUTE_SHADER;
		break;
	case SHADER_STAGE_ALL_BITS:
		break;
	}

	*pShader = glCreateShader(shaderType);
	if (*pShader)
	{
		glShaderBinary(1, pShader, GL_SHADER_BINARY_FORMAT_SPIR_V, createInfo.pCode, createInfo.codeSize);
	}
	else
		throw std::runtime_error("failed to create shader");
}

void createProgram(const ProgramCreateInfo &createInfo, ProgramHandle *pProgram)
{
	*pProgram = glCreateProgram();
	if (*pProgram)
	{
		glProgramParameteri(*pProgram, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, createInfo.retrievable);
		glProgramParameteri(*pProgram, GL_PROGRAM_SEPARABLE, createInfo.separable);
		for (uint32_t i = 0; i < createInfo.shaderCount; ++i)
			glAttachShader(*pProgram, createInfo.pShaders[i]);
		glLinkProgram(*pProgram);
#ifndef NODEBUG
		GLint success;
		glGetProgramiv(*pProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLint len;
			glGetProgramiv(*pProgram, GL_INFO_LOG_LENGTH, &len);
			std::string log;
			log.resize(static_cast<size_t>(len));
			glGetProgramInfoLog(*pProgram, len, NULL, &log[0]);
			LOG(log);
			glDeleteProgram(*pProgram);
			throw std::runtime_error("failed to link program");
		}
#endif
	}
	else
	{
		throw std::runtime_error("failed to create program");
	}
}

void createGraphicsPipeline(const GraphicsPipelineCreateInfo &createInfo, PipelineHandle *pPipeline, ProgramHandle *pPrograms)
{
	if (GLVersion.major * 10 + GLVersion.minor > 41)
	{
		glGenProgramPipelines(1, pPipeline);
	}
	else
	{
#ifdef GL_EXT_separate_shader_objects
		glGenProgramPipelinesEXT(1, pPipeline);
#else
		throw std::runtime_error("failed to create pipelines");
#endif
	}
	glBindProgramPipeline(*pPipeline);
	for (uint32_t j = 0; j < createInfo.stageCount; ++j)
	{
		auto &stageCreateInfo = createInfo.pStages[j];
		//equal to compilation
		glSpecializeShader(
			stageCreateInfo.shaderHandle,
			stageCreateInfo.pEntryName,
			stageCreateInfo.pSecializationInfo->numSpecializationConstants,
			stageCreateInfo.pSecializationInfo->pConstantIDs,
			stageCreateInfo.pSecializationInfo->pConstantValues);

		// Specialization is equivalent to compilation.
		GLint isCompiled = 0;
		glGetShaderiv(stageCreateInfo.shaderHandle, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(stageCreateInfo.shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::string infoLog;
			infoLog.resize(maxLength);
			glGetShaderInfoLog(stageCreateInfo.shaderHandle, maxLength, &maxLength, infoLog.data());

			LOG(infoLog);
			// We don't need the shader anymore.
			glDeleteShader(stageCreateInfo.shaderHandle);

			// Use the infoLog as you see fit.
			throw std::runtime_error("failed to specialize shader");
		}
		createProgram({1, &stageCreateInfo.shaderHandle}, &pPrograms[j]);
		glUseProgramStages(*pPipeline, stageCreateInfo.stage, pPrograms[j]);
	}
}
void createBuffer(const BufferCreateInfo &createInfo, const void *pData, BufferHandle *pBuffer)
{
	glGenBuffers(1, pBuffer);
	// target do not matter when creating buffer
	glBindBuffer(GL_ARRAY_BUFFER, *pBuffer);
	if (createInfo.flags & BUFFER_CREATE_MUTABLE_FORMAT_BIT)
		glBufferData(GL_ARRAY_BUFFER, createInfo.size, pData, Map(createInfo.storageUsage));
	else
		//(GLVersion.major * 10 + GLVersion.minor < 44 || GL_ARB_buffer_storage)
		glBufferStorage(GL_ARRAY_BUFFER, createInfo.size, pData, createInfo.storageFlags);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void createVertexArray(const VertexInputStateCreateInfo &vertexInputDescription,
					   const std::vector<BufferHandle> &vertexBuffers,
					   const BufferHandle indexBuffer,
					   VertexArrayHandle *pVertexArray)
{
	std::vector<std::pair<int, int>> bindingDescriptions(vertexInputDescription.vertexBindingDescriptionCount);

	for (uint32_t i = 0; i < vertexInputDescription.vertexBindingDescriptionCount; ++i)
	{
		const auto &e = vertexInputDescription.pVertexBindingDescriptions[i];
		bindingDescriptions[e.binding] = {e.stride, e.divisor};
	}

	glGenVertexArrays(1, pVertexArray);
	if (*pVertexArray == 0)
		throw std::runtime_error("failed to create vertex array object");
	glBindVertexArray(*pVertexArray);

	for (uint32_t i = 0; i < vertexInputDescription.vertexAttributeDescriptionCount; ++i)
	{
		const auto &attrib = vertexInputDescription.pVertexAttributeDescriptions[i];
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[attrib.binding]);
		glEnableVertexAttribArray(attrib.location);
		glVertexAttribPointer(
			attrib.location, attrib.components,
			Map(attrib.dataType), attrib.normalized,
			bindingDescriptions[attrib.binding].first,
			(void *)attrib.offset);
		glVertexAttribDivisor(attrib.location, bindingDescriptions[attrib.binding].second);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBindVertexArray(0);
}

void createImage(const ImageCreateInfo &createInfo, ImageHandle *pImage)
{
	glGenTextures(1, pImage);
	GLenum target = Map(createInfo.imageType, createInfo.samples > SAMPLE_COUNT_1_BIT);
	glBindTexture(target, *pImage);
	if (createInfo.flags & IMAGE_CREAT_MUTABLE_FORMAT_BIT)
	{
		if (createInfo.samples > SAMPLE_COUNT_1_BIT)
		{
			glTexImage3DMultisample(GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
									createInfo.samples,
									createInfo.format,
									createInfo.extent.width,
									createInfo.extent.height,
									createInfo.extent.depth,
									GL_FALSE);
		}
		else
		{
			switch (createInfo.imageType)
			{
			case IMAGE_TYPE_1D:
				glTexImage2D(GL_TEXTURE_1D_ARRAY,
							 0,
							 createInfo.format,
							 createInfo.extent.width,
							 createInfo.extent.height,
							 0,
							 GL_RGBA,
							 GL_UNSIGNED_BYTE,
							 nullptr);
				break;
			case IMAGE_TYPE_2D:
				glTexImage3D(GL_TEXTURE_2D_ARRAY,
							 0,
							 createInfo.format,
							 createInfo.extent.width,
							 createInfo.extent.height,
							 createInfo.extent.depth,
							 0,
							 GL_RGBA,
							 GL_UNSIGNED_BYTE,
							 nullptr);
				break;
			case IMAGE_TYPE_3D:
				glTexImage3D(GL_TEXTURE_3D,
							 0,
							 createInfo.format,
							 createInfo.extent.width,
							 createInfo.extent.height,
							 createInfo.extent.depth,
							 0,
							 GL_RGBA,
							 GL_UNSIGNED_BYTE,
							 nullptr);
				break;
			}
		}
	}
	else
	{
		if (createInfo.samples > SAMPLE_COUNT_1_BIT)
		{
			glTexStorage3DMultisample(target,
									  createInfo.samples,
									  createInfo.format,
									  createInfo.extent.width,
									  createInfo.extent.height,
									  createInfo.extent.depth,
									  GL_FALSE);
		}
		else
		{
			switch (createInfo.imageType)
			{
			case IMAGE_TYPE_1D:
				glTexStorage2D(target,
							   createInfo.mipLevels,
							   createInfo.format,
							   createInfo.extent.width,
							   createInfo.extent.height);
				break;
			case IMAGE_TYPE_2D:
				glTexStorage3D(target,
							   createInfo.mipLevels,
							   createInfo.format,
							   createInfo.extent.width,
							   createInfo.extent.height,
							   createInfo.extent.depth);
				break;
			case IMAGE_TYPE_3D:
				glTexStorage3D(target,
							   createInfo.mipLevels,
							   createInfo.format,
							   createInfo.extent.width,
							   createInfo.extent.height,
							   createInfo.extent.depth);
				break;
			}
		}
	}
	glBindTexture(target, 0);
}

void createSampler(const SamplerCreateInfo &createInfo, SamplerHandle *pSampler)
{
	glGenSamplers(1, pSampler);
	glSamplerParameteri(*pSampler, GL_TEXTURE_MAG_FILTER, Map(createInfo.magFilter));
	if (createInfo.mipmapMode == SAMPLER_MIPMAP_MODE_LINEAR)
	{
		if (createInfo.minFilter == FILTER_LINEAR)
			glSamplerParameteri(*pSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glSamplerParameteri(*pSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	}
	else
	{
		if (createInfo.minFilter == FILTER_LINEAR)
			glSamplerParameteri(*pSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		else
			glSamplerParameteri(*pSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	glSamplerParameterf(*pSampler, GL_TEXTURE_LOD_BIAS, createInfo.mipLodBias);
	glSamplerParameterf(*pSampler, GL_TEXTURE_MIN_LOD, createInfo.minLod);
	glSamplerParameterf(*pSampler, GL_TEXTURE_MAX_LOD, createInfo.maxLod);
	glSamplerParameteri(*pSampler, GL_TEXTURE_WRAP_S, Map(createInfo.wrapModeU));
	glSamplerParameteri(*pSampler, GL_TEXTURE_WRAP_T, Map(createInfo.wrapModeV));
	glSamplerParameteri(*pSampler, GL_TEXTURE_WRAP_R, Map(createInfo.wrapModeW));
	if (createInfo.compareEnable)
	{
		glSamplerParameteri(*pSampler, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glSamplerParameteri(*pSampler, GL_TEXTURE_COMPARE_FUNC, Map(createInfo.compareOp));
	}
	if (createInfo.borderColor.dataType == DATA_TYPE_INT)
		glSamplerParameterIiv(*pSampler, GL_TEXTURE_BORDER_COLOR, createInfo.borderColor.color.int32);
	else if (createInfo.borderColor.dataType == DATA_TYPE_UNSIGNED_INT)
		glSamplerParameterIuiv(*pSampler, GL_TEXTURE_BORDER_COLOR, createInfo.borderColor.color.uint32);
	else if (createInfo.borderColor.dataType == DATA_TYPE_FLOAT)
		glSamplerParameterfv(*pSampler, GL_TEXTURE_BORDER_COLOR, createInfo.borderColor.color.float32);
}

//void createMemory(MemoryHandle *pMemory)
//{
//	/*
//		server memory, cannot be accessed directly
//	*/
//	glCreateMemoryObjectsEXT(1, pMemory);
//
//	int dedicatedMemory{}; //external memory
//	glMemoryObjectParameterivEXT(*pMemory, GL_DEDICATED_MEMORY_OBJECT_EXT, &dedicatedMemory);
//	int protectedMemory{}; //
//	glMemoryObjectParameterivEXT(*pMemory, GL_PROTECTED_MEMORY_OBJECT_EXT, &protectedMemory);
//}
SemaphoreHandle createSemaphore()
{
	SemaphoreHandle semaphore;
	glGenSemaphoresEXT(1, &semaphore);
	return semaphore;
}

GLenum findSupportedTilingType(const std::vector<ImageTiling> &candidateTilings, ImageType imageType, bool multisample, GLenum format)
{
	GLenum target = Map(imageType, multisample);

	int NUM_TILING_TYPES_EXT;
	glGetInternalformativ(target, format, GL_NUM_TILING_TYPES_EXT, 1, &NUM_TILING_TYPES_EXT);
	LOG_VAR(NUM_TILING_TYPES_EXT);

	std::vector<int> TILING_TYPES_EXT(NUM_TILING_TYPES_EXT);
	glGetInternalformativ(target, format, GL_TILING_TYPES_EXT, NUM_TILING_TYPES_EXT, TILING_TYPES_EXT.data());
	for (auto canditiling : candidateTilings)
	{
		for (GLenum tiling : TILING_TYPES_EXT)
		{
			if (Map(canditiling) == tiling)
			{
				LOG("support tiling type is:");
				LOG(tiling);
				return tiling;
			}
		}
	}
	throw std::runtime_error("failed to find supported tiling type");
}

void createImageView(const ImageViewCreateInfo &createInfo, bool multisample, ImageHandle *pImageViewHandle)
{
	GLenum target = Map(createInfo.viewType, multisample);
	glGenTextures(1, pImageViewHandle);
	glTextureView(*pImageViewHandle,
				  target,
				  createInfo.image,
				  createInfo.format,
				  createInfo.subresourceRange.baseMipLevel,
				  createInfo.subresourceRange.levelCount,
				  createInfo.subresourceRange.baseArrayLayer,
				  createInfo.subresourceRange.layerCount);
	glBindTexture(target, *pImageViewHandle);
	if (createInfo.subresourceRange.aspectMask & IMAGE_ASPECT_DEPTH_BIT)
		glTextureParameteri(target, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT);
	else if (createInfo.subresourceRange.aspectMask & IMAGE_ASPECT_STENCIL_BIT)
		glTextureParameteri(target, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_COMPONENTS);
	glBindTexture(target, 0);
}

void updateImageSubData(ImageHandle image, ImageType imageType, const ImageSubData &imageSubData)
{
	GLenum target = Map(imageType, false);
	glBindTexture(target, image);
	switch (imageType)
	{
	case IMAGE_TYPE_1D:
		glTexSubImage2D(
			target,
			imageSubData.mipLevel,
			imageSubData.rect.offset.x,
			imageSubData.rect.offset.y,
			imageSubData.rect.extent.width,
			imageSubData.rect.extent.height,
			imageSubData.format,
			Map(imageSubData.dataType),
			imageSubData.data);
		break;
	case IMAGE_TYPE_2D:
	case IMAGE_TYPE_3D:
		glTexSubImage3D(
			target,
			imageSubData.mipLevel,
			imageSubData.rect.offset.x,
			imageSubData.rect.offset.y,
			imageSubData.rect.offset.z,
			imageSubData.rect.extent.width,
			imageSubData.rect.extent.height,
			imageSubData.rect.extent.depth,
			imageSubData.format,
			Map(imageSubData.dataType),
			imageSubData.data);
		break;
	}
	glBindTexture(target, 0);
};

void setImageSampler(const SamplerCreateInfo &createInfo, ImageHandle image, ImageViewType imageViewType)
{
	//multisample texture do not support sampler parameter
	GLenum target = Map(imageViewType, false);
	glBindTexture(target, image);

	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, Map(createInfo.magFilter));
	if (createInfo.mipmapMode == SAMPLER_MIPMAP_MODE_LINEAR)
	{
		if (createInfo.minFilter == FILTER_LINEAR)
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	}
	else
	{
		if (createInfo.minFilter == FILTER_LINEAR)
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		else
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	glTexParameterf(target, GL_TEXTURE_LOD_BIAS, createInfo.mipLodBias);
	glTexParameterf(target, GL_TEXTURE_MIN_LOD, createInfo.minLod);
	glTexParameterf(target, GL_TEXTURE_MAX_LOD, createInfo.maxLod);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, Map(createInfo.wrapModeU));
	glTexParameteri(target, GL_TEXTURE_WRAP_T, Map(createInfo.wrapModeV));
	glTexParameteri(target, GL_TEXTURE_WRAP_R, Map(createInfo.wrapModeW));
	if (createInfo.compareEnable)
	{
		glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(target, GL_TEXTURE_COMPARE_FUNC, Map(createInfo.compareOp));
	}
	if (createInfo.borderColor.dataType == DATA_TYPE_INT)
		glTexParameterIiv(target, GL_TEXTURE_BORDER_COLOR, createInfo.borderColor.color.int32);
	else if (createInfo.borderColor.dataType == DATA_TYPE_UNSIGNED_INT)
		glTexParameterIuiv(target, GL_TEXTURE_BORDER_COLOR, createInfo.borderColor.color.uint32);
	else if (createInfo.borderColor.dataType == DATA_TYPE_FLOAT)
		glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, createInfo.borderColor.color.float32);
	glBindTexture(target, 0);
}
void createProgramPipeline(const GraphicsPipelineCreateInfo &createInfo, PipelineHandle *pPipeline)
{
	glGenProgramPipelines(1, pPipeline);

}