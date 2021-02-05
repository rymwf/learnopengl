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

constexpr GLuint WIDTH = 800, HEIGHT = 600;

constexpr char *testImagePath = "./assets/textures/Lenna_test.jpg";

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
	ProgramHandle programId;
	VertexArrayHandle vertexArray;
	BufferHandle vertexBuffer;
	BufferHandle indexBuffer;
	BufferHandle drawIndirectCmdBuffer;
	BufferHandle drawIndirectCmdCountBuffer;
	BufferHandle drawIndexedIndirectCmdBuffer;
	BufferHandle drawIndexedIndirectCmdCountBuffer;
	BufferHandle uboMVPBuffer;

	ImageHandle testImage;
	ImageHandle testImageView;

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

		listGLInfo();

#ifndef NODEBUG
		EnableDebugOutput(this);
#endif

		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
		glEnable(GL_FRAMEBUFFER_SRGB);

		createTestProgram();
		createVertexBuffer();
		createIndexBuffer();
		createUboBuffer();
		createVAO();
		createDrawCommandBuffer();
		createTextureImage();
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
		glDeleteTextures(1, &testImage);
		glDeleteTextures(1, &testImageView);

		glDeleteBuffers(1, &uboMVPBuffer);
		glDeleteBuffers(1, &drawIndexedIndirectCmdCountBuffer);
		glDeleteBuffers(1, &drawIndexedIndirectCmdBuffer);
		glDeleteBuffers(1, &drawIndirectCmdCountBuffer);
		glDeleteBuffers(1, &drawIndirectCmdBuffer);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &indexBuffer);
		glDeleteVertexArrays(1, &vertexArray);
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		glDeleteProgram(programId);

		glfwDestroyWindow(window);
		glfwTerminate();
	}
	void drawFrame()
	{
		resize();
		glClear(GL_COLOR_BUFFER_BIT);
		updateUboBuffers();
		glUseProgram(programId);
		glBindVertexArray(vertexArray);
		glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboMVPBuffer, 0, sizeof(UBO_MVP));
		//glBindTexture(GL_TEXTURE_2D_ARRAY, testImage);
		glBindTexture(GL_TEXTURE_2D, testImageView);
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
			int width = 0, height = 0;
			glfwGetFramebufferSize(window, &width, &height);
			if (width == 0 || height == 0)
			{
				glfwGetFramebufferSize(window, &width, &height);
				glfwWaitEvents();
			}
			glViewport(0, 0, width, height);
			framebufferResized = false;
		}
	}

	static void framebufferResizeCallback(GLFWwindow *window, [[maybe_unused]] int width, [[maybe_unused]] int height)
	{
		auto app = static_cast<Hello *>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}

	void createTestProgram()
	{
		if (GLVersion.major * 10 + GLVersion.minor > 45 && isSupportShaderBinaryFormat(SHADER_BINARY_FORMAT_SPIR_V))
		{
			auto vertCode = readFile((std::string(vertFile) + ".spv").c_str());
			auto fragCode = readFile((std::string(fragFile) + ".spv").c_str());

			ShaderCreateInfo vertShaderCreateInfo{
				SHADER_STAGE_VERTEX_BIT,
				vertCode.size(),
				vertCode.data()};

			ShaderCreateInfo fragShaderCreateInfo{
				SHADER_STAGE_FRAGMENT_BIT,
				fragCode.size(),
				fragCode.data()};

			createShaderBinary(vertShaderCreateInfo, &vertShader);
			glSpecializeShader(vertShader, "main", 0, nullptr, nullptr);
			createShaderBinary(fragShaderCreateInfo, &fragShader);
			glSpecializeShader(fragShader, "main", 0, nullptr, nullptr);
		}
		else
		{
			auto vertCode = readFile(vertFile);
			auto fragCode = readFile(fragFile);
			ShaderCreateInfo vertShaderCreateInfo{
				SHADER_STAGE_VERTEX_BIT,
				vertCode.size(),
				vertCode.data()};

			ShaderCreateInfo fragShaderCreateInfo{
				SHADER_STAGE_FRAGMENT_BIT,
				fragCode.size(),
				fragCode.data()};

			createShader(vertShaderCreateInfo, &vertShader);
			createShader(fragShaderCreateInfo, &fragShader);
		}
		ShaderHandle shaders[] = {vertShader, fragShader};
		createProgram({2, shaders}, &programId);
	}

	void createVertexBuffer()
	{
		createBuffer({0, vertices.size() * sizeof(vertices[0]), 0}, vertices.data(), &vertexBuffer);
	}

	void createIndexBuffer()
	{
		createBuffer({0, indices.size() * sizeof(indices[0]), 0}, indices.data(), &indexBuffer);
	}
	void createVAO()
	{
		std::vector<VertexBindingDescription> bindingDescriptions{
			Vertex::getVertexBindingDescription(0)};

		auto attributeDescriptions0 = Vertex::getVertexAttributeDescription(0, 0);
		std::vector<VertexAttributeDescription> attributeDescriptions;
		attributeDescriptions.insert(attributeDescriptions.end(), attributeDescriptions0.begin(), attributeDescriptions0.end());

		VertexInputStateDescription vertexInputStateDescription{
			bindingDescriptions.size(),
			bindingDescriptions.data(),
			attributeDescriptions.size(),
			attributeDescriptions.data(),
		};
		createVertexArray(vertexInputStateDescription, {vertexBuffer}, indexBuffer, &vertexArray);
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
		createBuffer({0, sizeof(UBO_MVP), BUFFER_STORAGE_MAP_WRITE_BIT}, &uboMVP, &uboMVPBuffer);
		//createBuffer({BUFFER_CREATE_MUTABLE_FORMAT_BIT, sizeof(UBO_MVP),  BUFFER_MUTABLE_STORAGE_STREAM_DRAW}, &uboMVP,&uboMVPBuffer);
	}
	void updateUboBuffers()
	{
		//float time = std::chrono::duration<float, std::chrono::seconds::period>(curTime - startTime).count();
		//uboMVP.M = glm::rotate(glm::mat4(1), time * glm::radians(30.f), glm::vec3(0, 0, 1));

		//glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMVPBuffer, 0, sizeof(glm::mat4));
		//void *data = glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), GL_MAP_WRITE_BIT);
		//memcpy(data, &uboMVP.M, sizeof(uboMVP.M));
		//glUnmapBuffer(GL_UNIFORM_BUFFER);
	}
	void createTextureImage()
	{
		int width, height, components;
		auto pixels = stbi_load(testImagePath, &width, &height, &components, 4); //force load an alpha channel,even not exist
		if (!pixels)
			throw std::runtime_error("failed to load texture image!");

		ImageCreateInfo imageCreateInfo{
			0,
			IMAGE_TYPE_2D,
			GL_SRGB8_ALPHA8,
			{static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1},
			1,
			1,
			SAMPLE_COUNT_1_BIT};
		createImage(imageCreateInfo, &testImage);
		ImageSubData imageSubData{
			GL_RGBA,
			DATA_TYPE_UNSIGNED_BYTE,
			0,
			{{},
			 imageCreateInfo.extent},
			pixels};
		updateImageSubData(testImage, imageCreateInfo.imageType, imageCreateInfo.samples > SAMPLE_COUNT_1_BIT, imageSubData);
		ImageViewCreateInfo imageViewCreateInfo{
			testImage,
			IMAGE_VIEW_TYPE_2D,
			GL_SRGB8_ALPHA8,
			{},
			{0, 1, 0, 1},
		};
		createImageView(imageViewCreateInfo, &testImageView);

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