#include "Global.h"
#include "SceneManager.h"
#include "../Utility/ShaderManager.h"
#include "../Primitives/CLight.h"

HGLRC                                   g_hRC               = NULL;
HDC                                     g_hDC               = NULL;
HWND                                    g_hWnd              = NULL;
HINSTANCE                               g_hInstance;

bool                                    g_bKeys[256];
bool                                    g_bActive           = true;
bool                                    g_bFullscreen       = false;

TSConsole                               g_Console;

int                                     g_Fps = 60;

int                                     g_SelectedIndex     = -1;
ShaderManager                           g_ShaderMgr;

CLight                                  g_Light;