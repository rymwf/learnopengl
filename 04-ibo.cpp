/**
 * @file 04-ibo.cpp
 * @author yangzs
 * @brief 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "common.h"

constexpr GLuint WIDTH = 800, HEIGHT = 600;

GLint glVersion{0}; //set glversion,such as 33 mean use version 33 , if 0, use latest

constexpr char *vertFile = SHADER_PATH "03.vert";
constexpr char *fragFile = SHADER_PATH "03.frag";

std::vector<Vertex> vertices{
	{{-0.5, -0.5, 0}, {1, 0, 0}},
	{{0.5, -0.5, 0}, {0, 1, 0}},
	{{-0.5, 0.5, 0}, {0, 0, 1}},
	{{0.5, 0.5, 0}, {1, 1, 0}},
};
std::vector<uint32_t> indices{
	0, 1, 2, 2, 1, 3};

std::vector<DrawIndirectCommand> drawIndirectCmds{{4, 1, 0, 0}};
std::vector<DrawIndexedIndirectCommand> drawIndexedIndirectCmds{{6, 1, 0, 0, 0}};

class Hello
{

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

	void initWindow()
	{
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

		LOG(GLVersion.major);
		LOG(GLVersion.minor);

		listGLInfo();

#ifndef NODEBUG
		EnableDebugOutput(this);
#endif

		glClearColor(0.2f, 0.2f, 0.2f, 1.f);

		createTestProgram();
		createVertexBuffer();
		createIndexBuffer();
		createVAO();
		createDrawCommandBuffer();
	}
	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			drawFrame();
			glfwSwapBuffers(window);
		}
	}
	void cleanup()
	{
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
		glUseProgram(programId);
		glBindVertexArray(vertexArray);
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
			glBindBuffer(GL_PARAMETER_BUFFER, drawIndexedIndirectCmdCountBuffer);
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
		createBuffer({vertices.size() * sizeof(vertices[0]), vertices.data(), 0}, &vertexBuffer);
	}

	void createIndexBuffer()
	{
		createBuffer({indices.size() * sizeof(indices[0]), indices.data(), 0}, &indexBuffer);
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
		createBuffer({sizeof(drawIndirectCmds[0]) * drawIndirectCmds.size(), drawIndirectCmds.data()}, &drawIndirectCmdBuffer);
		uint32_t count = drawIndirectCmds.size();
		createBuffer({4, &count}, &drawIndirectCmdCountBuffer);

		createBuffer({sizeof(drawIndexedIndirectCmds[0]) * drawIndexedIndirectCmds.size(), drawIndexedIndirectCmds.data()}, &drawIndexedIndirectCmdBuffer);
		count = drawIndexedIndirectCmds.size();
		createBuffer({4, &count}, &drawIndexedIndirectCmdCountBuffer);
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