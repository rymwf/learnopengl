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
		if (GLEW_VERSION_4_3)
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
	if (glewIsSupported("GL_EXT_memory_object"))
	{
		int NUM_DEVICE_UUIDS_EXT;
		glGetIntegerv(GL_NUM_DEVICE_UUIDS_EXT, &NUM_DEVICE_UUIDS_EXT);
		LOG_VAR(NUM_DEVICE_UUIDS_EXT);
		GLint64 DEVICE_UUID_EXT{};
		for (int i = 0; i < NUM_DEVICE_UUIDS_EXT; ++i)
		{
			LOG(i);
			glGetInteger64i_v(GL_DEVICE_UUID_EXT, i, &DEVICE_UUID_EXT);
			LOG_VAR(DEVICE_UUID_EXT);
		}
		GLint64 DRIVER_UUID_EXT;
		glGetInteger64v(GL_DRIVER_UUID_EXT, &DRIVER_UUID_EXT);
		LOG_VAR(DRIVER_UUID_EXT);
	}

	// check opengl informations
	LOG_VAR(glGetString(GL_VENDOR));
	LOG_VAR(glGetString(GL_RENDERER));
	LOG_VAR(glGetString(GL_VERSION));
	LOG_VAR(glGetString(GL_SHADING_LANGUAGE_VERSION));

	int numShadingLanguageVersions{};
	glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &numShadingLanguageVersions);
	LOG_VAR(numShadingLanguageVersions);
	for (int i = 0; i < numShadingLanguageVersions; ++i)
		LOG_VAR(glGetStringi(GL_SHADING_LANGUAGE_VERSION, i));

	int samples;
	glGetIntegerv(GL_SAMPLES, &samples);
	LOG_VAR(samples);

	int maxFramebufferWidth{}, maxFramebufferHeight{};
	glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &maxFramebufferWidth);
	glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &maxFramebufferHeight);
	LOG_VAR(maxFramebufferWidth);
	LOG_VAR(maxFramebufferHeight);

	if (GLEW_VERSION_4_3)
	{
		//framebuffer parameter
		GLenum list0[]{
			//		GL_FRAMEBUFFER_DEFAULT_WIDTH,
			//		GL_FRAMEBUFFER_DEFAULT_HEIGHT,
			//		GL_FRAMEBUFFER_DEFAULT_LAYERS,
			//		GL_FRAMEBUFFER_DEFAULT_SAMPLES,
			//		GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS,

			GL_DOUBLEBUFFER,
			GL_IMPLEMENTATION_COLOR_READ_FORMAT,
			GL_IMPLEMENTATION_COLOR_READ_TYPE,
			GL_SAMPLES,
			GL_SAMPLE_BUFFERS,
			GL_STEREO};
		const char *list1[]{
			"GL_DOUBLEBUFFER",
			"GL_IMPLEMENTATION_COLOR_READ_FORMAT",
			"GL_IMPLEMENTATION_COLOR_READ_TYPE",
			"GL_SAMPLES",
			"GL_SAMPLE_BUFFERS",
			"GL_STEREO"};

		//default framebuffer
		for (int i = 0, len = sizeof list0 / sizeof list0[0]; i < len; ++i)
		{
			GLint a{};
			glGetFramebufferParameteriv(GL_FRAMEBUFFER, list0[i], &a);
			LOG(list1[i]);
			LOG(a);
		}
	}

	GLenum list2[]{
		GL_FRONT_LEFT,
		GL_FRONT_RIGHT,
		GL_BACK_LEFT,
		GL_BACK_RIGHT,
		GL_DEPTH,
		GL_STENCIL};
	const char *list3[]{
		"GL_FRONT_LEFT",
		"GL_FRONT_RIGHT",
		"GL_BACK_LEFT",
		"GL_BACK_RIGHT",
		"GL_DEPTH",
		"GL_STENCIL"};
	GLenum list4[]{
		GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE,
		GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE,
		GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE,
		GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE,
		GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE,
		GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE,
		GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE,
		GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING};
	const char *list5[]{
		"GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE",
		"GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE",
		"GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE",
		"GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE",
		"GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE",
		"GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE",
		"GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE",
		"GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING"};
	for (int i = 0; i < 6; ++i)
	{
		GLint a{};
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, list2[i], GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &a);
		LOG(list3[i]);
		LOG(a);
		if (a != GL_NONE)
		{
			for (int j = 0, len = sizeof list4 / sizeof list4[0]; j < len; ++j)
			{
				glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, list2[i], list4[j], &a);
				LOG(list5[j]);
				LOG(a);
			}
		}
	}

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
	const char *glCapabilityEnumNames[]{
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
	
	int a;
	glGetInternalformativ(GL_TEXTURE_2D_ARRAY, GL_BGR, GL_INTERNALFORMAT_SUPPORTED, 1, &a);
	LOG("GL_BGR supported:");
	LOG(a);
	glGetInternalformativ(GL_TEXTURE_2D_ARRAY, GL_BGR, GL_INTERNALFORMAT_PREFERRED, 1, &a);
	LOG("GL_BGR prefered:");
	LOG(bool(a == GL_BGR));

	constexpr GLenum aggregateLimits[]{
		GL_MIN_PROGRAM_TEXEL_OFFSET,
		GL_MAX_PROGRAM_TEXEL_OFFSET,
		GL_MAX_UNIFORM_BUFFER_BINDINGS,
		GL_MAX_UNIFORM_BLOCK_SIZE,
		GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT,
		GL_MAX_COMBINED_UNIFORM_BLOCKS,
		GL_MAX_VARYING_COMPONENTS,
		GL_MAX_VARYING_VECTORS,
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		GL_MAX_SUBROUTINES,
		GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS,
		GL_MAX_UNIFORM_LOCATIONS,
		GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS,
		GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE,
		GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS,
		GL_MAX_COMBINED_ATOMIC_COUNTERS,
		GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS,
		GL_MAX_SHADER_STORAGE_BLOCK_SIZE,
		GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS,
		GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT,
		GL_MAX_IMAGE_UNITS,
		GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES,
		GL_MAX_IMAGE_SAMPLES,
		GL_MAX_VERTEX_IMAGE_UNIFORMS,
		GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS,
		GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS,
		GL_MAX_GEOMETRY_IMAGE_UNIFORMS,
		GL_MAX_FRAGMENT_IMAGE_UNIFORMS,
		GL_MAX_COMBINED_IMAGE_UNIFORMS,
		GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS,
		GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS,
		GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS,
		GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS,
		GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS,
	};
	std::string aggregateLimitNames[]{
		"GL_MIN_PROGRAM_TEXEL_OFFSET",
		"GL_MAX_PROGRAM_TEXEL_OFFSET",
		"GL_MAX_UNIFORM_BUFFER_BINDINGS",
		"GL_MAX_UNIFORM_BLOCK_SIZE",
		"GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT",
		"GL_MAX_COMBINED_UNIFORM_BLOCKS",
		"GL_MAX_VARYING_COMPONENTS",
		"GL_MAX_VARYING_VECTORS",
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_SUBROUTINES",
		"GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS",
		"GL_MAX_UNIFORM_LOCATIONS",
		"GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS",
		"GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE",
		"GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS",
		"GL_MAX_COMBINED_ATOMIC_COUNTERS",
		"GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS",
		"GL_MAX_SHADER_STORAGE_BLOCK_SIZE",
		"GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS",
		"GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT",
		"GL_MAX_IMAGE_UNITS",
		"GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES",
		"GL_MAX_IMAGE_SAMPLES",
		"GL_MAX_VERTEX_IMAGE_UNIFORMS",
		"GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS",
		"GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS",
		"GL_MAX_GEOMETRY_IMAGE_UNIFORMS",
		"GL_MAX_FRAGMENT_IMAGE_UNIFORMS",
		"GL_MAX_COMBINED_IMAGE_UNIFORMS",
		"GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS",
		"GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS",
		"GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS",
		"GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS",
		"GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS",
	};		



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
		const char *code = createInfo.code.data();
		glShaderSource(*pShader, 1, &code, NULL);
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
		glShaderBinary(1, pShader, GL_SHADER_BINARY_FORMAT_SPIR_V, createInfo.code.data(), createInfo.code.size());
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
		for (auto shader : createInfo.shaders)
			glAttachShader(*pProgram, shader);
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

void createGraphicsPipeline(const GraphicsPipelineCreateInfo &createInfo, PipelineHandle *pPipeline, std::vector<ProgramHandle> *pPrograms)
{
	if (GLEW_VERSION_4_1)
	{
		glGenProgramPipelines(1, pPipeline);
	}
	else
	{
		throw std::runtime_error("failed to create pipelines");
	}
	glBindProgramPipeline(*pPipeline);
	for (auto &stageCreateInfo : createInfo.stages)
	{
		//equal to compilation
		glSpecializeShader(
			stageCreateInfo.shaderHandle,
			stageCreateInfo.entryName.data(),
			stageCreateInfo.specializationInfo.constantIDs.size(),
			stageCreateInfo.specializationInfo.constantIDs.data(),
			stageCreateInfo.specializationInfo.constantValues.data());

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
		pPrograms->emplace_back(0);
		createProgram({{stageCreateInfo.shaderHandle}, true}, &pPrograms->back());
		glUseProgramStages(*pPipeline, stageCreateInfo.stage, pPrograms->back());
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
void createVertexArray(const VertexInputStateCreateInfo &vertexInputStateCreateInfo,
					   const std::vector<BufferHandle> &vertexBuffers,
					   const BufferHandle indexBuffer,
					   VertexArrayHandle *pVertexArray)
{
	glGenVertexArrays(1, pVertexArray);
	if (*pVertexArray == 0)
		throw std::runtime_error("failed to create vertex array object");
	glBindVertexArray(*pVertexArray);

	for (auto &attrib : vertexInputStateCreateInfo.vertexAttributeDescriptions)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[attrib.binding]);
		glEnableVertexAttribArray(attrib.location);
		glVertexAttribPointer(
			attrib.location, attrib.components,
			Map(attrib.dataType), attrib.normalized,
			vertexInputStateCreateInfo.vertexBindingDescriptions[attrib.binding].stride,
			(void *)attrib.offset);
		glVertexAttribDivisor(attrib.location, vertexInputStateCreateInfo.vertexBindingDescriptions[attrib.binding].divisor);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBindVertexArray(0);
}
void createVertexArray2(const VertexInputStateCreateInfo &vertexInputStateCreateInfo, VertexArrayHandle &vao)
{
	glGenVertexArrays(1, &vao);
	if (vao == 0)
		THROW("failed to create vertex array object");
	glBindVertexArray(vao);
	for (auto &&attrib : vertexInputStateCreateInfo.vertexAttributeDescriptions)
	{
		glVertexAttribFormat(
			attrib.location,
			attrib.components,
			Map(attrib.dataType),
			attrib.normalized,
			attrib.offset);
		glVertexAttribBinding(attrib.location, attrib.binding);
		//glVertexBindingDivisor(attrib.location, vertexInputStateCreateInfo.vertexBindingDescriptions[attrib.binding].divisor);
		glVertexBindingDivisor(attrib.binding, vertexInputStateCreateInfo.vertexBindingDescriptions[attrib.binding].divisor);
		glEnableVertexAttribArray(attrib.location);
	}
}

void createImage(const ImageCreateInfo &createInfo, ImageHandle *pImage)
{
	if (createInfo.flags & IMAGE_CREATE_RENDER_BUFFER_BIT)
	{
		glGenRenderbuffers(1, pImage);
		glBindRenderbuffer(GL_RENDERBUFFER, *pImage);
		if (createInfo.samples > SAMPLE_COUNT_1_BIT)
		{
			glRenderbufferStorageMultisample(
				GL_RENDERBUFFER,
				createInfo.samples,
				createInfo.format,
				createInfo.extent.width,
				createInfo.extent.height);
		}
		else
		{
			glRenderbufferStorage(
				GL_RENDERBUFFER,
				createInfo.format,
				createInfo.extent.width,
				createInfo.extent.height);
		}
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
	else
	{
		glGenTextures(1, pImage);
		GLenum target = Map(createInfo.imageType, createInfo.samples > SAMPLE_COUNT_1_BIT);
		glBindTexture(target, *pImage);
		if (createInfo.flags & IMAGE_CREATE_MUTABLE_FORMAT_BIT)
		{
			if (createInfo.samples > SAMPLE_COUNT_1_BIT)
			{
				glTexImage3DMultisample(target,
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
					glTexImage2D(target,
								 0,
								 createInfo.format,
								 createInfo.extent.width,
								 createInfo.extent.height,
								 0,
								 GL_BGRA,
								 GL_UNSIGNED_BYTE,
								 nullptr);
					break;
				case IMAGE_TYPE_2D:
				case IMAGE_TYPE_3D:
					glTexImage3D(target,
								 0,
								 createInfo.format,
								 createInfo.extent.width,
								 createInfo.extent.height,
								 createInfo.extent.depth,
								 0,
								 GL_BGRA,
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

void createMemory(MemoryHandle &memory)
{
	/*
		server memory, cannot be accessed directly
	*/
	glCreateMemoryObjectsEXT(1, &memory);

	int dedicatedMemory{}; //external memory
	glMemoryObjectParameterivEXT(memory, GL_DEDICATED_MEMORY_OBJECT_EXT, &dedicatedMemory);
	int protectedMemory{}; //
	glMemoryObjectParameterivEXT(memory, GL_PROTECTED_MEMORY_OBJECT_EXT, &protectedMemory);
}
SemaphoreHandle createSemaphore()
{
	SemaphoreHandle semaphore;
	glGenSemaphoresEXT(1, &semaphore);
	return semaphore;
}

ImageTiling findSupportedTilingType(const std::vector<ImageTiling> &candidateTilings, ImageType imageType, bool multisample, GLenum format)
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
				return canditiling;
			}
		}
	}
	throw std::runtime_error("failed to find supported tiling type");
}

void createImageView(const ImageViewCreateInfo &createInfo, bool multisample, ImageViewHandle *pImageViewHandle)
{
	GLenum target = Map(createInfo.viewType, multisample);
	glGenTextures(1, pImageViewHandle);
	if (GLEW_VERSION_4_3)
	{
		glTextureView(*pImageViewHandle,
					  target,
					  createInfo.image,
					  createInfo.format,
					  createInfo.subresourceRange.baseMipLevel,
					  createInfo.subresourceRange.levelCount,
					  createInfo.subresourceRange.baseArrayLayer,
					  createInfo.subresourceRange.layerCount);
	}
	else if (glewIsSupported("GL_EXT_texture_view"))
	{
		glTextureViewEXT(*pImageViewHandle,
					  target,
					  createInfo.image,
					  createInfo.format,
					  createInfo.subresourceRange.baseMipLevel,
					  createInfo.subresourceRange.levelCount,
					  createInfo.subresourceRange.baseArrayLayer,
					  createInfo.subresourceRange.layerCount);
	}
	else
	{
		THROW("texture view not supported");
	}
	glBindTexture(target, *pImageViewHandle);
	if (createInfo.subresourceRange.aspectMask & IMAGE_ASPECT_DEPTH_BIT)
		glTexParameteri(target, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT);
	else if (createInfo.subresourceRange.aspectMask & IMAGE_ASPECT_STENCIL_BIT)
		glTexParameteri(target, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_COMPONENTS);

	GLint swizzles[4]{
		GL_RED,
		GL_GREEN,
		GL_BLUE,
		GL_ALPHA,
	};
	if (createInfo.components.r != COMPONENT_SWIZZLE_IDENTITY)
		swizzles[0] = Map(createInfo.components.r);
	if (createInfo.components.g != COMPONENT_SWIZZLE_IDENTITY)
		swizzles[1] = Map(createInfo.components.g);
	if (createInfo.components.b != COMPONENT_SWIZZLE_IDENTITY)
		swizzles[2] = Map(createInfo.components.b);
	if (createInfo.components.a != COMPONENT_SWIZZLE_IDENTITY)
		swizzles[3] = Map(createInfo.components.a);
	glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, swizzles);
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
void updateImageSubData2(ImageHandle image, ImageType imageType, const ImageSubData &imageSubData)
{
	GLenum target = Map(imageType, false);
	glBindTexture(target, image);

	glBindTexture(target, 0);
};

void setImageSampler(const SamplerCreateInfo &createInfo, ImageViewHandle image, ImageViewType imageViewType, bool multisample)
{
	if (multisample == true)
	{
		LOG("multisample texture do not support sampler parameter");
		return;
	}
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

void createDescriptorSetLayout(const DescriptorSetLayoutCreateInfo &createInfo, DescriptorSetLayout &outSetLayout)
{
	outSetLayout = new DescriptorSetLayout_T{createInfo};
}
void destroyDescriptorSetLayout(DescriptorSetLayout &setLayout)
{
	delete setLayout;
}

void createDescriptorSet(const std::vector<DescriptorSetLayout> &descriptorSetLayouts, DescriptorSet &descriptorSet)
{
	descriptorSet = new DescriptorSet_T{descriptorSetLayouts};
}

void destroyDescriptorSet(DescriptorSet descriptorSet)
{
	delete descriptorSet;
}

void updateDescriptorSets(const std::vector<WriteDescriptorSet> &descriptorWrites, const std::vector<CopyDescriptorSet> &descriptorCopy)
{
	for (auto &writeSet : descriptorWrites)
	{
		if (writeSet.descriptorType == DESCRIPTOR_TYPE_UNIFORM_BUFFER ||
			writeSet.descriptorType == DESCRIPTOR_TYPE_STORAGE_BUFFER)
		{
			for (auto &buffer : writeSet.dstSet->_buffers)
			{
				if (buffer.binding.binding == writeSet.dstBinding)
				{
					std::copy(writeSet.buffersInfo.begin(), writeSet.buffersInfo.end(), buffer.buffersInfo.begin() + writeSet.dstArrayElement);
					break;
				}
			}
		}
		else if (
			writeSet.descriptorType == DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ||
			writeSet.descriptorType == DESCRIPTOR_TYPE_SAMPLED_IMAGE ||
			writeSet.descriptorType == DESCRIPTOR_TYPE_STORAGE_IMAGE ||
			writeSet.descriptorType == DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER ||
			writeSet.descriptorType == DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER)
		{
			for (auto &image : writeSet.dstSet->_images)
			{
				if (image.binding.binding == writeSet.dstBinding)
				{
					for (int i = 0, len = writeSet.imagesInfo.size(); i < len; ++i)
					{
						setImageSampler(
							writeSet.imagesInfo[i].samplerCreateInfo,
							writeSet.imagesInfo[i].imageView,
							writeSet.viewType,
							writeSet.multisample);
						image.imageViews[i + writeSet.dstArrayElement] = writeSet.imagesInfo[i].imageView;
						image.imageViewType = writeSet.viewType;
						image.multisample = writeSet.multisample;
					}
					break;
				}
			}
		}
	}
	//for (auto &copySet : descriptorCopy)
	//{
	//}
}

void createFramebufferamer(const FramebufferCreateInfo &createInfo, FramebufferHandle &framebuffer)
{
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	const auto &attachmentDesc = *createInfo.renderpass->GetCreateInfoPtr()->spAttachments;
	for (int i = 0, len = attachmentDesc.size(); i < len; ++i)
	{
	}
}