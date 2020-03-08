#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>

#define MAX_KEYS 1024
#define MAX_MOUSE 24

#include "config.h"

class Window
{
private:
	uint32_t windowWidth = 1920, windowHeight = 1080;
	double time = glfwGetTime();
	double frame = 0;
	double prevFrame = 0;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static bool keys[MAX_KEYS];

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static bool mouse[MAX_MOUSE];

	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static double mx, my;//mouse pos

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static double msy;


	GLFWwindow* glfwWindow;

public:
	GLFWwindow* getWindow() { return glfwWindow; }
	uint32_t getWidth() { return windowWidth; }
	uint32_t getHeight() { return windowHeight; }
	Window();
	Window(config* _configPtr);
	~Window();
	uint32_t isKeyPressed(uint32_t keyCode);
	uint32_t isMousePressed(uint32_t mouseCode);
	static void getMousePos(double& x, double& y);
	static void getMouseScroll(double& y);
	float getfps();
	void centreMouse();
};

