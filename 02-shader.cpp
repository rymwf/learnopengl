/**
 * @file 02-shader.cpp
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

const char *vertFile = SHADER_PATH "02.vert";
const char *fragFile = SHADER_PATH "02.frag";

class Hello
{
	GLFWwindow *window;
	ShaderHandle vertShader;
	ShaderHandle fragShader;
	ProgramHandle programId;
	VertexArrayHandle vertexArray;
	bool framebufferResized{true};

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
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			THROW("failed to init glew");
		}

		listGLInfo();

#ifndef NODEBUG
		EnableDebugOutput(this);
#endif
		createTestProgram();
		createVAO();

		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
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
		glDrawArrays(GL_TRIANGLES, 0, 3);
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
		if (GLEW_VERSION_4_5 && isSupportShaderBinaryFormat(SHADER_BINARY_FORMAT_SPIR_V))
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
				vertCode};

			ShaderCreateInfo fragShaderCreateInfo{
				SHADER_STAGE_FRAGMENT_BIT,
				fragCode};

			createShader(vertShaderCreateInfo, &vertShader);
			createShader(fragShaderCreateInfo, &fragShader);
		}
		createProgram({{vertShader, fragShader}}, &programId);
	}

	void createVAO()
	{
		glGenVertexArrays(1, &vertexArray);
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