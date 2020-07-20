#pragma once

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include <iostream>
extern int lastX;
extern int lastY;
static float zoom = 1.;
static float xoffset = 0;
static float yoffset = 0;




/* Printable keys */
#define TUGEV_KEY_SPACE              32
#define TUGEV_KEY_APOSTROPHE         39  /* ' */
#define TUGEV_KEY_COMMA              44  /* , */
#define TUGEV_KEY_MINUS              45  /* - */
#define TUGEV_KEY_PERIOD             46  /* . */
#define TUGEV_KEY_SLASH              47  /* / */
#define TUGEV_KEY_0                  48
#define TUGEV_KEY_1                  49
#define TUGEV_KEY_2                  50
#define TUGEV_KEY_3                  51
#define TUGEV_KEY_4                  52
#define TUGEV_KEY_5                  53
#define TUGEV_KEY_6                  54
#define TUGEV_KEY_7                  55
#define TUGEV_KEY_8                  56
#define TUGEV_KEY_9                  57
#define TUGEV_KEY_SEMICOLON          59  /* ; */
#define TUGEV_KEY_EQUAL              61  /* = */
#define TUGEV_KEY_A                  65
#define TUGEV_KEY_B                  66
#define TUGEV_KEY_C                  67
#define TUGEV_KEY_D                  68
#define TUGEV_KEY_E                  69
#define TUGEV_KEY_F                  70
#define TUGEV_KEY_G                  71
#define TUGEV_KEY_H                  72
#define TUGEV_KEY_I                  73
#define TUGEV_KEY_J                  74
#define TUGEV_KEY_K                  75
#define TUGEV_KEY_L                  76
#define TUGEV_KEY_M                  77
#define TUGEV_KEY_N                  78
#define TUGEV_KEY_O                  79
#define TUGEV_KEY_P                  80
#define TUGEV_KEY_Q                  81
#define TUGEV_KEY_R                  82
#define TUGEV_KEY_S                  83
#define TUGEV_KEY_T                  84
#define TUGEV_KEY_U                  85
#define TUGEV_KEY_V                  86
#define TUGEV_KEY_W                  87
#define TUGEV_KEY_X                  88
#define TUGEV_KEY_Y                  89
#define TUGEV_KEY_Z                  90
#define TUGEV_KEY_LEFT_BRACKET       91  /* [ */
#define TUGEV_KEY_BACKSLASH          92  /* \ */
#define TUGEV_KEY_RIGHT_BRACKET      93  /* ] */
#define TUGEV_KEY_GRAVE_ACCENT       96  /* ` */
#define TUGEV_KEY_WORLD_1            161 /* non-US #1 */
#define TUGEV_KEY_WORLD_2            162 /* non-US #2 */

#define MOUSE_PRESS				GLFW_PRESS
#define MOUSE_RELEASE			GLFW_RELEASE
  
#define    TUGEV_MOUSE_LEFT        GLFW_MOUSE_BUTTON_LEFT    
#define    TUGEV_MOUSE_RIGHT       GLFW_MOUSE_BUTTON_RIGHT   
#define    TUGEV_MOUSE_MIDDLE       GLFW_MOUSE_BUTTON_MIDDLE  

static bool cursorInitialized = false;




namespace TUGEV {

	enum class  MouseAction
	{
		rightClick = 0,
		rightRealease = 1
	};
	typedef struct tugevCursor {
		int x;
		int y;
	}CursorPos;

	class Mouse {
	public:
		Mouse();

		CursorPos getCursorPos();

		bool getMouseActionStatus(int button, int  action);
		float cursor_x;
		float cursor_y;
		GLFWwindow* glfw_window;
	};

	class InputManager {
	public:
		GLFWwindow* glfw_window;
		void updateInput();
		InputManager();
		bool  getKeyPressing(int key);
	};

}


static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!cursorInitialized) {
		lastX = 0;
		lastY = 0;
		cursorInitialized = !cursorInitialized;
		return;

	}
	lastX = xpos;
	lastY = ypos;

}
