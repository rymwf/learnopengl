/**
 * @file 05-ubo.cpp
 * @author yangzs
 * @brief 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "common.h"

#pragma warning(disable : 5219)
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#pragma warning(default : 5219)

int WIDTH = 800, HEIGHT = 600;

constexpr char *testImagePath = "./assets/textures/Lenna_test.jpg";

SampleCountFlagBits samples = SAMPLE_COUNT_1_BIT;

GLint glVersion{0}; //set glversion,such as 33 mean use version 33 , if 0, use latest

constexpr char *vertFile = SHADER_PATH "06.vert";
constexpr char *fragFile = SHADER_PATH "06.frag";

//std140, round to base alignment of vec4
struct UBO_MVP
{
	alignas(16) glm::mat4 M;
	alignas(16) glm::mat4 V;
	alignas(16) glm::mat4 P;
};

UBO_MVP uboMVP{{
				   {2, 0, 0, 0},
				   {0, 2, 0, 0},
				   {0, 0, 1, 0},
				   {0, 0, 0, 1},
			   },
			   glm::mat4(1),
			   glm::mat4(1)};

std::vector<Vertex> vertices{
	{{-0.5, -0.5, 0}, {1, 0, 0}, {0, 1}},
	{{0.5, -0.5, 0}, {0, 1, 0}, {1, 1}},
	{{-0.5, 0.5, 0}, {0, 0, 1}, {0, 0}},
	{{0.5, 0.5, 0}, {1, 1, 0}, {1, 0}},
};
std::vector<uint32_t> indices{
	0, 1, 2, 2, 1, 3};

std::vector<DrawIndirectCommand> drawIndirectCmds{{4, 1, 0, 0}};
std::vector<DrawIndexedIndirectCommand> drawIndexedIndirectCmds{{6, 1, 0, 0, 0}};

class Hello
{

	uint32_t FPS;
	float frameTimeInterval_ms;
	std::chrono::system_clock::time_point curTime;
	std::chrono::system_clock::time_point startTime;

	GLFWwindow *window;
	bool framebufferResized{true};

	ShaderHandle vertShader;
	ShaderHandle fragShader;
	std::vector<ProgramHandle> programs;
	PipelineHandle pipeline;

	VertexArrayHandle vertexArray;
	BufferHandle vertexBuffer;
	BufferHandle indexBuffer;
	BufferHandle drawIndirectCmdBuffer;
	BufferHandle drawIndirectCmdCountBuffer;
	BufferHandle drawIndexedIndirectCmdBuffer;
	BufferHandle drawIndexedIndirectCmdCountBuffer;
	BufferHandle uboMVPBuffer;

	ImageHandle testImage;
	ImageHandle testImageView0;
	ImageHandle testImageView1;
	SamplerHandle samplers[4];

	DescriptorSetLayout descriptorSetLayout;
	DescriptorSet descriptorSet;

	void initWindow()
	{
		startTime = std::chrono::system_clock::now();
		glfwInit();

		//set gl versiont
		if (glVersion > 0)
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersion / 10);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersion % 10);
		}
#ifndef NDEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
		glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);
		glfwWindowHint(GLFW_SAMPLES, 16);

		window = glfwCreateWindow(WIDTH, HEIGHT, __FILE__, nullptr, nullptr);
		if (!window)
		{
			glfwTerminate();
			throw std::runtime_error("failed to create window");
		}
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
		glfwSwapInterval(1); //enable v sync
	}
	void initOpengl()
	{
		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(reinterpret_cast<uintptr_t>(glfwGetProcAddress))))
			throw std::runtime_error("failed to load glad");

#ifndef NODEBUG
		EnableDebugOutput(this);
#endif

		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
		glEnable(GL_FRAMEBUFFER_SRGB);
		glEnable(GL_MULTISAMPLE);

		listGLInfo();

		createVertexBuffer();
		createIndexBuffer();
		createUboBuffer();
		createPipelines();
		createDrawCommandBuffer();
		createTextureImage();
		createDescriptorSets();
	}
	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			auto temptime = std::chrono::system_clock::now();
			frameTimeInterval_ms = std::chrono::duration<float, std::chrono::milliseconds::period>(temptime - curTime).count();
			curTime = temptime;
			FPS = static_cast<uint32_t>(1000 / frameTimeInterval_ms);

			glfwPollEvents();
			drawFrame();
			glfwSwapBuffers(window);
		}
	}
	void cleanup()
	{
		destroyDescriptorSetLayout(descriptorSetLayout);
		destroyDescriptorSet(descriptorSet);

		glDeleteTextures(1, &testImage);
		glDeleteTextures(1, &testImageView0);
		glDeleteTextures(1, &testImageView1);

		glDeleteBuffers(1, &uboMVPBuffer);
		glDeleteBuffers(1, &drawIndexedIndirectCmdCountBuffer);
		glDeleteBuffers(1, &drawIndexedIndirectCmdBuffer);
		glDeleteBuffers(1, &drawIndirectCmdCountBuffer);
		glDeleteBuffers(1, &drawIndirectCmdBuffer);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &indexBuffer);
		glDeleteVertexArrays(1, &vertexArray);

		glDeleteProgramPipelines(1, &pipeline);
		for (auto e : programs)
			glDeleteProgram(e);

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		glfwDestroyWindow(window);
		glfwTerminate();
	}
	void drawFrame()
	{
		resize();
		glClear(GL_COLOR_BUFFER_BIT);
		updateUboBuffers();
		glBindProgramPipeline(pipeline);
		glBindVertexArray(vertexArray);
		descriptorSet->Bind();
		//glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboMVPBuffer, 0, sizeof(UBO_MVP));
		//glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboMVPBuffer, 0, 1);

		//glBindTexture(GL_TEXTURE_2D_ARRAY, testImage);
		//if (samples > SAMPLE_COUNT_1_BIT)
		//	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, testImageView0);
		//else
		//	glBindTexture(GL_TEXTURE_2D, testImageView0);
		//glBindTexture(GL_TEXTURE_2D, testImageView1);

		//1
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		//2
		//glDrawArraysIndirect(GL_TRIANGLE_STRIP, &drawIndirectCmd);
		//3
		//glBindBuffer(GL_DRAW_INDIRECT_BUFFER, drawIndirectCmdBuffer);
		//glDrawArraysIndirect(GL_TRIANGLE_STRIP, NULL);
		//4
		//		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, drawIndirectCmdBuffer);
		//		if(GLVersion.major*10+GLVersion.minor>=43)
		//		{
		//			glMultiDrawArraysIndirect(GL_TRIANGLE_STRIP, NULL, drawIndirectCmds.size(), sizeof(DrawIndirectCommand));
		//		}else{
		//#ifdef GL_AMD_multi_draw_indirect
		//			glMultiDrawArraysIndirectAMD(GL_TRIANGLE_STRIP, NULL, drawIndirectCmds.size(), sizeof(DrawIndirectCommand));
		//#endif
		//		}
		//5
		//		if (GLVersion.major * 10 + GLVersion.minor >= 46)
		//		{
		//			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, drawIndirectCmdBuffer);
		//			glBindBuffer(GL_PARAMETER_BUFFER, drawIndirectCmdCountBuffer);
		//			glMultiDrawArraysIndirectCount(GL_TRIANGLE_STRIP, NULL, 0, drawIndirectCmds.size(), sizeof(DrawIndirectCommand)); //disgusting
		//		}
		//		else
		//		{
		//#ifdef GL_ARB_indirect_parameters
		//			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, drawIndirectCmdBuffer);
		//			glBindBuffer(GL_PARAMETER_BUFFER_ARB, drawIndirectCmdCountBuffer);
		//			glMultiDrawArraysIndirectCountARB(GL_TRIANGLE_STRIP, NULL, 0, drawIndirectCmds.size(), sizeof(DrawIndirectCommand)); //disgusting
		//#endif
		//		}

		//draw index

		//1
		//glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
		//2
		//glDrawElementsIndirect(GL_TRIANGLES,GL_UNSIGNED_INT,&drawIndexedIndirectCmd);
		//3
		//glBindBuffer(GL_DRAW_INDIRECT_BUFFER, drawIndexedIndirectCmdBuffer);
		//glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, NULL);
		//4
		//		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, drawIndexedIndirectCmdBuffer);
		//		if (GLVersion.major * 10 + GLVersion.minor >= 43)
		//		{
		//			glMultiDrawElementsIndirect(GL_TRIANGLE_STRIP, GL_UNSIGNED_INT, NULL, drawIndexedIndirectCmds.size(), sizeof(DrawIndexedIndirectCommand));
		//		}
		//		else
		//		{
		//#ifdef GL_AMD_multi_draw_indirect
		//			glMultiDrawElementsIndirectAMD(GL_TRIANGLE_STRIP, GL_UNSIGNED_INT, NULL, drawIndexedIndirectCmds.size(), sizeof(DrawIndexedIndirectCommand));
		//#endif
		//		}
		//5
		if (GLVersion.major * 10 + GLVersion.minor >= 46)
		{
			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, drawIndexedIndirectCmdBuffer);
			glBindBuffer(GL_PARAMETER_BUFFER, drawIndexedIndirectCmdCountBuffer);
			glMultiDrawElementsIndirectCount(GL_TRIANGLE_STRIP, GL_UNSIGNED_INT, NULL, 0, drawIndexedIndirectCmds.size(), sizeof(DrawIndexedIndirectCommand));
		}
		else
		{
#ifdef GL_ARB_indirect_parameters
			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, drawIndexedIndirectCmdBuffer);
			glBindBuffer(GL_PARAMETER_BUFFER_ARB, drawIndexedIndirectCmdCountBuffer);
			glMultiDrawElementsIndirectCountARB(GL_TRIANGLE_STRIP, GL_UNSIGNED_INT, NULL, 0, drawIndexedIndirectCmds.size(), sizeof(DrawIndexedIndirectCommand));
#endif
		}
	}

	void resize()
	{
		if (framebufferResized)
		{
			//int width = 0, height = 0;
			glfwGetFramebufferSize(window, &WIDTH, &HEIGHT);
			if (WIDTH == 0 || HEIGHT == 0)
			{
				glfwGetFramebufferSize(window, &WIDTH, &HEIGHT);
				glfwWaitEvents();
			}
			glViewport(0, 0, WIDTH, HEIGHT);
			framebufferResized = false;

			uboMVP.P = glm::perspective(glm::radians(45.f), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 1.f, 10.f);
			glBindBuffer(GL_UNIFORM_BUFFER, uboMVPBuffer);
			void *data = glMapBufferRange(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::mat4), GL_MAP_WRITE_BIT);
			memcpy(data, &uboMVP.P, sizeof(glm::mat4));
			glUnmapBuffer(GL_UNIFORM_BUFFER);
		}
	}

	static void framebufferResizeCallback(GLFWwindow *window, [[maybe_unused]] int width, [[maybe_unused]] int height)
	{
		auto app = static_cast<Hello *>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}

	void createPipelines()
	{
		auto vertCode = readFile((std::string(vertFile) + ".spv").c_str());
		auto fragCode = readFile((std::string(fragFile) + ".spv").c_str());

		ShaderCreateInfo vertShaderCreateInfo{
			SHADER_STAGE_VERTEX_BIT,
			vertCode};

		ShaderCreateInfo fragShaderCreateInfo{
			SHADER_STAGE_FRAGMENT_BIT,
			fragCode};

		createShaderBinary(vertShaderCreateInfo, &vertShader);
		createShaderBinary(fragShaderCreateInfo, &fragShader);

		SpecializationInfo vertSpecializationInfo{};
		SpecializationInfo fragSpecializationInfo{};
		std::vector<PipelineShaderStageCreateInfo> shaderStageCreateInfos{
			{SHADER_STAGE_VERTEX_BIT,
			 vertShader,
			 "main",
			 vertSpecializationInfo},
			{SHADER_STAGE_FRAGMENT_BIT,
			 fragShader,
			 "main",
			 fragSpecializationInfo},
		};

		std::vector<VertexBindingDescription> bindingDescriptions{
			Vertex::getVertexBindingDescription(0)};

		auto attributeDescriptions0 = Vertex::getVertexAttributeDescription(0, 0);
		std::vector<VertexAttributeDescription> attributeDescriptions;
		attributeDescriptions.insert(attributeDescriptions.end(), attributeDescriptions0.begin(), attributeDescriptions0.end());

		VertexInputStateCreateInfo vertexInputStateDescription{
			std::move(bindingDescriptions),
			std::move(attributeDescriptions)};

		GraphicsPipelineCreateInfo pipelineCreateInfo{
			std::move(shaderStageCreateInfos),
			std::move(vertexInputStateDescription)};

		createGraphicsPipeline(pipelineCreateInfo, &pipeline, &programs);

		//create vao
		createVertexArray(pipelineCreateInfo.vertexInputState, {vertexBuffer}, indexBuffer, &vertexArray);
	}
	void createDescriptorSets()
	{
		DescriptorSetLayoutCreateInfo setLayoutCreateInfo{{
			{
				1,
				DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				1,
			},
			{
				1,
				DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				1,
			},
		}};
		createDescriptorSetLayout(setLayoutCreateInfo, descriptorSetLayout);

		createDescriptorSet({descriptorSetLayout}, descriptorSet);

		SamplerCreateInfo samplerCreateInfo{
			FILTER_NEAREST,
			FILTER_NEAREST,
			SAMPLER_MIPMAP_MODE_NEAREST,
			SAMPLER_WRAP_MODE_REPEAT,
			SAMPLER_WRAP_MODE_REPEAT,
			SAMPLER_WRAP_MODE_REPEAT,
			0,
		};
		std::vector<WriteDescriptorSet> descriptorWrites{
			{
				descriptorSet,
				1,
				0,
				DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				{{samplerCreateInfo,
				  testImageView0}},
				IMAGE_VIEW_TYPE_2D,
				false,
			},
			{
				descriptorSet,
				1,
				0,
				DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				{},
				IMAGE_VIEW_TYPE_2D,
				0,
				{{uboMVPBuffer,
				  0,
				  static_cast<uint32_t>(sizeof(UBO_MVP))}},
			},
		};
		updateDescriptorSets(descriptorWrites, {});
	}

	void createVertexBuffer()
	{
		createBuffer({0, vertices.size() * sizeof(vertices[0]), 0}, vertices.data(), &vertexBuffer);
	}

	void createIndexBuffer()
	{
		createBuffer({0, indices.size() * sizeof(indices[0]), 0}, indices.data(), &indexBuffer);
	}

	void createDrawCommandBuffer()
	{
		createBuffer({0, sizeof(drawIndirectCmds[0]) * drawIndirectCmds.size()}, drawIndirectCmds.data(), &drawIndirectCmdBuffer);
		uint32_t count = drawIndirectCmds.size();
		createBuffer({0, 4}, &count, &drawIndirectCmdCountBuffer);

		createBuffer({0, sizeof(drawIndexedIndirectCmds[0]) * drawIndexedIndirectCmds.size()}, drawIndexedIndirectCmds.data(), &drawIndexedIndirectCmdBuffer);
		count = drawIndexedIndirectCmds.size();
		createBuffer({0, 4}, &count, &drawIndexedIndirectCmdCountBuffer);
	}

	void createUboBuffer()
	{
		createBuffer({0, sizeof(UBO_MVP), BUFFER_STORAGE_MAP_WRITE_BIT}, nullptr, &uboMVPBuffer);

		uboMVP = {
			glm::rotate(glm::mat4(1), glm::radians(20.f), glm::vec3(0, 0, 1)),
			glm::lookAt(glm::vec3(0, 0, 2), glm::vec3(0), glm::vec3(0, 1, 0)),
			glm::perspective(glm::radians(45.f), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 1.f, 10.f)};
		glBindBuffer(GL_UNIFORM_BUFFER, uboMVPBuffer);
		void *data = glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(UBO_MVP), GL_MAP_WRITE_BIT);
		memcpy(data, &uboMVP, sizeof(UBO_MVP));
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	}
	void updateUboBuffers()
	{
		float time = std::chrono::duration<float, std::chrono::seconds::period>(curTime - startTime).count();
		uboMVP.M = glm::rotate(glm::mat4(1), time * glm::radians(30.f), glm::vec3(0, 0, 1));

		glBindBuffer(GL_UNIFORM_BUFFER, uboMVPBuffer);
		void *data = glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), GL_MAP_WRITE_BIT);
		memcpy(data, &uboMVP.M, sizeof(uboMVP.M));
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	}
	void createTextureImage()
	{
		int width, height, components;
		auto pixels = stbi_load(testImagePath, &width, &height, &components, 4); //force load an alpha channel,even not exist
		if (!pixels)
			throw std::runtime_error("failed to load texture image!");

		uint32_t mipmapLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height))) + 1);

		ImageCreateInfo imageCreateInfo{
			0,
			IMAGE_TYPE_2D,
			GL_SRGB8_ALPHA8,
			{static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1},
			mipmapLevels,
			1,
			samples};
		createImage(imageCreateInfo, &testImage);
		ImageSubData imageSubData{
			GL_RGBA,
			DATA_TYPE_UNSIGNED_BYTE,
			0,
			{{},
			 imageCreateInfo.extent},
			pixels};

		if (imageCreateInfo.samples <= SAMPLE_COUNT_1_BIT)
		{
			updateImageSubData(testImage, imageCreateInfo.imageType, imageSubData);

			GLenum target = Map(IMAGE_TYPE_2D, imageCreateInfo.samples > SAMPLE_COUNT_1_BIT);
			glBindTexture(target, testImage);
			glGenerateMipmap(target);
			glBindTexture(target, 0);
		}

		ImageViewCreateInfo imageViewCreateInfo{
			testImage,
			IMAGE_VIEW_TYPE_2D,
			GL_SRGB8_ALPHA8,
			{},
			{IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1},
		};
		createImageView(imageViewCreateInfo, imageCreateInfo.samples > SAMPLE_COUNT_1_BIT, &testImageView0);
		imageViewCreateInfo.format = GL_RGBA8;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		createImageView(imageViewCreateInfo, imageCreateInfo.samples > SAMPLE_COUNT_1_BIT, &testImageView1);

		stbi_image_free(pixels);
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