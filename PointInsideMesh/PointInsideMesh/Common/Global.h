#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <windows.h>
#include <gl\glew.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "../Utility/TSControl.h"
#include "../Utility/VMath.h"

#define FPS                            60

extern HGLRC                           g_hRC;
extern HDC                             g_hDC;
extern HWND                            g_hWnd;
extern HINSTANCE                       g_hInstance;

extern bool                            g_bKeys[256];
extern bool                            g_bActive;
extern bool                            g_bFullscreen;

extern TSConsole                       g_Console;
extern int                             g_Fps;

enum eMyMouseEvent {
    eMouseMove = 0,
    eLeftButtonDown,
    eLeftButtonUp,
    eRightButtonDown,
    eRightButtonUp,
    eMouseWheelScrollUp,
    eMouseWheelScrollDown,
};

class IObject {
public:
    virtual bool                       Initialize(void* arg)              = 0;
    virtual bool                       Update()                           = 0;
    virtual bool                       Destroy()                          = 0;

    virtual bool                       InteractWith(void* arg)            = 0;
    virtual void                       DebugOutput()                      = 0;
    virtual bool                       Render()                           = 0;

    virtual void                       OnMouseEvent(eMyMouseEvent evt, WPARAM wParam, LPARAM lParam) = 0;

};

#endif ///> end of __GLOBAL_H__