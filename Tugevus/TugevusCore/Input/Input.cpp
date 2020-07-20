#include "Input/Input.h"

int lastX;
int lastY;
void TUGEV::InputManager::updateInput()
{
}

TUGEV::InputManager::InputManager()
{
}

bool TUGEV::InputManager::getKeyPressing(int key)
{
	if (glfwGetKey(glfw_window, key) == GLFW_PRESS) {
		return true;
	}
}

TUGEV::Mouse::Mouse()
{
}

TUGEV::CursorPos TUGEV::Mouse::getCursorPos()
{
	cursor_x = lastX;
	cursor_y = lastY;
	TUGEV::CursorPos c;
	c.x = cursor_x;
	c.y = cursor_y;

	return c;
}

bool TUGEV::Mouse::getMouseActionStatus(int button , int  action)
{
	
	return (glfwGetMouseButton(glfw_window, button) == action);
}


