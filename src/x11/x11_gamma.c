//========================================================================
// GLFW - An OpenGL framework
// Platform:    X11/GLX
// API version: 3.0
// WWW:         http://www.glfw.org/
//------------------------------------------------------------------------
// Copyright (c) 2010 Camilla Berglund <elmindreda@elmindreda.org>
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

#include <limits.h>
#include <string.h>


//************************************************************************
//****                  GLFW internal functions                       ****
//************************************************************************

//========================================================================
// Save the original gamma ramp so that we can restore it later
//========================================================================

void _glfwPlatformSaveGammaRamp(void)
{
    if (_glfwLibrary.X11.XRandR.available &&
        !_glfwLibrary.X11.XRandR.gammaBroken)
    {
#if defined (_GLFW_HAS_XRANDR)
        size_t size = GLFW_GAMMA_RAMP_SIZE * sizeof(unsigned short);

        XRRScreenResources* rr = XRRGetScreenResources(_glfwLibrary.X11.display,
                                                       _glfwLibrary.X11.root);

        XRRCrtcGamma* gamma = XRRGetCrtcGamma(_glfwLibrary.X11.display,
                                              rr->crtcs[0]);

        memcpy(_glfwLibrary.originalRamp.red, gamma->red, size);
        memcpy(_glfwLibrary.originalRamp.green, gamma->green, size);
        memcpy(_glfwLibrary.originalRamp.blue, gamma->blue, size);

        XRRFreeGamma(gamma);
        XRRFreeScreenResources(rr);
    }
#endif
    else if (_glfwLibrary.X11.XF86VidMode.available)
    {
#if defined (_GLFW_HAS_XF86VIDMODE)
        XF86VidModeGetGammaRamp(_glfwLibrary.X11.display,
                                _glfwLibrary.X11.screen,
                                GLFW_GAMMA_RAMP_SIZE,
                                _glfwLibrary.originalRamp.red,
                                _glfwLibrary.originalRamp.green,
                                _glfwLibrary.originalRamp.blue);
#endif
    }
}


//========================================================================
// Make the specified gamma ramp current
//========================================================================

void _glfwPlatformSetGammaRamp(const GLFWgammaramp* ramp)
{
    if (_glfwLibrary.X11.XRandR.available &&
        !_glfwLibrary.X11.XRandR.gammaBroken)
    {
#if defined (_GLFW_HAS_XRANDR)
        int i;
        size_t size = GLFW_GAMMA_RAMP_SIZE * sizeof(unsigned short);

        XRRScreenResources* rr = XRRGetScreenResources(_glfwLibrary.X11.display,
                                                       _glfwLibrary.X11.root);

        // Update gamma per monitor
        for (i = 0;  i < rr->ncrtc;  i++)
        {
            XRRCrtcGamma* gamma = XRRAllocGamma(GLFW_GAMMA_RAMP_SIZE);

            memcpy(gamma->red, ramp->red, size);
            memcpy(gamma->green, ramp->green, size);
            memcpy(gamma->blue, ramp->blue, size);

            XRRSetCrtcGamma(_glfwLibrary.X11.display, rr->crtcs[i], gamma);
            XRRFreeGamma(gamma);
        }

        XRRFreeScreenResources(rr);
    }
#endif
    else if (_glfwLibrary.X11.XF86VidMode.available)
    {
#if defined (_GLFW_HAS_XF86VIDMODE)
        XF86VidModeSetGammaRamp(_glfwLibrary.X11.display,
                                _glfwLibrary.X11.screen,
                                GLFW_GAMMA_RAMP_SIZE,
                                (unsigned short*) ramp->red,
                                (unsigned short*) ramp->green,
                                (unsigned short*) ramp->blue);
#endif
    }
}
