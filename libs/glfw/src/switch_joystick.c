//========================================================================
// GLFW 3.3 - www.glfw.org
//------------------------------------------------------------------------
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
#include "string.h"

// Built-in soft-reset combo that triggers a GLFW window close event
#define SOFT_RESET_COMBO (KEY_PLUS|KEY_MINUS|KEY_L|KEY_R)

// Internal constants for gamepad mapping source types
#define _GLFW_JOYSTICK_AXIS     1
#define _GLFW_JOYSTICK_BUTTON   2
#define _GLFW_JOYSTICK_HATBIT   3

#define TOUCH_WIDTH             1280
#define TOUCH_HEIGHT            720

GLFWjoystickmapping joystick_config = { -1 };

enum
{
    _SWITCH_AXIS_LEFT_X,
    _SWITCH_AXIS_LEFT_Y,
    _SWITCH_AXIS_RIGHT_X,
    _SWITCH_AXIS_RIGHT_Y,

    _SWITCH_AXIS_COUNT
};

enum
{
    _SWITCH_BUTTON_A,
    _SWITCH_BUTTON_B,
    _SWITCH_BUTTON_X,
    _SWITCH_BUTTON_Y,
    _SWITCH_BUTTON_LSTICK,
    _SWITCH_BUTTON_RSTICK,
    _SWITCH_BUTTON_L,
    _SWITCH_BUTTON_R,
    _SWITCH_BUTTON_ZL,
    _SWITCH_BUTTON_ZR,
    _SWITCH_BUTTON_PLUS,
    _SWITCH_BUTTON_MINUS,

    _SWITCH_BUTTON_COUNT
};

enum
{
    _SWITCH_HAT_DPAD,
    _SWITCH_HAT_LEFT_STICK,
    _SWITCH_HAT_RIGHT_STICK,

    _SWITCH_HAT_COUNT
};

static _GLFWmapping s_switchMapping =
{
    .name = "Nintendo Switch Controller",
    .guid = "",
    .buttons =
    {
        [GLFW_GAMEPAD_BUTTON_A]            = { .type = _GLFW_JOYSTICK_BUTTON, .index = _SWITCH_BUTTON_A },
        [GLFW_GAMEPAD_BUTTON_B]            = { .type = _GLFW_JOYSTICK_BUTTON, .index = _SWITCH_BUTTON_B },
        [GLFW_GAMEPAD_BUTTON_X]            = { .type = _GLFW_JOYSTICK_BUTTON, .index = _SWITCH_BUTTON_X },
        [GLFW_GAMEPAD_BUTTON_Y]            = { .type = _GLFW_JOYSTICK_BUTTON, .index = _SWITCH_BUTTON_Y },
        [GLFW_GAMEPAD_BUTTON_BACK]         = { .type = _GLFW_JOYSTICK_BUTTON, .index = _SWITCH_BUTTON_MINUS },
        [GLFW_GAMEPAD_BUTTON_START]        = { .type = _GLFW_JOYSTICK_BUTTON, .index = _SWITCH_BUTTON_PLUS },
        //[GLFW_GAMEPAD_BUTTON_GUIDE]        = { .type = _GLFW_JOYSTICK_BUTTON, .index = 0 },
        [GLFW_GAMEPAD_BUTTON_LEFT_BUMPER]  = { .type = _GLFW_JOYSTICK_BUTTON, .index = _SWITCH_BUTTON_L },
        [GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] = { .type = _GLFW_JOYSTICK_BUTTON, .index = _SWITCH_BUTTON_R },
        [GLFW_GAMEPAD_BUTTON_LEFT_THUMB]   = { .type = _GLFW_JOYSTICK_BUTTON, .index = _SWITCH_BUTTON_LSTICK },
        [GLFW_GAMEPAD_BUTTON_RIGHT_THUMB]  = { .type = _GLFW_JOYSTICK_BUTTON, .index = _SWITCH_BUTTON_RSTICK },
        [GLFW_GAMEPAD_BUTTON_DPAD_UP]      = { .type = _GLFW_JOYSTICK_HATBIT, .index = (_SWITCH_HAT_DPAD<<4) | 0x1 },
        [GLFW_GAMEPAD_BUTTON_DPAD_RIGHT]   = { .type = _GLFW_JOYSTICK_HATBIT, .index = (_SWITCH_HAT_DPAD<<4) | 0x2 },
        [GLFW_GAMEPAD_BUTTON_DPAD_DOWN]    = { .type = _GLFW_JOYSTICK_HATBIT, .index = (_SWITCH_HAT_DPAD<<4) | 0x4 },
        [GLFW_GAMEPAD_BUTTON_DPAD_LEFT]    = { .type = _GLFW_JOYSTICK_HATBIT, .index = (_SWITCH_HAT_DPAD<<4) | 0x8 },
    },
    .axes =
    {
        [GLFW_GAMEPAD_AXIS_LEFT_TRIGGER]   = { .type = _GLFW_JOYSTICK_BUTTON, .index = _SWITCH_BUTTON_ZL },
        [GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER]  = { .type = _GLFW_JOYSTICK_BUTTON, .index = _SWITCH_BUTTON_ZR },
        [GLFW_GAMEPAD_AXIS_LEFT_X]         = { .type = _GLFW_JOYSTICK_AXIS,   .index = _SWITCH_AXIS_LEFT_X,  .axisScale = 1, .axisOffset = 0 },
        [GLFW_GAMEPAD_AXIS_LEFT_Y]         = { .type = _GLFW_JOYSTICK_AXIS,   .index = _SWITCH_AXIS_LEFT_Y,  .axisScale = 1, .axisOffset = 0 },
        [GLFW_GAMEPAD_AXIS_RIGHT_X]        = { .type = _GLFW_JOYSTICK_AXIS,   .index = _SWITCH_AXIS_RIGHT_X, .axisScale = 1, .axisOffset = 0 },
        [GLFW_GAMEPAD_AXIS_RIGHT_Y]        = { .type = _GLFW_JOYSTICK_AXIS,   .index = _SWITCH_AXIS_RIGHT_Y, .axisScale = 1, .axisOffset = 0 },
    },
};

void _glfwInitSwitchJoysticks(void)
{
    _GLFWjoystick* js = _glfwAllocJoystick(s_switchMapping.name, s_switchMapping.guid,
        _SWITCH_AXIS_COUNT, _SWITCH_BUTTON_COUNT, _SWITCH_HAT_COUNT);

    js->mapping = &s_switchMapping;
    js->nx.id = CONTROLLER_P1_AUTO;
}

extern int get_scancode_by_glfwid(int);
void _glfwUpdateSwitchJoysticks(void)
{
    u64 down, held, up;

    // Read input state
    hidScanInput();
    down = hidKeysDown(CONTROLLER_P1_AUTO);
    held = hidKeysHeld(CONTROLLER_P1_AUTO);
    up   = hidKeysUp  (CONTROLLER_P1_AUTO);

    // Check for soft-reset combo
    if ((held & SOFT_RESET_COMBO) == SOFT_RESET_COMBO)
    {
        _glfwInputWindowCloseRequest(_glfw.nx.cur_window);
        return;
    }

#define MAP_KEY(_libnx_key, _glfw_key, _scancode) \
    do { \
        if (down & (_libnx_key)) _glfwInputKey(_glfw.nx.cur_window, (_glfw_key), (_scancode), GLFW_PRESS, 0); \
        else if (up & (_libnx_key)) _glfwInputKey(_glfw.nx.cur_window, (_glfw_key), (_scancode), GLFW_RELEASE, 0); \
    } while (0)

    // Map common keyboard keys to the controller
    // TODO: Only do this mapping if a keyboard isn't connected
    MAP_KEY(KEY_UP, GLFW_KEY_UP, KBD_UP);
    MAP_KEY(KEY_DOWN, GLFW_KEY_DOWN, KBD_DOWN);
    MAP_KEY(KEY_LEFT, GLFW_KEY_LEFT, KBD_LEFT);
    MAP_KEY(KEY_RIGHT, GLFW_KEY_RIGHT, KBD_RIGHT);
    MAP_KEY(KEY_A, joystick_config.a, get_scancode_by_glfwid(joystick_config.a));
    MAP_KEY(KEY_B, joystick_config.b, get_scancode_by_glfwid(joystick_config.b));
    MAP_KEY(KEY_X, joystick_config.x, get_scancode_by_glfwid(joystick_config.x));
    MAP_KEY(KEY_Y, joystick_config.y, get_scancode_by_glfwid(joystick_config.y));
    MAP_KEY(KEY_L, joystick_config.lb, get_scancode_by_glfwid(joystick_config.lb));
    MAP_KEY(KEY_R, joystick_config.rb, get_scancode_by_glfwid(joystick_config.rb));
    MAP_KEY(KEY_ZL, joystick_config.lt, get_scancode_by_glfwid(joystick_config.lt));
    MAP_KEY(KEY_ZR, joystick_config.rt, get_scancode_by_glfwid(joystick_config.rt));
    MAP_KEY(KEY_PLUS, GLFW_KEY_ENTER, KBD_ENTER);
    MAP_KEY(KEY_MINUS, GLFW_KEY_ESCAPE, KBD_ESC);

    // Report touch inputs as mouse clicks
    if (hidTouchCount() > 0)
    {
        touchPosition touch;
        hidTouchRead(&touch, 0);

        double scaledXPos = (double)touch.px / TOUCH_WIDTH * _glfw.nx.scr_width;
        double scaledYPos = (double)touch.py / TOUCH_HEIGHT * _glfw.nx.scr_height;

        _glfwInputCursorPos(_glfw.nx.cur_window, scaledXPos, scaledYPos);

        if (_glfw.nx.cur_window->mouseButtons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_RELEASE)
            _glfwInputMouseClick(_glfw.nx.cur_window, GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0);

    } else if (_glfw.nx.cur_window->mouseButtons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS)
        _glfwInputMouseClick(_glfw.nx.cur_window, GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE, 0);
}

//////////////////////////////////////////////////////////////////////////
//////                       GLFW platform API                      //////
//////////////////////////////////////////////////////////////////////////

int _glfwPlatformPollJoystick(_GLFWjoystick* js, int mode)
{
    // Detect presence - we assume CONTROLLER_P1_AUTO always exists
    if (js->nx.id != CONTROLLER_P1_AUTO && !hidIsControllerConnected(js->nx.id))
        return GLFW_FALSE;

    if (mode & _GLFW_POLL_AXES)
    {
        JoystickPosition left, right;
        hidJoystickRead(&left,  js->nx.id, JOYSTICK_LEFT);
        hidJoystickRead(&right, js->nx.id, JOYSTICK_RIGHT);
        _glfwInputJoystickAxis(js, _SWITCH_AXIS_LEFT_X,   left.dx  / 32768.0f);
        _glfwInputJoystickAxis(js, _SWITCH_AXIS_LEFT_Y,  -left.dy  / 32768.0f);
        _glfwInputJoystickAxis(js, _SWITCH_AXIS_RIGHT_X,  right.dx / 32768.0f);
        _glfwInputJoystickAxis(js, _SWITCH_AXIS_RIGHT_Y, -right.dy / 32768.0f);
    }

    if (mode & _GLFW_POLL_BUTTONS)
    {
        int i;
        u64 keys = hidKeysHeld(js->nx.id);
        keys |= ((keys >> 24) & 0x3) << _SWITCH_BUTTON_L; // Map KEY_SL/SR_LEFT  into KEY_L/R
        keys |= ((keys >> 26) & 0x3) << _SWITCH_BUTTON_L; // Map KEY_SL/SR_RIGHT into KEY_L/R
        for (i = 0; i < _SWITCH_BUTTON_COUNT; i ++)
            _glfwInputJoystickButton(js, i, (keys & BIT(i)) ? GLFW_PRESS : GLFW_RELEASE);
        for (i = 0; i < _SWITCH_HAT_COUNT; i ++)
        {
            // Extract hat values. A small ROR operation is necessary
            // in order to convert Switch DRUL into glfw LDRU
            char hatbits = (keys >> (12 + 4*i)) & 0xF;
            hatbits = (hatbits >> 1) | ((hatbits & 1) << 3);
            _glfwInputJoystickHat(js, i, hatbits);
        }
    }

    return GLFW_TRUE;
}

/* Not used on Switch
void _glfwPlatformUpdateGamepadGUID(char* guid)
{
}
*/

void glfwConfigureJoystickButtons(GLFWjoystickmapping in_joystick_config)
{
    printf("SETTING JOYSTICK CONFIGURATION... %d %d %d %d %d %d %d %d\n",
    in_joystick_config.y, in_joystick_config.x, in_joystick_config.b, in_joystick_config.a,
    in_joystick_config.lb, in_joystick_config.lt, in_joystick_config.rb, in_joystick_config.rt);
    
    joystick_config = in_joystick_config;
}

