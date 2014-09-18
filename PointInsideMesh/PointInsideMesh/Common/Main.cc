#include "Global.h"
#include "../Primitives/CMesh.h"
#include "SceneManager.h"
#include "../Primitives/CLight.h"

extern CLight g_Light;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void ReSizeGlScene(GLsizei width, GLsizei height)
{
    if (!height) {
        height = 1;
    }
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// This function is called before any OpenGL functions are called
bool PreInitialize()
{
    glewInit();
//    g_ShaderMgr.Initialize("spotlight.vert", "spotlight.frag");
    return true;
}

bool Initialize()
{
    SceneManager::GetInstance()->Initialize(NULL);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);
    glShadeModel(GL_SMOOTH);

    g_Light.InitLight("spotlight.vert", "spotlight.frag");

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);

//    g_ShaderMgr.TurnShader(true);

    return true;
}

bool DrawGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    SceneManager::GetInstance()->Update();
    SceneManager::GetInstance()->Render();

    return true;
}

void KillGLWindow()
{
    SceneManager::GetInstance()->Destroy();

    if (g_bFullscreen) {
        ChangeDisplaySettings(NULL, 0);
        ShowCursor(true);
    }
    if (g_hRC) {
        if (!wglMakeCurrent(NULL, NULL)) {
            MessageBox(NULL, "Release Of DC And Rc Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        }
        if (!wglDeleteContext(g_hRC)) {
            MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        }
        g_hRC = NULL;
    }
    if (g_hDC && !ReleaseDC(g_hWnd, g_hDC)) {
        MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        g_hDC = NULL;
    }
    if (g_hWnd && !DestroyWindow(g_hWnd)) {
        MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        g_hWnd = NULL;
    }
    if (!UnregisterClass("OpenGL", g_hInstance)) {
        MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        g_hInstance = NULL;
    }
}

bool CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
    GLuint PixelFormat;
    WNDCLASS wc;
    DWORD dwExStyle;
    DWORD dwStyle;
    RECT WindowRect;
    WindowRect.left = (long)0;
    WindowRect.right = (long)width;
    WindowRect.top = (long)0;
    WindowRect.bottom = (long)height;
    g_bFullscreen = fullscreenflag;
    g_hInstance = GetModuleHandle(NULL);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC) WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = g_hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "OpenGL";

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONINFORMATION);
        return false;
    }

    if (g_bFullscreen) {
        DEVMODE dmScreenSettings;
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = width;
        dmScreenSettings.dmPelsHeight = height;
        dmScreenSettings.dmBitsPerPel = bits;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        if(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_BADDUALVIEW) {
            if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONINFORMATION) == IDYES) {
                g_bFullscreen = false;
            } else {
                MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
                return false;
            }
        }
    }
    if (g_bFullscreen) {
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle = WS_POPUP;
        ShowCursor(false);
    } else {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle = WS_OVERLAPPEDWINDOW;
    }

    AdjustWindowRectEx(&WindowRect, dwStyle, false, dwExStyle);

    if (!(g_hWnd = CreateWindowEx(
        dwExStyle, "OpenGL", title,
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,
        0, 0,
        WindowRect.right-WindowRect.left,
        WindowRect.bottom-WindowRect.top,
        NULL, NULL, g_hInstance, NULL))) {
        KillGLWindow();
        MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    static PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        bits,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 
        0, 16, 0, 0,
        PFD_MAIN_PLANE,
        0, 0, 0, 0
    };

    if (!(g_hDC = GetDC(g_hWnd))) {
        KillGLWindow();
        MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    if (!(PixelFormat = ChoosePixelFormat(g_hDC, &pfd))) {
        KillGLWindow();
        MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    if (!SetPixelFormat(g_hDC, PixelFormat, &pfd)) {
        KillGLWindow();
        MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return false;
    }

    if (!(g_hRC = wglCreateContext(g_hDC))) {
        KillGLWindow();
        MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    if (!wglMakeCurrent(g_hDC, g_hRC)) {
        KillGLWindow();
        MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    ShowWindow(g_hWnd, SW_SHOW);
    SetForegroundWindow(g_hWnd);
    SetFocus(g_hWnd);

    PreInitialize();

    ReSizeGlScene(width, height);
    if (!Initialize()) {
        KillGLWindow();
        MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_ACTIVATE:
        {
            if (!HIWORD(wParam)) {
                g_bActive = true;
            } else {
                g_bActive = false;
            }
            return 0;
        } break;
    case WM_SYSCOMMAND:
        {
            switch(wParam)
            {
            case SC_SCREENSAVE:
            case SC_MONITORPOWER:
                return 0;
            }
        } break;
    case WM_CLOSE:
        {
            PostQuitMessage(0);
            return 0;
        } break;
    case WM_KEYDOWN:
        {
            g_bKeys[wParam] = true;
            return 0;
        } break;
    case WM_KEYUP:
        {
            g_bKeys[wParam] = false;
            return 0;
        } break;
    case WM_MOUSEMOVE:
        {
            SceneManager::GetInstance()->OnMouseEvent(eMouseMove, wParam, lParam);
            return 0;
        } break;
    case WM_LBUTTONUP:
        {
            SceneManager::GetInstance()->OnMouseEvent(eLeftButtonUp, wParam, lParam);
            return 0;
        } break;
    case WM_RBUTTONDOWN:
        {
            SceneManager::GetInstance()->OnMouseEvent(eRightButtonDown, wParam, lParam);
            return 0;
        } break;
    case WM_RBUTTONUP:
        {
            SceneManager::GetInstance()->OnMouseEvent(eRightButtonUp, wParam, lParam);
            return 0;
        } break;
    case WM_MOUSEWHEEL:
        {
            static int thisDelta = 0, lastDelta;
            thisDelta += GET_WHEEL_DELTA_WPARAM(wParam);
            if (thisDelta != lastDelta) {
                if (thisDelta > lastDelta) {
                    SceneManager::GetInstance()->OnMouseEvent(eMouseWheelScrollUp, wParam, lParam);
                } else if (thisDelta < lastDelta) {
                    SceneManager::GetInstance()->OnMouseEvent(eMouseWheelScrollDown, wParam, lParam);
                }
                lastDelta = thisDelta;
            }
            return 0;
        } break;
    case WM_SIZE:
        {
            ReSizeGlScene(LOWORD(lParam), HIWORD(lParam));
            return 0;
        } break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrecInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    bool done = false;

    if (!CreateGLWindow("Point Inside Mesh", 1366, 768, 16, g_bFullscreen)) {
        return 0;
    }
    while (!done) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                done = true;
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        } else {
            if (g_bActive) {
                if (g_bKeys[VK_ESCAPE]) {
                    done = true;
                } else {
                    DWORD starttime = timeGetTime();
                    DrawGLScene();
                    SwapBuffers(g_hDC);

                    // limit the FPS
                    DWORD endtime = timeGetTime() - starttime;
                    if (endtime < 1000 / FPS) {
                        Sleep(1000 / FPS - endtime);
                    }
                    endtime = timeGetTime() - starttime;
                    if (endtime == 0) {
                        g_Fps = 60;
                    } else {
                        g_Fps = 1000 / endtime;
                    }
                    //std::cout << gFps << std::endl;
                }
            }
        }
    }
    KillGLWindow();
    return (msg.wParam);
}
