//========================================================================
// GLFW 3.3 - www.glfw.org
//------------------------------------------------------------------------
// Copyright (c) 2016 Google Inc.
// Copyright (c) 2016-2017 Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================

#include <switch.h>

// Internally rename GLFW OpenGL context management functions
// This is necessary in order to avoid a linker dependency to EGL/mesa when
// GLFW's OpenGL context management functions aren't called by user code
//
#if !defined(_GLFW_SWITCH_NO_CONTEXT_WRAPPER)
#define glfwMakeContextCurrent _glfwMakeContextCurrentImpl
#define glfwGetCurrentContext _glfwGetCurrentContextImpl
#define glfwSwapBuffers _glfwSwapBuffersImpl
#define glfwSwapInterval _glfwSwapIntervalImpl
#define glfwExtensionSupported _glfwExtensionSupportedImpl
#define glfwGetProcAddress _glfwGetProcAddressImpl
#endif

#define _GLFW_EGL_NATIVE_WINDOW  ((EGLNativeWindowType) window->nx.nwin)
#define _GLFW_EGL_NATIVE_DISPLAY EGL_DEFAULT_DISPLAY

#define _GLFW_PLATFORM_WINDOW_STATE         _GLFWwindowNX nx
#define _GLFW_PLATFORM_LIBRARY_WINDOW_STATE _GLFWlibraryNX nx

#define _GLFW_PLATFORM_CONTEXT_STATE
#define _GLFW_PLATFORM_LIBRARY_CONTEXT_STATE

#define _GLFW_OSMESA_CONTEXT_STATE
#define _GLFW_OSMESA_LIBRARY_CONTEXT_STATE

#define _GLFW_PLATFORM_LIBRARY_TIMER_STATE
#define _GLFW_PLATFORM_MONITOR_STATE
#define _GLFW_PLATFORM_CURSOR_STATE

#include "egl_context.h"
#include "posix_thread.h"
#include "switch_joystick.h"

#define _GLFW_SWITCH_EVENT_FOCUS_CHANGED       0x01
#define _GLFW_SWITCH_EVENT_SCREEN_SIZE_CHANGED 0x02

// Switch-specific per-window data
//
typedef struct _GLFWwindowNX
{
    NWindow* nwin;
    int width, height;
} _GLFWwindowNX;

// Switch-specific global data
//
typedef struct _GLFWlibraryNX
{
    _GLFWwindow* cur_window;
    int event_mask;
    int is_focused;
    int scr_width, scr_height;
} _GLFWlibraryNX;

// Switch-specific context management functions
//
int _glfwPlatformCreateContext(_GLFWwindow* window,
                                   const _GLFWctxconfig* ctxconfig,
                                   const _GLFWfbconfig* fbconfig);
void _glfwPlatformTerminateContextApi(void);

// Wrapped names of GLFW context management functions
//
GLFWAPI void _glfwMakeContextCurrentImpl(GLFWwindow* handle);
GLFWAPI GLFWwindow* _glfwGetCurrentContextImpl(void);
GLFWAPI void _glfwSwapBuffersImpl(GLFWwindow* handle);
GLFWAPI void _glfwSwapIntervalImpl(int interval);
GLFWAPI int _glfwExtensionSupportedImpl(const char* extension);
GLFWAPI GLFWglproc _glfwGetProcAddressImpl(const char* procname);

