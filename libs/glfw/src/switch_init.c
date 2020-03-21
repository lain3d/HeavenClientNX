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

#include "internal.h"


static AppletHookCookie s_glfwAppletHookCookie;

static void _glfwSwitchRefreshFocusState(void)
{
    int is_focused = appletGetFocusState() == AppletFocusState_Focused ? GLFW_TRUE : GLFW_FALSE;
    if (is_focused != _glfw.nx.is_focused)
    {
        _glfw.nx.event_mask |= _GLFW_SWITCH_EVENT_FOCUS_CHANGED;
        _glfw.nx.is_focused = is_focused;
    }
}

static void _glfwSwitchRefreshScreenSize(void)
{
    _glfw.nx.event_mask |= _GLFW_SWITCH_EVENT_SCREEN_SIZE_CHANGED;
    switch (appletGetOperationMode())
    {
        default:
        case AppletOperationMode_Handheld:
            _glfw.nx.scr_width = 1280;
            _glfw.nx.scr_height = 720;
            break;
        case AppletOperationMode_Docked:
            _glfw.nx.scr_width = 1920;
            _glfw.nx.scr_height = 1080;
            break;
    }
}

static void _glfwSwitchAppletHook(AppletHookType hook, void* param)
{
    switch (hook)
    {
        default:
            break;
        case AppletHookType_OnFocusState:
            _glfwSwitchRefreshFocusState();
            break;
        case AppletHookType_OnOperationMode:
            _glfwSwitchRefreshScreenSize();
            break;
    }
}

__attribute__ ((weak))
void _glfwPlatformTerminateContextApi(void)
{
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW platform API                      //////
//////////////////////////////////////////////////////////////////////////

int _glfwPlatformInit(void)
{
    appletLockExit();
    appletSetFocusHandlingMode(AppletFocusHandlingMode_NoSuspend);
    appletHook(&s_glfwAppletHookCookie, _glfwSwitchAppletHook, NULL);
    _glfwSwitchRefreshFocusState();
    _glfwSwitchRefreshScreenSize();

    _glfwAllocMonitor("Default", 1920, 1080);
    _glfwInitSwitchJoysticks();
    return GLFW_TRUE;
}

void _glfwPlatformTerminate(void)
{
    _glfwPlatformTerminateContextApi();
    appletUnhook(&s_glfwAppletHookCookie);
    appletUnlockExit();
}

const char* _glfwPlatformGetVersionString(void)
{
    return _GLFW_VERSION_NUMBER " Switch EGL";
}

