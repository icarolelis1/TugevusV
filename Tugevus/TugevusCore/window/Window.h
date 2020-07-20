#pragma once

#include "Render.h"

#include "Input/Input.h"
#ifndef WINDOW
#define WINDOW
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <iostream>
#include <assert.h>
#include <sstream> 

struct WindowProps {
	unsigned int height; unsigned int width;
	GLFWmonitor* monitor;
	bool windowed;
	bool fullscreen;
	
};


class Window
{
public:

	GLFWwindow* glfw_window = nullptr;;

	Window(unsigned int height, int unsigned width, const char* title);
	//void handleKeyInputs();
	std::shared_ptr<Render> render;
	static TUGEV::InputManager	inputManager;
	static TUGEV::Mouse mouse;
	//void mousePressed();	
	void update();



private:
	WindowProps windowProperties; 
	const char* title;

//	std::unique_ptr<TUGEV::InputManager> inputManager;



	void initiateResources();
	//void minimizeWindow();
	//void resizeWindow();
	void destroy();

	//void setWindowCallBack();
};

#endif