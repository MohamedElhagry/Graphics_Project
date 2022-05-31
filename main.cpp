#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLu.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "DrawSimpleShapes.h"
#include "hermitFilling.h"
#include "fillingQuarter.h"


#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

using namespace std;


/// init painter
const int screenWidth = 800;
const int screenHeight = 600;
GLfloat drawingColor[] = {1.0f, 1.0f, 0.0f};
GLfloat backgroundColor[] = {1.0f, 1.0f, 1.0f};

/// create menu
HMENU hmenu;
#define Save_File 1
#define Load_File 2
#define Clear_Screen 454

void AddMenu(HWND hwnd) {
    hmenu = CreateMenu();
    HMENU FileMenu = CreateMenu();
    AppendMenuW(hmenu, MF_POPUP, (UINT_PTR) FileMenu, L"File");

    AppendMenuW(FileMenu, MF_STRING, Save_File, L"Save File");
    AppendMenuW(FileMenu, MF_STRING, Load_File, L"Load File");
    AppendMenuW(FileMenu, MF_SEPARATOR, NULL, L"Space");
    AppendMenuW(FileMenu, MF_STRING, Clear_Screen, L"Clear Screen");
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
    RECT rect = {0};
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
    GetWindowRect(hwnd, &rect);
    uint32_t pixel;

    for (int i = 0; i < screenHeight; i++) {
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
const int toolsHigth = 100;
GLfloat toolsLine[] = {0.5111f, 0.5111f, 0.5111f};
GLfloat toolsBKColor[] = {0.89f, 0.89f, 0.89f};
GLfloat Colors[NpalletWidth * NpalletHigth][3] = {{1.0f, 0,    0},      // Red
                                                  {0,    1.0f, 0},      // Green
                                                  {0,    0,    1.0f},   // Blue
                                                  {1.0f, 1.0,    0},    // Yellow
                                                  {0,    1.0f, 0},
                                                  {0,    0,    1.0f},
                                                  {1.0f, 0,    0},
                                                  {0,    1.0f, 0},
                                                  {0,    0,    1.0f},
                                                  {1.0f, 0,    0},
                                                  {0,    1.0f, 0},
                                                  {0,    0,    1.0f},};
vector<vector<int>> toolsButton;

void drawBlock(int x1, int y1, int x3, int y3, GLfloat *c) {
    FillRectangleWithHermite(x1, y1, x3, y3, c);
    toolsButton.push_back({x1, x3, y1, y3});
}

int choosetool(int x, int y) {
    int i = 0;
    while (i < toolsButton.size() &&
           !(toolsButton[i][0] < x && toolsButton[i][1] > x && toolsButton[i][2] < y && toolsButton[i][3] > y)) {
        i++;
    }
    cout << i << '\n';
    cout.flush();
    return i;
}

void addToolsSction() {

    FillScreen(0, screenWidth, 0, toolsHigth, toolsBKColor);
    drawLine(0, toolsHigth, screenWidth, toolsHigth, toolsLine);
    int i = 0;
    for (int y = 0; y < NpalletHigth; y++) {
        for (int x = 0; x < NpalletWidth; x++) {

            // TODO Choose color for each button
            drawBlock(palletX + x * stepright, palletY + y * stepdown, x * stepright + palletWidth,
                      y * stepdown + palletHigth,
                      Colors[i++]);

        }
    }
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

            break;
        case SHOW_TOOLS:

            // TODO create the tools section of every thing
            cout << "Tools Loaded\n";
            cout.flush();
            addToolsSction();

            break;
        case WM_SIZE:
            AdjustWindowFor2D(hdc, LOWORD(lp), HIWORD(lp));
            break;
        case WM_LBUTTONDOWN:
            choosetool(LOWORD(lp), HIWORD(lp));

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