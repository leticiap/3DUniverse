#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}
	mouseFirstMoved = true;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}
	mouseFirstMoved = true;
}

int Window::Initialise()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core profile = no backwards compability
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow foward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Biffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set the context for GLFW to use
	glfwMakeContextCurrent(mainWindow);

	// Handle Key + Mouse Input
	CreateCallBacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Glew initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);
	return 0;
}


bool Window::GetShouldClose()
{
	return glfwWindowShouldClose(mainWindow);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(mainWindow);
}

void Window::SetKeyValue(int key, bool value)
{
	if (key >= 0 && key < 1024)
		keys[key] = value;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

void Window::CreateCallBacks()
{
	glfwSetKeyCallback(mainWindow, HandleKeys);
	glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

void Window::HandleKeys(GLFWwindow * window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->SetKeyValue(key, true);
			printf("Pressed %d\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->SetKeyValue(key, false);
			printf("Released %d\n", key);
		}
	}
}

void Window::HandleMouse(GLFWwindow * window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->IsFirstMouseMove())
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->SetMouseFirstMoved(false);
	}
	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}
