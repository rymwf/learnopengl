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
	// check opengl informations
	LOG_VAR(glGetString(GL_VENDOR));
	LOG_VAR(glGetString(GL_RENDERER));
	LOG_VAR(glGetString(GL_VERSION));
	LOG_VAR(glGetString(GL_SHADING_LANGUAGE_VERSION));

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
void createBuffer(const BufferCreateInfo &createInfo, BufferHandle *pBuffer)
{
	glGenBuffers(1, pBuffer);
	// target do not matter when creating buffer
	glBindBuffer(GL_ARRAY_BUFFER, *pBuffer);
	if (GLVersion.major * 10 + GLVersion.minor < 44)
		glBufferData(GL_ARRAY_BUFFER, createInfo.size, createInfo.pData,
					 GL_DYNAMIC_DRAW);
	else
		glBufferStorage(GL_ARRAY_BUFFER, createInfo.size, createInfo.pData,
						createInfo.storageFlags);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void createVertexArray(const VertexInputStateDescription &vertexInputDescription,
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