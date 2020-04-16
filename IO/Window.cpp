//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#include "Window.h"
#include "UI.h"

#include "../Console.h"
#include "../Constants.h"
#include "../Configuration.h"
#include "../Timer.h"

#include "../Graphics/GraphicsGL.h"

#define STB_IMAGE_IMPLEMENTATION
// TODO: (rich) is needed?
//#include <stb_image.h>
#ifdef WIN32
#include <Windows.h>
#else
#include "stb_image.h"
#endif

#ifdef __SWITCH__
#include <switch.h>
#endif

namespace ms
{
	Window::Window()
	{
		context = nullptr;
		glwnd = nullptr;
		opacity = 1.0f;
		opcstep = 0.0f;
		width = Constants::Constants::get().get_viewwidth();
		height = Constants::Constants::get().get_viewheight();
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	void error_callback(int no, const char* description)
	{
		Console::get().print("glfw error: " + std::string(description) + " (" + std::to_string(no) + ")");
	}

	void key_callback(GLFWwindow*, int key, int, int action, int)
	{
		UI::get().send_key(key, action != GLFW_RELEASE);
	}

	std::chrono::time_point<std::chrono::steady_clock> start = ContinuousTimer::get().start();

#ifdef __SWITCH__
	void handleTouch() {
        u32 touches = hidTouchCount();
        touchPosition currentTouch;
        if (touches == 0)
            UI::get().send_cursor(false);
        if (touches == 1) {
            hidTouchRead(&currentTouch, 0);
            UI::get().send_cursor(Point<int16_t>(currentTouch.px, currentTouch.py));
            UI::get().send_cursor(true);
        }
	}
#endif

	void mousekey_callback(GLFWwindow*, int button, int action, int)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			switch (action)
			{
			case GLFW_PRESS:
				UI::get().send_cursor(true);
				break;
			case GLFW_RELEASE:
			{
				auto diff_ms = ContinuousTimer::get().stop(start) / 1000;
				start = ContinuousTimer::get().start();

				if (diff_ms > 10 && diff_ms < 200)
					UI::get().doubleclick();

				UI::get().send_cursor(false);
			}
			break;
			}

			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			switch (action)
			{
			case GLFW_PRESS:
				UI::get().rightclick();
				break;
			}

			break;
		}
	}

	void cursor_callback(GLFWwindow*, double xpos, double ypos)
	{
		int16_t x = static_cast<int16_t>(xpos);
		int16_t y = static_cast<int16_t>(ypos);
		Point<int16_t> pos = Point<int16_t>(x, y);
		UI::get().send_cursor(pos);
	}

	void focus_callback(GLFWwindow*, int focused)
	{
		UI::get().send_focus(focused);
	}

	void scroll_callback(GLFWwindow*, double xoffset, double yoffset)
	{
		UI::get().send_scroll(yoffset);
	}

	void close_callback(GLFWwindow* window)
	{
		UI::get().send_close();

		glfwSetWindowShouldClose(window, GL_FALSE);
	}

	Error Window::init()
	{
		printf("====> Window::init()\n");
		//fullscreen = Setting<Fullscreen>::get().load();

		if (!glfwInit())
			return Error::Code::GLFW;

/*glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
context = glfwCreateWindow(1, 1, "", nullptr, nullptr);
glfwMakeContextCurrent(context);
glfwSetErrorCallback(error_callback);
glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

		printf("<==== Window::init()\n");
		return initwindow();
	}

	Error Window::initwindow()
	{
		printf("====> Window::initwindow()\n");
		if (glwnd)
			glfwDestroyWindow(glwnd);

		glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		printf("====> glfwCreateWindow\n");
		glwnd = glfwCreateWindow(
				1280,
				720,
				"MapleStory",
				nullptr,
				nullptr
		);


		GLFWjoystickmapping joystick_config = { .y = Setting<Joystick_Y>::get().load(),
									         .x = Setting<Joystick_X>::get().load(),
									         .b = Setting<Joystick_B>::get().load(),
			                        	     .a = Setting<Joystick_A>::get().load(),
		                                     .lb = Setting<Joystick_LB>::get().load(),
			                        	     .lt = Setting<Joystick_LT>::get().load(),
			                        	     .rb = Setting<Joystick_RB>::get().load(),
							                 .rt = Setting<Joystick_RT>::get().load()
		};
		glfwConfigureJoystickButtons(joystick_config);

		printf("<==== glfwCreateWindow\n");
		if (!glwnd) {
			printf("[!] Error calling glfwCreateWindow\n");
			return Error::Code::WINDOW;
		}

		bool vsync = Setting<VSync>::get().load();
		printf("[*] vsync value: %d\n", vsync);
		glfwSwapInterval(vsync ? 1 : 0);
		glfwSetInputMode(glwnd, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		glfwMakeContextCurrent(glwnd);
		printf("[*] made glwnd current context.\n");

		if (Error error = GraphicsGL::get().init()) {
			printf("[!] Error initializing graphics.\n");
			return error;
		}

		printf("[*] glViewport. width %d height%d\n", width, height);
		glViewport(0, 0, width, height);
		printf("[*] glMatrixMode\n");
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glfwSetInputMode(glwnd, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		double xpos, ypos;

		printf("[*] glfwGetCursorPos\n");
		glfwGetCursorPos(glwnd, &xpos, &ypos);
		cursor_callback(glwnd, xpos, ypos);

		glfwSetInputMode(glwnd, GLFW_STICKY_KEYS, GL_TRUE);
		glfwSetKeyCallback(glwnd, key_callback);
		glfwSetMouseButtonCallback(glwnd, mousekey_callback);
		glfwSetCursorPosCallback(glwnd, cursor_callback);
		glfwSetWindowFocusCallback(glwnd, focus_callback);
		glfwSetScrollCallback(glwnd, scroll_callback);
		glfwSetWindowCloseCallback(glwnd, close_callback);

		/*char buf[256];
		GetCurrentWorkingDir()
		GetCurrentDirectoryA(256, buf);*/
		//printf("Current working dir: %s", GetCurrentWorkingDir());
		//std::string icon_path = "HeavenClient/Icon.png";
		//strcat(buf, "\\Icon.png");

		//GLFWimage images[1];

		//auto stbi = stbi_load(icon_path.c_str(), &images[0].width, &images[0].height, 0, 4);

		//if (stbi == NULL)
		//	return Error(Error::Code::MISSING_ICON, stbi_failure_reason());

		//images[0].pixels = stbi;

		//glfwSetWindowIcon(glwnd, 1, images);
		//stbi_image_free(images[0].pixels);

		GraphicsGL::get().reinit();

		printf("<==== Window::initwindow()\n");
		return Error::Code::NONE;
	}

	bool Window::not_closed() const
	{
		return glfwWindowShouldClose(glwnd) == 0;
	}

	void Window::update()
	{
		updateopc();
	}

	void Window::updateopc()
	{
		if (opcstep != 0.0f)
		{
			opacity += opcstep;

			if (opacity >= 1.0f)
			{
				opacity = 1.0f;
				opcstep = 0.0f;
			}
			else if (opacity <= 0.0f)
			{
				opacity = 0.0f;
				opcstep = -opcstep;

				fadeprocedure();
			}
		}
	}

	void Window::check_events()
	{
		int16_t max_width = Configuration::get().get_max_width();
		int16_t max_height = Configuration::get().get_max_height();
		int16_t new_width = Constants::Constants::get().get_viewwidth();
		int16_t new_height = Constants::Constants::get().get_viewheight();

		if (width != new_width || height != new_height)
		{
			width = new_width;
			height = new_height;

			if (new_width >= max_width || new_height >= max_height)
				fullscreen = true;

			initwindow();
		}
#if 0 //__SWITCH__
        handleTouch();
#endif
		glfwPollEvents();
	}

	void Window::begin() const
	{
		GraphicsGL::get().clearscene();
	}

	void Window::end() const
	{
		GraphicsGL::get().flush(opacity);
		glfwSwapBuffers(glwnd);
	}

	void Window::fadeout(float step, std::function<void()> fadeproc)
	{
		opcstep = -step;
		fadeprocedure = fadeproc;
	}

	void Window::setclipboard(const std::string& text) const
	{
		glfwSetClipboardString(glwnd, text.c_str());
	}

	std::string Window::getclipboard() const
	{
		const char* text = glfwGetClipboardString(glwnd);

		return text ? text : "";
	}

	void Window::toggle_fullscreen()
	{
		int16_t max_width = Configuration::get().get_max_width();
		int16_t max_height = Configuration::get().get_max_height();

		if (width < max_width && height < max_height)
		{
			fullscreen = !fullscreen;
			Setting<Fullscreen>::get().save(fullscreen);

			initwindow();
			glfwPollEvents();
		}
	}

	std::string Window::GetCurrentWorkingDir( void ) {
		char buff[FILENAME_MAX];
		GetCurrentDir( buff, FILENAME_MAX );
		std::string current_working_dir(buff);
		return current_working_dir;
	}
}
