#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLu.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "DrawSimpleShapes.h"
#include "hermitFilling.h"
#include "fillingQuarter.h"
#include <string>
#include <cstring>

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

using namespace std;


/// init painter
const int toolsHigth = 100;
const int screenWidth = 800;
const int screenHeight = 600 - toolsHigth;
bool defaultAlgorithms = 0;
GLfloat drawingColor[] = {1.0f, 1.0f, 0.0f};
GLfloat backgroundColor[] = {1.0f, 1.0f, 1.0f};

/// create menu
HMENU hmenu;
#define Save_File 1
#define Load_File 2
#define Clear_Screen 454
#define Default 222
void AddMenu(HWND hwnd) {
    hmenu = CreateMenu();
    HMENU FileMenu = CreateMenu();
    HMENU ToolsMenu = CreateMenu();

    AppendMenuW(hmenu, MF_POPUP, (UINT_PTR) FileMenu, L"File");
    AppendMenuW(hmenu, MF_POPUP, (UINT_PTR) ToolsMenu, L"Tools");

    AppendMenuW(FileMenu, MF_STRING, Save_File, L"Save File");
    AppendMenuW(FileMenu, MF_STRING, Load_File, L"Load File");

    AppendMenuW(ToolsMenu, MF_STRING, Clear_Screen, L"Clear Screen");
    AppendMenuW(ToolsMenu, MF_STRING, Default, L"Default Algorithms");

    SetMenu(hwnd, hmenu);

    HANDLE hIcon = LoadImage(0, ("icon.ico"), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
    if (hIcon) {
        //Change both icons to the same icon handle.
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM) hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM) hIcon);

        //This will ensure that the application icon gets changed too.
        SendMessage(GetWindow(hwnd, GW_OWNER), WM_SETICON, ICON_SMALL, (LPARAM) hIcon);
        SendMessage(GetWindow(hwnd, GW_OWNER), WM_SETICON, ICON_BIG, (LPARAM) hIcon);
    }
}
/// end of menu

/// Save and Load files
void SaveFile(HWND hwnd) {
    RECT rect;
    GetWindowRect(hwnd, &rect);

    uint32_t *pixels = (uint32_t *) malloc(sizeof(uint32_t) * screenWidth * screenHeight);

    glReadPixels(rect.left, rect.top, screenWidth, screenHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    ofstream myfile;
    myfile.open("file.txt");

    int long len = abs(screenWidth - rect.left) * abs(screenHeight - rect.top);
    for (int long i = 0; i < len; i++) {
        myfile << pixels[i] << '\n';
    }

    myfile.close();
    free(pixels);

    cout << "Saved as file.txt\n";
    cout.flush();

    MessageBeep(MB_ICONASTERISK);
}

void LoadFile(HWND hwnd) {
    glBegin(GL_POINTS);

    RECT rect = {0};
    ifstream myfile;
    myfile.open("file.txt");
    uint32_t pixel;

    for (int i = toolsHigth + 1; i < screenHeight; i++) {
        for (int j = 0; j < screenWidth; j++) {
            myfile >> pixel;
            glColor4ubv((GLubyte *) &pixel);
            glVertex2d(j, i);
        }
    }

    myfile.close();
    glEnd();
    glFlush();

    cout << "Loaded from file.txt\n";
    cout.flush();

    MessageBeep(MB_ICONASTERISK);

//    for errors sound
//    MessageBeep(MB_ICONHAND);
}

/// Choose Color from pallet "e. Give me option to choose shape color before drawing from menu"
void FillScreen(int xs, int xe, int ys, int ye, GLfloat *C) {

    glBegin(GL_POINTS);
    glColor3f(C[0], C[1], C[2]);

    for (int i = ys; i < ye; i++) {
        for (int j = xs; j < xe; j++) {
            glVertex2d(j, i);
        }
    }

    glEnd();
    glFlush();
}

#define SHOW_TOOLS 101

const int palletX = 10;
const int palletY = 6;
const int palletWidth = 45;
const int palletHigth = 45;
const int NpalletWidth = 6;
const int NpalletHigth = 2;
const int stepright = palletWidth + 10;
const int stepdown = palletHigth + 5;
GLfloat toolsShading[] = {0.62f, 0.62f, 0.62f};
GLfloat toolsBKColor[] = {0.97f, 0.97f, 0.97f};
GLfloat Colors[NpalletWidth * NpalletHigth][3] = {{1.0f, 0,    0},      // Red
                                                  {0,    1.0f, 0},      // Green
                                                  {0,    0,    1.0f},   // Blue
                                                  {1.0f, 1.0,  0},    // Yellow
                                                  {0,    1.0f, 1.0f},   // Cyan
                                                  {0,    0,    0},
                                                  {0,    0,    0},
                                                  {0,    0,    0},
                                                  {0,    0,    0},
                                                  {0,    0,    0},
                                                  {0,    0,    0},
                                                  {0,    0,    0}};
vector<vector<int>> colorButtons;

void drawBlock(int x1, int y1, int x3, int y3, vector<vector<int>> &buttons, GLfloat *c) {
    FillRectangleWithHermite(x1, y1, x3, y3, c);
    drawRectangle(x1, y1, x3, y3, toolsShading);
    buttons.push_back({x1, x3, y1, y3});
}

int chooseTool(int x, int y, vector<vector<int>> &buttons) {
    int i = 0;
    while (i < buttons.size() &&
           !(buttons[i][0] < x && buttons[i][1] > x && buttons[i][2] < y && buttons[i][3] > y)) {
        i++;
    }
    cout << i << '\n';
    cout.flush();
    return i;
}

void addToolsSction() {

    FillScreen(0, screenWidth, 0, toolsHigth, toolsBKColor);
    drawLine(0, toolsHigth, screenWidth, toolsHigth, toolsShading);
    int i = 0;
    for (int y = 0; y < NpalletHigth; y++) {
        for (int x = 0; x < NpalletWidth; x++) {
            drawBlock(palletX + x * stepright, palletY + y * stepdown, x * stepright + palletWidth,
                      y * stepdown + palletHigth, colorButtons, Colors[i++]);

        }
    }
}

#define Shape_choice 4004
const int shapesX = 400;
const int shapesY = 6;
const int shapesWidth = 70;
const int shapesHigth = 40;
const int NshapesWidth = 4;
const int NshapesHigth = 2;
const int shapesright = shapesWidth + 10;
const int shapesdown = shapesHigth + 5;
const LPCTSTR shapes[] = {reinterpret_cast<LPCTSTR const>(L"Line"), reinterpret_cast<LPCTSTR const>(L"Square"),
                          reinterpret_cast<LPCTSTR const>(L"Rectangle"),
                          reinterpret_cast<LPCTSTR const>(L"Ellipse"), reinterpret_cast<LPCTSTR const>(L"Circle"),
                          reinterpret_cast<LPCTSTR const>(L"Fill Circle"), reinterpret_cast<LPCTSTR const>(L"Fill"),
                          reinterpret_cast<LPCTSTR const>(L"FloodFill"), reinterpret_cast<LPCTSTR const>(L"Curve"),
                          reinterpret_cast<LPCTSTR const>(L"Clip")};
vector<vector<int>> shapeBounds;

void addShapesButtons(HWND hwnd) {
    int i = 0, y, x;
    for (y = 0; y < NshapesHigth; y++) {
        for (x = 0; x < NshapesWidth; x++) {
            CreateWindowW(L"BUTTON", reinterpret_cast<LPCWSTR>(shapes[i]),
                          WS_VISIBLE | WS_CHILD,
                          shapesX + x * shapesright, shapesY + y * shapesdown, shapesWidth,
                          shapesHigth, hwnd, (HMENU) Shape_choice, NULL,
                          NULL);
            shapeBounds.push_back({shapesX + x * shapesright, shapesY + y * shapesdown, x * shapesright + shapesWidth,
                                   y * shapesdown + shapesHigth});
            i++;
        }
    }
    y--;
    CreateWindowW(L"BUTTON", reinterpret_cast<LPCWSTR>(shapes[i]),
                  WS_VISIBLE | WS_CHILD,
                  shapesX + x * shapesright, shapesY + y * shapesdown, shapesWidth,
                  shapesHigth, hwnd, (HMENU) Shape_choice, NULL,
                  NULL);
    shapeBounds.push_back({shapesX + x * shapesright, shapesY + y * shapesdown, x * shapesright + shapesWidth,
                           y * shapesdown + shapesHigth});
}

void getColor(LPARAM lp) {
    /// Get Color
    glReadPixels(LOWORD(lp), HIWORD(lp), 1, 1, GL_RGBA, GL_FLOAT, drawingColor);
    cout << "Color Changed to " << "R = " << drawingColor[0] << " B = " << drawingColor[1] << " G = " << drawingColor[2]
         << '\n';
    cout.flush();
    MessageBeep(MB_ICONASTERISK);
}

/// clear screen "f. Implement item to clear screen from shapes"
void clearScreen() {
    /// global background color
    FillScreen(0, screenWidth, toolsHigth + 1, screenHeight, backgroundColor);
    cout << "Screen Cleared\n";
    MessageBeep(MB_ICONASTERISK);
}


HGLRC InitOpenGl(HDC hdc) {
    PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd
            1,                     // version number
            PFD_DRAW_TO_WINDOW |   // support window
            PFD_SUPPORT_OPENGL,  // support OpenGL
//            PFD_DOUBLEBUFFER,      // double buffered
            PFD_TYPE_RGBA,         // RGBA type
            24,                    // 24-bit color depth
            0, 0, 0, 0, 0, 0,      // color bits ignored
            0,                     // no alpha buffer
            0,                     // shift bit ignored
            0,                     // no accumulation buffer
            0, 0, 0, 0,            // accum bits ignored
            32,                    // 32-bit z-buffer
            0,                     // no stencil buffer
            0,                     // no auxiliary buffer
            PFD_MAIN_PLANE,        // main layer
            0,                     // reserved
            0, 0, 0                // layer masks ignored
    };
    int iPixelFormat;
    iPixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, iPixelFormat, &pfd);
    HGLRC glrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, glrc);
    return glrc;
}

void AdjustWindowFor2D(HDC hdc, int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EndOpenGl(HGLRC glrc) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(glrc);
}

LRESULT WINAPI
MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp) {
    static HDC hdc;
    static HGLRC glrc;
    switch (mcode) {
        /// action listeners
        case WM_COMMAND:
            switch (wp) {
                case Save_File:
                    SaveFile(hwnd);
                    break;
                case Load_File:
                    LoadFile(hwnd);
                    break;
                case Clear_Screen:
                    clearScreen();
                    MessageBeep(MB_ICONASTERISK);
                    break;
            }
            break;
        case WM_CREATE:
            hdc = GetDC(hwnd);
            glrc = InitOpenGl(hdc);

            // add menu
            AddMenu(hwnd);

            // add shapes
//            addShapesButtons(hwnd);
            break;
        case SHOW_TOOLS:

            // TODO create the tools section of every thing
          //  String s;


            cout << "Tools Loaded\n";
            cout.flush();
            addToolsSction();
            addShapesButtons(hwnd);
            break;
        case WM_SIZE:
            AdjustWindowFor2D(hdc, LOWORD(lp), HIWORD(lp));
            break;
        case WM_LBUTTONDOWN:
//            chooseTool(LOWORD(lp), HIWORD(lp));

            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            EndOpenGl(glrc);
            ReleaseDC(hwnd, hdc);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, mcode, wp, lp);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmd, int nsh) {
    WNDCLASS wc;
    wc.cbClsExtra = wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = hinst;
    wc.lpfnWndProc = MyWndProc;
    wc.lpszClassName = reinterpret_cast<LPCSTR>(L"MyClass");
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow((LPCSTR) L"MyClass", (LPCSTR) "Paintit",
                             WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, 0, 0, screenWidth, screenHeight,
                             NULL, NULL, hinst,
                             0);
    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);
    MSG msg;
    SendMessage(hwnd, SHOW_TOOLS, NULL, NULL);        /// show all the tools
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}