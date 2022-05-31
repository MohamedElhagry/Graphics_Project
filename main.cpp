#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLu.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "DrawSimpleShapes.h"
#include "hermitFilling.h"
#include "fillingQuarter.h"
#include "convex.h"
#include "nonConvex.h"
#include "bezierFilling.h"
#include "Filling Utilities.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

using namespace std;


/// init painter
const int toolsHigth = 100;
const int bufferSize = 15;
const int screenWidth = 800;
const int screenHeight = 600 - toolsHigth;
GLfloat drawingColor[] = {1.0f, 1.0f, 0.0f};
RECT currColor = {340, 10, 380, 50};
GLfloat backgroundColor[] = {1.0f, 1.0f, 0.0f};

/// create menu
HMENU hmenu;
#define Save_File 1
#define Load_File 2
#define Clear_Screen 454
#define Default 222
#define WHITE_BK 11111

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
    AppendMenuW(ToolsMenu, MF_STRING, WHITE_BK, L"White Background");

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
GLfloat toolsShading[] = {0.75f, 0.75f, 0.75f};
GLfloat toolsBKColor[] = {0.97f, 0.97f, 0.97f};
GLfloat Colors[NpalletWidth * NpalletHigth][3] = {{1.0f, 0,     0},
                                                  {0,    1.0f,  0},
                                                  {0,    0,     1.0f},
                                                  {1.0f, 1.0,   0},
                                                  {0,    1.0f,  1.0f},
                                                  {0.44, 0.3,   0.1},
                                                  {0.6,  0.8,   0.9},
                                                  {0.22, 0.13,  0.67},
                                                  {0.86, 0.766, 0.43},
                                                  {0.1,  0.8,   0},
                                                  {0,    0.66,  0.5},
                                                  {0.22, 0.11,  0.65}};
vector<vector<int>> colorButtons;

void drawBlock(int x1, int y1, int x3, int y3, vector<vector<int>> &buttons, GLfloat *c) {
    FillRectangleWithHermite(x1, y1, x3, y3, c);
    for (int i = 1; i < 3; i++) {
        drawRectangle(x1 - i, y1 - i, x3 + i, y3 + i, toolsShading);
    }
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
const int NshapesWidth = 5;
const int NshapesHigth = 2;
const int shapesright = shapesWidth + 10;
const int shapesdown = shapesHigth + 5;
const int shapesNum = 10;
const LPCTSTR shapes[shapesNum] = {reinterpret_cast<LPCTSTR const>(L"Line"), reinterpret_cast<LPCTSTR const>(L"Square"),
                                   reinterpret_cast<LPCTSTR const>(L"Rectangle"),
                                   reinterpret_cast<LPCTSTR const>(L"Ellipse"),
                                   reinterpret_cast<LPCTSTR const>(L"Circle"),
                                   reinterpret_cast<LPCTSTR const>(L"Fill Circle"),
                                   reinterpret_cast<LPCTSTR const>(L"Fill"),
                                   reinterpret_cast<LPCTSTR const>(L"FloodFill"),
                                   reinterpret_cast<LPCTSTR const>(L"Curve"),
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
            shapeBounds.push_back(
                    {shapesX + x * shapesright, shapesY + y * shapesdown, (shapesX + x * shapesright) + shapesWidth,
                     (shapesY + y * shapesdown) + shapesHigth});
            i++;
        }
    }

    FillRectangleWithHermite(currColor.left, currColor.top, currColor.right, currColor.bottom, drawingColor);
    for (int i = 1; i < 5; i++) {
        drawRectangle(currColor.left - i, currColor.top - i, currColor.right + i, currColor.bottom + i, toolsShading);
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

void changeBackground(HWND hwnd, int r, int g, int b) {
    HBRUSH brush = CreateSolidBrush(RGB(r, g, b));
    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR) brush);
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


enum ShapeType {
    CircleS, RectangleS, EllipseS, SquareS, PolygonS, None
};

LRESULT WINAPI
MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp) {
    static HDC hdc;
    static HGLRC glrc;
    static int x, y, choice, choice2;
    static Point points[bufferSize];
    static bool inProcess = false;
    static bool defaultAlgorithms = false;
    static int target, counter;
    static int n;
    static ShapeType lastShape;
    static double R, A, B;
    static int xc, yc;
    static int quarter;
    static vector<Point> pointsVec;


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
                case WHITE_BK:
                    changeBackground(hwnd, 255, 255, 255);
                    break;
                case Shape_choice:
                    cout << "shapes\n";
                    cout.flush();
                    x = LOWORD(lp), y = HIWORD(lp);
                    choice = chooseTool(x, y, shapeBounds);
                    cout << chooseTool(x, y, shapeBounds) << '\n';
                    switch (choice) {
                        case 0 :        /// Line
                            inProcess = true;
                            cout << "Please choose the algorithm to draw your Line\n";
                            cout << "1-DDA, 2-MidPoint, 3-parametric\n";
                            cin >> choice2;

                            target = 2;
                            cout << "Please click on two points for the line\n";
                            break;
                        case 1:       /// Square
                            inProcess = true;

                            target = 2;
                            cout << "Please click on two points for the square\n";
                            break;
                        case 2:     /// Rectangle
                            inProcess = true;

                            target = 2;
                            cout << "Please click on two points for the rectangle\n";
                            break;
                        case 3:     /// Ellipse
                            inProcess = true;
                            cout << "Please choose the algorithm to draw your Ellipse\n";
                            cout << "1-Direct, 2-Polar, 3-MidPoint\n";
                            cin >> choice2;

                            target = 3;
                            cout << "Please click on three points for the ellipse\n";
                            break;
                        case 4:     /// Circle
                            inProcess = true;
                            cout << "Please choose the algorithm to draw your Circle\n";
                            cout << "1-Direct, 2-Polar, 3- Iterative polar, 4-Midpoint, 5-Modified Midpoint\n";
                            cin >> choice2;

                            target = 2;
                            cout << "Please click on two points for the circle\n";
                            break;
                        case 5:     /// Fill Circle
                            inProcess = true;
                            cout << "Please choose wether you want to fill the circle with lines or circles\n";
                            cout << "1-With lines, 2-With Circles\n";

                            cin >> choice2;

                            target = 0;
                            break;
                        case 6:     /// Convex and Non Convex Filling
                            inProcess = true;
                            cout << "Please choose the algorithm to fill your polygon\n";

                            cin >> choice2;
                            break;
                        case 7:     /// FloodFill
                            inProcess = true;
                            cout << "Please click on a point to start the floodfill\n";

                            break;
                        case 8:     /// Curve
                            inProcess = true;
                            cout << "Please choose the algorithm to draw your spline\n";

                            cout << "Please click on two points for the line\n";
                            break;
                        case 9:     /// Clip
                            inProcess = true;
                            cout << "Please choose the type of clipping window\n";
                            cout << "1 - Rectangular,2 - square, 3 -circle\n";
                            cin >> choice2;

                            target = 2;
                            break;
                        default:
                            target = 0;
                            break;
                    }
                    cout.flush();
                    break;
                case Default:

                    if (defaultAlgorithms == 0)
                        defaultAlgorithms = 1;
                    else
                        defaultAlgorithms = 0;
                    break;
                    /// add shapes drawing here
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
            //  String s;
            addToolsSction();
            // add shapes
            addShapesButtons(hwnd);
            break;
        case WM_SIZE:
            AdjustWindowFor2D(hdc, LOWORD(lp), HIWORD(lp));
            break;
        case WM_LBUTTONDOWN:
            x = LOWORD(lp), y = HIWORD(lp);
            if (y < toolsHigth) {
                int choice = chooseTool(x, y, colorButtons);
                if (choice < colorButtons.size()) {
                    drawingColor[0] = Colors[choice][0];
                    drawingColor[1] = Colors[choice][1];
                    drawingColor[2] = Colors[choice][2];
                    FillRectangleWithHermite(currColor.top, currColor.left, currColor.right, currColor.bottom,
                                             drawingColor);
                }
            } else {
                if (inProcess) {
                    points[counter++] = Point(x, y);

                    if (counter == target) {
                        counter = 0;
                        inProcess = false;

                        switch (choice) {
                            case 0 :        /// Line
                                inProcess = false;
                                //"1-DDA, 2-MidPoint, 3-parametric\n";

                                if (choice2 == 1) {
                                    drawLineDDA(points[0].x, points[0].y, points[1].x, points[1].y, drawingColor);
                                } else if (choice == 2) {
                                    drawLineMidPoint(points[0].x, points[0].y, points[1].x, points[1].y, drawingColor);
                                } else if (choice == 3) {
                                    drawLineParametric(points[0].x, points[0].y, points[1].x, points[1].y,
                                                       drawingColor);
                                }
                                break;
                            case 1:       /// Square


                                drawRectangle(points[0].x, points[0].y, points[1].x, points[1].y, drawingColor);
                                break;
                            case 2:     /// Rectangle


                                drawRectangle(points[0].x, points[0].y, points[1].x, points[1].y, drawingColor);
                                break;
                            case 3:     /// Ellipse

                                //"1-Direct, 2-Polar, 3-MidPoint\n";
                                R = (points[1].x - points[0].x) * (points[1].x - points[0].x) +
                                    (points[1].y - points[0].y) * (points[1].y - points[0].y);
                                B = (points[2].x - points[0].x) * (points[2].x - points[0].x) +
                                    (points[2].y - points[0].y) * (points[2].y - points[0].y);;
                                if (choice2 == 1) {
                                    //drawEllipse(points[0].x, points[0].y, points[1].x, points[1].y, drawingColor);
                                } else if (choice == 2) {
                                    drawEllipsePolar(points[0].x, points[0].y, A, B, drawingColor);
                                } else if (choice == 3) {
                                    drawEllipseMidPoint(points[0].x, points[0].y, A, B, drawingColor);
                                } else {
                                    drawEllipsePolar(points[0].x, points[0].y, A, B, drawingColor);
                                }

                                break;
                            case 4:     /// Circle


                                //"1-Direct, 2-Polar, 3- Iterative polar, 4-Midpoint, 5-Modified Midpoint";
                                R = (points[1].x - points[0].x) * (points[1].x - points[0].x) +
                                    (points[1].y - points[0].y) * (points[1].y - points[0].y);
                                if (choice2 == 1) {
                                    drawCircle(points[0].x, points[0].y, R, drawingColor);
                                } else if (choice2 == 2) {
                                    drawCirclePolar(points[0].x, points[0].y, R, drawingColor);
                                } else if (choice2 == 3) {
                                    drawCirclePolarIterative(points[0].x, points[0].y, R, drawingColor);
                                } else if (choice2 == 4) {
                                    drawCircleMidPoint(points[0].x, points[0].y, R, drawingColor);
                                } else if (choice == 5) {
                                    drawCircleMidPointModified(points[0].x, points[0].y, R, drawingColor);
                                } else {
                                    drawCircle(points[0].x, points[0].y, R, drawingColor);
                                }

                                break;
                            case 5:     /// Fill Circle

                                //"1-With lines, 2-With Circles\n";
                                R = (points[1].x - points[0].x) * (points[1].x - points[0].x) +
                                    (points[1].y - points[0].y) * (points[1].y - points[0].y);
                                if (choice2 == 1) {
                                    FillQuarter(points[0].x, points[0].y, R, drawingColor, quarter);
                                } else if (choice2 == 2) {

                                } else {
                                    FillQuarter(points[0].x, points[0].y, R, drawingColor, quarter);
                                }
                                target = 0;
                                break;
                            case 6:     /// Convex and Non Convex Filling

                                cout << "1-Convex polygon filling, 2-general polygon filling\n";
                                R = (points[1].x - points[0].x) * (points[1].x - points[0].x) +
                                    (points[1].y - points[0].y) * (points[1].y - points[0].y);
                                if (choice2 == 1) {
                                    FillQuarter(points[0].x, points[0].y, R, drawingColor, quarter);
                                } else if (choice2 == 2) {

                                } else {
                                    FillQuarter(points[0].x, points[0].y, R, drawingColor, quarter);
                                }

                                break;
                            case 7:     /// FloodFill

                                cout << "Please click on a point to start the floodfill\n";
                                //"1-With lines, 2-With Circles\n";
                                R = (points[1].x - points[0].x) * (points[1].x - points[0].x) +
                                    (points[1].y - points[0].y) * (points[1].y - points[0].y);
                                if (choice2 == 1) {
                                    FillQuarter(points[0].x, points[0].y, R, drawingColor, quarter);
                                } else if (choice2 == 2) {

                                } else {
                                    FillQuarter(points[0].x, points[0].y, R, drawingColor, quarter);
                                }
                                break;
                            case 8:     /// Curve

                                cout << "Please choose the algorithm to draw your spline\n";
                                R = (points[1].x - points[0].x) * (points[1].x - points[0].x) +
                                    (points[1].y - points[0].y) * (points[1].y - points[0].y);
                                if (choice2 == 1) {
                                    FillQuarter(points[0].x, points[0].y, R, drawingColor, quarter);
                                } else if (choice2 == 2) {

                                } else {
                                    FillQuarter(points[0].x, points[0].y, R, drawingColor, quarter);
                                }
                                break;
                            case 9:     /// Clip
                                //"1-With lines, 2-With Circles\n";
                                R = (points[1].x - points[0].x) * (points[1].x - points[0].x) +
                                    (points[1].y - points[0].y) * (points[1].y - points[0].y);
                                if (choice2 == 1) {
                                    FillQuarter(points[0].x, points[0].y, R, drawingColor, quarter);
                                } else if (choice2 == 2) {

                                } else {
                                    FillQuarter(points[0].x, points[0].y, R, drawingColor, quarter);
                                }
                                break;
                            default:
                                target = 0;
                                break;
                        }

                    }
                }
            }
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