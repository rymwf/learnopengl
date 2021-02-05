/**
 * @file 01-init.cpp
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

class Hello
{
	GLFWwindow *window;

	void initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

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
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	void drawFrame()
	{
		glClear(GL_COLOR_BUFFER_BIT);
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