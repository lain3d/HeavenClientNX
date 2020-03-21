//========================================================================
// GLFW 3.3 - www.glfw.org
//------------------------------------------------------------------------
// Copyright (c) 2016 Google Inc.
// Copyright (c) 2016-2019 Camilla Löwy <elmindreda@glfw.org>
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

#define _GLFW_SWITCH_NO_CONTEXT_WRAPPER
#include "internal.h"


int _glfwPlatformCreateContext(_GLFWwindow* window,
                                   const _GLFWctxconfig* ctxconfig,
                                   const _GLFWfbconfig* fbconfig)
{
    if (!_glfwInitEGL())
        return GLFW_FALSE;
    if (!_glfwCreateContextEGL(window, ctxconfig, fbconfig))
        return GLFW_FALSE;
    return GLFW_TRUE;
}

void _glfwPlatformTerminateContextApi(void)
{
    _glfwTerminateEGL();
}

GLFWAPI void glfwMakeContextCurrent(GLFWwindow* handle)
{
    _glfwMakeContextCurrentImpl(handle);
}

GLFWAPI GLFWwindow* glfwGetCurrentContext(void)
{
    return _glfwGetCurrentContextImpl();
}

GLFWAPI void glfwSwapBuffers(GLFWwindow* handle)
{
    _glfwSwapBuffersImpl(handle);
}

GLFWAPI void glfwSwapInterval(int interval)
{
    _glfwSwapIntervalImpl(interval);
}

GLFWAPI int glfwExtensionSupported(const char* extension)
{
    return _glfwExtensionSupportedImpl(extension);
}

GLFWAPI GLFWglproc glfwGetProcAddress(const char* procname)
{
    return _glfwGetProcAddressImpl(procname);
}

