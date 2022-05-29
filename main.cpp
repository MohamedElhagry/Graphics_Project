#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLu.h>
#include <math.h>
#include <iostream>
#include <fstream>

#include "Clipping algorithms.cpp"

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

void AddMenu(HWND hwnd) {
    hmenu = CreateMenu();
    HMENU FileMenu = CreateMenu();
    AppendMenuW(hmenu, MF_POPUP, (UINT_PTR) FileMenu, L"File");

    AppendMenuW(FileMenu, MF_STRING, Save_File, L"Save File");
    AppendMenuW(FileMenu, MF_STRING, Load_File, L"Load File");
    // AppendMenuW(FileMenu, MF_SEPARATOR, NULL, NULL);

    SetMenu(hwnd, hmenu);
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
#define Color_Button 101

const int palletX = 0;
const int palletY = 0;
const int palletWidth = 40;
const int palletHigth = 40;
const int NpalletWidth = 6;
const int NpalletHigth = 2;
const int stepright = palletWidth + 10;
const int stepdown = palletHigth + 10;

void AddColorPalletWindow(HWND hwnd) {
    for (int y = palletY; y - palletY < NpalletHigth; y++) {
        for (int x = palletX; x - palletX < NpalletWidth; x++) {

            CreateWindowW(L"BUTTON", NULL,
                          WS_VISIBLE | WS_CHILD,
                          x * stepright, y * stepdown, palletWidth, palletHigth, hwnd, (HMENU) Color_Button, NULL,
                          NULL);

            // TODO Choose color for each button


        }
    }
}

void drawRectangles() {
    glColor3f(0,1,1);
    for (int y = palletY; y - palletY < NpalletHigth; y++) {
        for (int x = palletX; x - palletX < NpalletWidth; x++) {
            float xStart = x * stepright;
            float yStart = y * stepdown;
            glRectd(xStart, yStart, xStart + palletWidth, yStart + palletHigth);
        }
    }
    glFlush();
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
#define Clear_Screen 454

void clearScreen() {
    glBegin(GL_POINTS);


    /// global background color
    glColor3f(backgroundColor[0], backgroundColor[1], backgroundColor[2]);

    for (int i = 0; i < screenHeight; i++) {
        for (int j = 0; j < screenWidth; j++) {
            glVertex2d(j, i);
        }
    }

    cout << "Screen Cleared\n";
    glEnd();
    glFlush();

    MessageBeep(MB_ICONASTERISK);
}


HGLRC InitOpenGl(HDC hdc) {
    PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd
            1,                     // version number
            PFD_DRAW_TO_WINDOW |   // support window
            PFD_SUPPORT_OPENGL    // support OpenGL
            // |PFD_DOUBLEBUFFER,      // double buffered
            ,
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
    static int x1, y1, x2, y2;
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
                case Clear_Screen:
                    clearScreen();
                    glFlush();
                    SwapBuffers(hdc);
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
                          WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON ,
                          100, 100, 50, 50, hwnd, (HMENU) Clear_Screen, NULL, NULL);
            //AddColorPalletWindow(hwnd);

            break;
        case WM_SIZE:
            AdjustWindowFor2D(hdc, LOWORD(lp), HIWORD(lp));
            break;
        case WM_LBUTTONDOWN:
            x1 = LOWORD(lp);
            y1 = HIWORD(lp);


            break;
        case WM_RBUTTONDOWN:
            drawRectangles();
            glFlush();
            break;
        case WM_MOUSEMOVE:
            DrawPoint(LOWORD(lp), HIWORD(lp), drawingColor);
            //glFlush();
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

int APIENTRY
WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmd, int nsh) {
    WNDCLASS wc;
    wc.cbClsExtra = wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = hinst;
    wc.lpfnWndProc = MyWndProc;
    wc.lpszClassName = reinterpret_cast<LPCSTR>(L"MyClass");
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow(reinterpret_cast<LPCSTR>(L"MyClass"), reinterpret_cast<LPCSTR>(L"Paintet"),
                             WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, screenWidth, screenHeight,
                             NULL, NULL, hinst,
                             0);
    ShowWindow(hwnd, nsh);
    ShowCursor(false);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}