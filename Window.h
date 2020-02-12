#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;

	Window();

	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLfloat GetBufferWidth() { return (GLfloat) bufferWidth; };
	GLfloat GetBufferHeight() { return (GLfloat) bufferHeight; };

	bool GetShouldClose();

	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	void SwapBuffers();

	void SetKeyValue(int key, bool value);
	bool IsFirstMouseMove() { return mouseFirstMoved; };
	void SetMouseFirstMoved(bool value) { mouseFirstMoved = value; };

	~Window();

private:
	GLFWwindow * mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	bool mouseFirstMoved;

	void CreateCallBacks();
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);
};

