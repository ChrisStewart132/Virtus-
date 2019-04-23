#include "Window.h"

bool Window::keys[MAX_KEYS];
bool Window::mouse[MAX_MOUSE];
double Window::mx;
double Window::my;
double Window::msy;

Window::Window() {}

Window::Window(config* _configPtr)
{
	windowWidth = _configPtr->windowWidth;
	windowHeight = _configPtr->windowHeight;

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindow = glfwCreateWindow(windowWidth, windowHeight, "Virtus", nullptr, nullptr);

	glfwSetKeyCallback(glfwWindow, key_callback);
	glfwSetMouseButtonCallback(glfwWindow, mouse_button_callback);
	glfwSetCursorPosCallback(glfwWindow, cursor_position_callback);
	glfwSetScrollCallback(glfwWindow, scroll_callback);
	glfwSetInputMode(glfwWindow, GLFW_STICKY_KEYS, 1);
}


Window::~Window()
{
	
}



void Window::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		keys[key] = true;
	}
	if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}
}

uint32_t Window::isKeyPressed(uint32_t keyCode)
{
	return keys[keyCode];
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		mouse[button] = true;
	}
	if (action == GLFW_RELEASE)
	{
		mouse[button] = false;
	}
}

uint32_t Window::isMousePressed(uint32_t mouseCode)
{
	return mouse[mouseCode];
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	mx = xpos;
	my = ypos;
}

void Window::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	msy = yoffset;
}

void Window::getMousePos(double& x, double& y)
{
	x = mx;
	y = my;
}

void Window::getMouseScroll(double& y)
{
	y = msy;
}

float Window::getfps()
{
	double ctime = glfwGetTime();
	frame++;
	if (ctime - time >= 1.0)
	{
		time += 1.0f;
		//std::cout << "fps: " << frame << std::endl;
		prevFrame = frame;	
		frame = 0;
		return prevFrame;
	}
	ctime = 0;
	return prevFrame;
}
