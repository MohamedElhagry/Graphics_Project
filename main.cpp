#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLu.h>
#include <math.h>
#include <iostream>
#include <Commctrl.h>
#include <fstream>

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

using namespace std;

const int screenWidth = 800;
const int screenHeight = 600;
int Bitmap[screenWidth * screenHeight][3];


/// create menu

HMENU hmenu;
#define Save_File 1
#define Load_File 2

void AddMenu(HWND hwnd) {
    hmenu = CreateMenu();
    HMENU FileMenu = CreateMenu();
    AppendMenuW(hmenu, MF_POPUP, (UINT_PTR) FileMenu, L"File");

    AppendMenuW(FileMenu, MF_STRING, Save_File, L"Save File");
    AppendMenuW(FileMenu, MF_STRING, Load_File, L"Load File");
    // AppendMenuW(FileMenu, MF_SEPARATOR, NULL, NULL);

    SetMenu(hwnd, hmenu);
}

// end of meanu

/// Save and Load files

void SaveFile(HWND hwnd) {
    RECT rect = {0};
    ofstream myfile;
    myfile.open("file.txt");
    int Height = abs(rect.top - rect.bottom);
    GetWindowRect(hwnd, &rect);
    uint32_t *pixels = (uint32_t *) malloc(sizeof(uint32_t) * screenWidth * screenHeight);
    glReadPixels(rect.left, rect.top, screenWidth, screenHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    for (int i = rect.left; i < rect.right; i++) {
        for (int j = rect.top; j < rect.bottom; j++) {

            myfile << pixels[j * screenHeight + i] << '\n';

        }
    }

    myfile.close();
    free(pixels);

    cout << "Saved as file.txt\n";

    // success sound
    MessageBeep(MB_ICONASTERISK);
}

void LoadFile(HWND hwnd) {
    glBegin(GL_POINTS);
    RECT rect = {0};
    ifstream myfile;
    myfile.open("file.txt");
    GetWindowRect(hwnd, &rect);

    for (int i = rect.left; i < rect.right; i++) {
        for (int j = rect.top; j < rect.bottom; j++) {
            uint32_t data;
            myfile >> data;

            uint32_t red = (data >> 16) & 0xff;
            uint32_t green = (data >> 8) & 0xff;
            uint32_t blue = data & 0xff;

            glColor3f(red, green, blue);
            glVertex2d(i, j);
        }
    }

    myfile.close();
    glEnd();
    glFlush();
    cout << "Loaded from File.txt\n";
    MessageBeep(MB_ICONASTERISK);
    // for errors sound
//    MessageBeep(MB_ICONHAND);
}

/// Choose Color from pallet "e. Give me option to choose shape color before drawing from menu"
#define Color_Button 101
const int palletX = 0;
const int palletY = 0;
const int palletWidth = 40;
const int palletHigth = 40;
const int NpalletWidth = 6;
const int NpalletHigth = 2;
const int stepright = palletWidth + 10;
const int stepdown = palletHigth + 10;

void AddColorpalletWindow(HWND hwnd) {

    for (int y = palletY; y - palletY < NpalletHigth; y++) {
        for (int x = palletX; x - palletX < NpalletWidth; x++) {

            CreateWindowW(L"BUTTON", NULL,
                          WS_VISIBLE | WS_CHILD,
                          x * stepright, y * stepdown, palletWidth, palletHigth, hwnd, (HMENU) Color_Button, NULL,
                          NULL);

            // TODO Choose color for each button
            /// DrawRect(hwnd,  x * stepright, y * stepdown,x * stepright + palletWidth,y * stepdown + palletHigth , RGB(0,255,0));

        }
    }
}

void getColor(HWND hwnd) {

    MessageBeep(MB_ICONASTERISK);
}


/// clear screen "f. Implement item to clear screen from shapes"
#define Clear_Screen 454

void clearScreen(HWND hwnd) {
    //TODO Clear Screen
    hwnd = CreateWindowW(L"Main_Window", L"Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                         CW_USEDEFAULT, 368, 248, NULL, NULL, 0, NULL);

    MessageBeep(MB_ICONASTERISK);
}


void swap(int &x1, int &y1, int &x2, int &y2) {
    int tmp = x1;
    x1 = x2;
    x2 = tmp;
    tmp = y1;
    y1 = y2;
    y2 = tmp;
}

int Round(double x) {
    return (int) (x + 0.5);
}

void DrawLine1(int x1, int y1, int x2, int y2) {
    glBegin(GL_POINTS);
    glColor3f(1, 0, 0);
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (abs(dy) <= abs(dx)) {
        if (x1 > x2)swap(x1, y1, x2, y2);
        glVertex2d(x1, y1);
        int x = x1;
        while (x < x2) {
            x++;
            double y = y1 + (double) (x - x1) * dy / dx;
            glVertex2d(x, y);
        }
    } else {
        if (y1 > y2)swap(x1, y1, x2, y2);
        glVertex2d(x1, y1);
        int y = y1;
        while (y < y2) {
            y++;
            double x = x1 + (double) (y - y1) * dx / dy;
            glVertex2d(x, y);
        }
    }
    glEnd();
    glFlush();
}

void Draw8Points(int xc, int yc, int x, int y) {
    glVertex2d(xc + x, yc + y);
    glVertex2d(xc + x, yc - y);
    glVertex2d(xc - x, yc - y);
    glVertex2d(xc - x, yc + y);
    glVertex2d(xc + y, yc + x);
    glVertex2d(xc + y, yc - x);
    glVertex2d(xc - y, yc - x);
    glVertex2d(xc - y, yc + x);
}

void DrawCircle1(int xc, int yc, int R) {
    glBegin(GL_POINTS);
    glColor3f(0, 0, 1);
    int x = 0;
    double y = R;
    Draw8Points(xc, yc, 0, R);
    while (x < y) {
        x++;
        y = sqrt((double) R * R - x * x);
        Draw8Points(xc, yc, x, Round(y));
    }
    glEnd();
    glFlush();
}

HGLRC InitOpenGl(HDC hdc) {
    PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd
            1,                     // version number
            PFD_DRAW_TO_WINDOW |   // support window
            PFD_SUPPORT_OPENGL |   // support OpenGL
            PFD_DOUBLEBUFFER,      // double buffered
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
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SwapBuffers(hdc);
}

void EndOpenGl(HGLRC glrc) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(glrc);
}

LRESULT WINAPI
MyWndProc(HWND
          hwnd,
          UINT mcode, WPARAM
          wp,
          LPARAM lp
) {
    static HDC hdc;
    static HGLRC glrc;
    switch (mcode) {
        //action listener
        case WM_COMMAND:
            switch (wp) {
                case Save_File:
                    SwapBuffers(hdc);
                    SaveFile(hwnd);
                    SwapBuffers(hdc);
                    break;
                case Load_File:
                    LoadFile(hwnd);
                    glFlush();
                    SwapBuffers(hdc);
                    break;
                case Color_Button:
                    getColor(hwnd);
                    break;
                case Clear_Screen:
                    clearScreen(hwnd);
                    MessageBeep(MB_ICONASTERISK);
                    break;
            }
            break;
        case WM_CREATE:
            hdc = GetDC(hwnd);
            glrc = InitOpenGl(hdc);

            // add menu
            AddMenu(hwnd);

            // add GUI to main window
            CreateWindowW(L"BUTTON", L"Clear",
                          WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                          100, 100, 50, 50, hwnd, (HMENU) Clear_Screen, NULL, NULL);
            AddColorpalletWindow(hwnd);

            break;
        case WM_SIZE:
            AdjustWindowFor2D(hdc, LOWORD(lp), HIWORD(lp)
            );
            break;
        case WM_LBUTTONDOWN:
            DrawCircle1(400, 400, 100);
            DrawLine1(400, 400, 200, 200);
            glFlush();

            SwapBuffers(hdc);
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            EndOpenGl(glrc);
            ReleaseDC(hwnd, hdc
            );
            PostQuitMessage(0);
            break;
        default:
            return
                    DefWindowProc(hwnd, mcode, wp, lp
                    );
    }
    return 0;
}

int APIENTRY
WinMain(HINSTANCE
        hinst,
        HINSTANCE pinst, LPSTR
        cmd,
        int nsh
) {
    WNDCLASS wc;
    wc.
            cbClsExtra = wc.cbWndExtra = 0;
    wc.
            hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
    wc.
            hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.
            hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.
            hInstance = hinst;
    wc.
            lpfnWndProc = MyWndProc;
    wc.
            lpszClassName = reinterpret_cast<LPCSTR>(L"MyClass");
    wc.
            lpszMenuName = NULL;
    wc.
            style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow(reinterpret_cast<LPCSTR>(L"MyClass"), reinterpret_cast<LPCSTR>(L"My First Window"),
                             WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, screenWidth, screenHeight, NULL, NULL, hinst,
                             0);
    ShowWindow(hwnd, nsh
    );
    UpdateWindow(hwnd);
    MSG msg;
    while (
            GetMessage(&msg, NULL,
                       0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}