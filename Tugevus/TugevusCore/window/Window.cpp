#include "Window.h"

TUGEV::InputManager Window::inputManager;
TUGEV::Mouse Window::mouse;

Window::Window(unsigned int height, int unsigned width, const char* _title): title(_title)
{
	 glfwInit();
	 glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	 glfwWindowHint(GLFW_RESIZABLE, 0);

	 windowProperties.height = height;
	 windowProperties.width = width;	
	 glfw_window = glfwCreateWindow(windowProperties.width,windowProperties.height, title, nullptr, nullptr);
	 glfwMakeContextCurrent(glfw_window);

	 assert(glfw_window != nullptr);
	 glfwSetCursorPosCallback(glfw_window, mouse_callback);
	 glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); //input mode
	glfwSetWindowUserPointer(glfw_window, this);

	initiateResources();

}

//Not in Use
/*
void Window::handleKeyInputs()
{

}

//Not in Use

void Window::mousePressed()
{
	
}

*/
void Window::update()
{
	


	while (!glfwWindowShouldClose(glfw_window)) {
		std::stringstream ss;
		ss << title << " " <<   " [" << render->frameTime << " MILLISEC]";

		glfwSetWindowTitle(glfw_window, ss.str().c_str());

		glfwPollEvents();
		render->draw();
	}

	//Render will be destroyed
	render->waitIdle();
}

void Window::initiateResources()
{

	//inputManager = std::make_unique<TUGEV::InputManager>(glfw_window);
	render = std::make_shared<Render>("Tugevus Render",glfw_window);


}

void Window::destroy()
{
}

//ImGui glfw already set these callbacks...
/*
void Window::setWindowCallBack()
{
	return;
	glfwSetCursorPosCallback(glfw_window, mouse_callback);

	auto f = [](GLFWwindow* w, int key, int scancode, int action, int mods) {
		static_cast<Window*>(glfwGetWindowUserPointer(w))->handleKeyInputs();
	
	};
	auto g = [](GLFWwindow* w, int button, int action, int mods) {
		static_cast<Window*>(glfwGetWindowUserPointer(w))->handleKeyInputs();

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			std::cout << "Clicking\n";
		}

	};

	//glfwSetKeyCallback(glfw_window, f);
	//glfwSetMouseButtonCallback(glfw_window, g);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}*/