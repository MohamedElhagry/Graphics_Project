#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLu.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include "DrawSimpleShapes.h"
#include "hermitFilling.h"
#include "fillingQuarter.h"
#include "convex.h"
#include "nonConvex.h"
#include "bezierFilling.h"
#include "Filling Utilities.h"
#include "Clipping algorithms.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

using namespace std;


/// init painter
const int toolsHigth = 100;
const int bufferSize = 15;
const int screenWidth = 800;
const int screenHeight = 600 - toolsHigth;
GLfloat drawingColor[] = {1.0f, 1.0f, 0.0f};
const int menuHeight = 49;
// defaults
GLfloat Green[] = {0, 1, 0};
GLfloat Black[] = {0, 0, 0};
GLfloat White[] = {1, 1, 1};
GLfloat Red[] = {1, 0, 0};
RECT currColor = {340, 10, 383, 50};
RECT Checkconsle = {350, 68, 373, 90};
GLfloat backgroundColor[] = {0.8f, 0.8f, 0.8f};

/// create menu
HMENU hmenu;
#define Save_File 1
#define Load_File 2
#define Clear_Screen 454
#define WHITE_BK 11111
#define SHOW_TOOLS 101

void AddMenu(HWND hwnd) {
    hmenu = CreateMenu();
    HMENU FileMenu = CreateMenu();
    HMENU ToolsMenu = CreateMenu();

    AppendMenuW(hmenu, MF_POPUP, (UINT_PTR) FileMenu, L"File");
    AppendMenuW(hmenu, MF_POPUP, (UINT_PTR) ToolsMenu, L"Tools");

    AppendMenuW(FileMenu, MF_STRING, Save_File, L"Save File");
    AppendMenuW(FileMenu, MF_STRING, Load_File, L"Load File");

    AppendMenuW(ToolsMenu, MF_STRING, Clear_Screen, L"Clear Screen");
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

    uint32_t *pixels = (uint32_t *) malloc(sizeof(uint32_t) * screenWidth * (screenHeight + toolsHigth));
    glReadPixels(0, 0, screenWidth, screenHeight + toolsHigth, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    ofstream myfile;
    myfile.open("file.txt");

    const int long len = screenWidth * (screenHeight + toolsHigth);
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

    RECT rect;
    GetWindowRect(hwnd, &rect);

    ifstream myfile;
    myfile.open("file.txt");
    uint32_t pixel;

    for (int y = screenHeight - menuHeight; y > toolsHigth; y--) {
        for (int x = 0; x < screenWidth; x++) {
            if (myfile >> pixel) {
                glColor4ubv((GLubyte *) &pixel);
                glVertex2d(x, y);
            }
        }
    }

    myfile.close();
    glEnd();
    glFlush();

    cout << "Loaded from file.txt\n";
    cout.flush();

    MessageBeep(MB_ICONASTERISK);
}

/// Choose Color from pallet "e. Give me option to choose shape color before drawing from menu"
void FillScreen(int xs, int xe, int ys, int ye, GLfloat *C) {

    glactivation();
    glColor3f(C[0], C[1], C[2]);

    for (int i = ys; i < ye; i++) {
        for (int j = xs; j < xe; j++) {
            glVertex2d(j, i);
        }
    }

    gldeactivation();
}

const int palletX = 10;
const int palletY = 6;
const int palletWidth = 48;
const int palletHigth = 45;
const int NpalletWidth = 6;
const int NpalletHigth = 2;
const int stepright = palletWidth + 7;
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
    FillRectangleWithHermite(x1 + 2, y1 + 2, x3 - 2, y3 - 2, c);
    for (int i = 0; i < 2; i++) {
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
    cout << "Choice number: " << i << '\n';
    cout.flush();
    return i;
}

void addColorsSection() {

    FillScreen(0, screenWidth, 0, toolsHigth, toolsBKColor);
    int i = 0, y, x;
    for (y = 0; y < NpalletHigth; y++) {
        for (x = 0; x < NpalletWidth; x++) {
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
vector<vector<int>> shapeButtons;

void updateGUI() {
    FillRectangleWithHermite(shapeButtons[6][0] + 24, shapeButtons[6][2] + 11,
                             shapeButtons[6][0] + 28,
                             shapeButtons[6][2] + 13, drawingColor);

    FillRectangleWithHermite(shapeButtons[6][0] + 21, shapeButtons[6][2] + 22,
                             shapeButtons[6][1] - 21,
                             shapeButtons[6][3] - 13, drawingColor);

    FillRectangleWithHermite(shapeButtons[7][0] + 24, shapeButtons[7][2] + 11,
                             shapeButtons[7][0] + 28,
                             shapeButtons[7][2] + 13, drawingColor);

    FillRectangleWithHermite(shapeButtons[7][0] + 21, shapeButtons[7][2] + 22,
                             shapeButtons[7][1] - 21,
                             shapeButtons[7][3] - 13, drawingColor);

    FillRectangleWithHermite(currColor.left, currColor.top, currColor.right, currColor.bottom,
                             drawingColor);

}

void drawOnShapeButtons() {
    // for testing a visuals
    /// Line
    for (int i = 0; i < 4; i++)
        drawLine(shapeButtons[0][0] + 10 - i, shapeButtons[0][2] + 8, shapeButtons[0][1] - 5 - i,
                 shapeButtons[0][3] - 5, Black);

    /// Square
    for (int i = 0; i < 3; i++)
        drawRectangle(shapeButtons[1][0] + 20 - i, shapeButtons[1][2] + 7 - i, shapeButtons[1][0] + 50 + i,
                      shapeButtons[1][2] + 33 + i, Black);

    /// Rectangle
    for (int i = 0; i < 3; i++)
        drawRectangle(shapeButtons[2][0] + 15 - i, shapeButtons[2][2] + 8 - i, shapeButtons[2][1] - 15 + i,
                      shapeButtons[2][3] - 8 + i, Black);

    /// Ellipse
    for (int i = 0; i < 3; i++)
        drawEllipsePolar(shapeButtons[3][0] + (shapesWidth / 2), shapeButtons[3][2] + (shapesHigth / 2), 25 + i,
                         10 + i, Black);

    /// Circle
    for (int i = 0; i < 3; i++)
        drawCircle(shapeButtons[4][0] + (shapesWidth / 2), shapeButtons[4][2] + (shapesHigth / 2), 13 + i, Black);

    /// Fill Circle
    drawCircle(shapeButtons[5][0] + (shapesWidth / 2), shapeButtons[5][2] + (shapesHigth / 2), 13, Black);

    FillQuarter(shapeButtons[5][0] + (shapesWidth / 2), shapeButtons[5][2] + (shapesHigth / 2), 13, Black, 2);

    /// Fill
    FillRectangleWithHermite(shapeButtons[6][0] + 22, shapeButtons[6][2] + 13,
                             shapeButtons[6][0] + 30,
                             shapeButtons[6][2] + (shapesHigth / 2), Black);

    FillRectangleWithHermite(shapeButtons[6][0] + 20, shapeButtons[6][2] + 17,
                             shapeButtons[6][1] - 20,
                             shapeButtons[6][3] - 5, Black);

    for (int i = 0; i < 4; i++)
        drawCircle(shapeButtons[6][1] - 20, shapeButtons[5][2] + (shapesHigth / 2) + 5, 8 + i, White);

    for (int i = 0; i < 1; i++)
        drawRectangle(shapeButtons[6][0] + 20 - i, shapeButtons[6][2] + 17 - i,
                      shapeButtons[6][1] - 20 + i,
                      shapeButtons[6][3] - 5 + i, toolsShading);


    /// FloodFill
    FillRectangleWithHermite(shapeButtons[7][0] + 22, shapeButtons[7][2] + 13,
                             shapeButtons[7][0] + 30,
                             shapeButtons[7][2] + (shapesHigth / 2), Black);

    FillRectangleWithHermite(shapeButtons[7][0] + 20, shapeButtons[7][2] + 17,
                             shapeButtons[7][1] - 20,
                             shapeButtons[7][3] - 5, Black);

    for (int i = 0; i < 1; i++)
        drawRectangle(shapeButtons[7][0] + 20 - i, shapeButtons[7][2] + 17 - i,
                      shapeButtons[7][1] - 20 + i,
                      shapeButtons[7][3] - 5 + i, toolsShading);

    /// Curve
    vector<Vector> p = {{shapeButtons[8][0] + 5, shapeButtons[8][2] + 5},
                        {shapeButtons[8][0] + 6, shapeButtons[8][2] + 40},
                        {shapeButtons[8][1] - 6, shapeButtons[8][3] - 40},
                        {shapeButtons[8][1] - 6, shapeButtons[8][3] - 7}};

    Vector T1(3 * (p[1][0] - p[0][0]), 3 * (p[1][1] - p[0][1]));
    Vector T2(3 * (p[3][0] - p[2][0]), 3 * (p[3][1] - p[2][1]));
    for (int i = 0; i < 10; i += 2) {
        T1[1] += i;
        T2[1] += i;
        DrawHermiteCurvee(p[0], T1, p[3], T2, Black);
    }

    /// Clip
    for (int i = 0; i < 6; i++)
        drawLine(shapeButtons[9][0] + 15 - i, shapeButtons[9][2] + 15, shapeButtons[9][1] - 10 - i,
                 shapeButtons[9][3] - 10, Red);

    for (int i = 0; i < 6; i++)
        drawLine(shapeButtons[9][0] + 15 - i, shapeButtons[9][3] - 10, shapeButtons[9][1] - 10 - i,
                 shapeButtons[9][2] + 15, Red);


    /// left line
    for (int i = 7; i <= 10; i++)
        drawLine(colorButtons.back()[1] + i, 0, colorButtons.back()[1] + i, toolsHigth, toolsShading);

    /// curr color square
    for (int i = 1; i < 3; i++) {
        drawRectangle(currColor.left - i, currColor.top - i, currColor.right + i, currColor.bottom + i, toolsShading);
    }

    /// right line and bottom
    drawLine(0, toolsHigth, screenWidth, toolsHigth, toolsShading);

    for (int i = 7; i <= 10; i++)
        drawLine(currColor.right + i, 0, currColor.right + i, toolsHigth, toolsShading);

    for (int i = 7; i <= 10; i++)
        drawLine(colorButtons.back()[1] + 7, currColor.bottom + i, currColor.right + 7, currColor.bottom + i,
                 toolsShading);

    /// Default on button (off by default with color Red)
    GLfloat ptr[] = {0, 1, 0};
    FillRectangleWithHermite(Checkconsle.left, Checkconsle.top, Checkconsle.right, Checkconsle.bottom,
                             ptr);

    for (int i = 1; i < 4; i++) {
        drawRectangle(Checkconsle.left - i, Checkconsle.top - i, Checkconsle.right + i, Checkconsle.bottom + i,
                      toolsShading);
    }

    updateGUI();
}

void addShapesButtons() {

    int i = 0;
    GLfloat BBK[] = {1.0, 1.0, 1.0};
    for (int y = 0; y < NshapesHigth; y++) {
        for (int x = 0; x < NshapesWidth; x++) {
            drawBlock(shapesX + x * shapesright, shapesY + y * shapesdown, (shapesX + x * shapesright) + shapesWidth,
                      (shapesY + y * shapesdown) + shapesHigth, shapeButtons, BBK);
            i++;
        }
    }
    drawOnShapeButtons();
}


/// clear screen "f. Implement item to clear screen from shapes"
void clearScreen() {
    /// global background color
    FillScreen(0, screenWidth, toolsHigth + 1, screenHeight, backgroundColor);
    cout << "Screen Cleared\n";
    cout.flush();
    MessageBeep(MB_ICONASTERISK);
}

HGLRC InitOpenGl(HDC hdc) {
    PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd
            1,                     // version number
            PFD_DRAW_TO_WINDOW |    // support window
            PFD_SUPPORT_OPENGL,             // support OpenGL
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
    glClearColor(backgroundColor[0] * 255, backgroundColor[1] * 255, backgroundColor[2] * 255, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EndOpenGl(HGLRC glrc) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(glrc);
}

LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp) {
    static HDC hdc;
    static HGLRC glrc;
    static int x, y, choice, choice2, choice3;
    static Point points[bufferSize];
    static bool inProcess = false;
    static int target, counter;
    static double R, A, B;
    static int quarter, num, dist;
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
                    break;
                case WHITE_BK: {
                    backgroundColor[0] = White[0];
                    backgroundColor[1] = White[1];
                    backgroundColor[2] = White[2];
                    FillScreen(0, screenWidth, toolsHigth + 1, screenHeight, backgroundColor);
                    cout << "Background Color is White" << endl;
                    break;
                }
                case Shape_choice:
                    FillRectangleWithHermite(Checkconsle.left, Checkconsle.top, Checkconsle.right, Checkconsle.bottom,
                                             Red);
                    SetCursor(LoadCursorA(NULL, IDC_WAIT));
                    drawRectangle(shapeButtons[choice][0] + 2, shapeButtons[choice][2] + 2, shapeButtons[choice][1] - 2,
                                  shapeButtons[choice][3] - 2, White);
                    choice = lp;
                    switch (choice) {
                        case 0 :        /// Line
                            inProcess = true;
                            target = 2;
                            cout << "1-DDA\n2-MidPoint\n3-parametric\n";
                            cin >> choice2;
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
                            target = 3;
                            cout << "1-Direct\n";

                            cout << "2-Polar\n3-MidPoint\n";
                            cin >> choice2;

                            cout << "Please click on three points for the ellipse\n";
                            break;
                        case 4:     /// Circle
                            inProcess = true;
                            cout << "Please choose the algorithm to draw your Circle\n";
                            cout << "1- Direct\n";
                            target = 2;
                            cout << "2- Polar\n3- Iterative polar\n4- Midpoint\n5- Modified Midpoint\n";
                            cin >> choice2;

                            cout << "Please click on two points for the circle\n";
                            break;
                        case 5:     /// Fill Circle
                            inProcess = true;
                            target = 0;
                            cout << "1-With lines\n";

                            cout << "2-With Circles\n";
                            cin >> choice2;

                            break;
                        case 6:     /// Convex and Non Convex Filling
                            inProcess = true;
                            cout << "Please choose the type of filling\n";
                            cout << "1-Convex polygon filling, 2-General Polygon filling\n";
                            cin >> choice2;

                            cout << "Please choose the number of points your polygon will have\n";
                            cin >> target;

                            cout << "please click on your polygon points\n";
                            break;
                        case 7:     /// FloodFill
                            inProcess = true;
                            target = 1;
                            cout << "Please click on a point to start the floodfill\n";
                            break;
                        case 8:     /// Curve
                            inProcess = true;
                            cout << "Please choose the number of points you want to be included in your spline\n";
                            cin >> target;
                            cout << "Please click on the points\n";
                            break;
                        case 9:     /// Clip
                            inProcess = true;
                            target = 2;
                            cout << "1- Rectangular\n";
                            cout << "2- square\n3- circle\n";
                            cin >> choice2;


                            if (choice2 == 1) {
                                target = 4;
                                cout << "Please enter the type of shape you want to clip\n";
                                cout << "1-Point\n2-Line\n3-Polygon\n";
                                cin >> choice3;


                                if (choice3 == 1) {
                                    target += 1;
                                    cout << "Please draw your rectangle then draw ";
                                    cout << "your point\n";
                                } else if (choice3 == 2) {
                                    target += 2;
                                    cout << "Please draw your rectangle then draw ";
                                    cout << "your line\n";
                                } else if (choice3 == 3) {
                                    cout << "Please enter the number of sides of the polygon\n";
                                    cin >> num;
                                    target += num;
                                    cout << "Please draw your rectangle then draw ";
                                    cout << "your polygon\n";
                                }

                                cout
                                        << "You draw the rectangle by selecting the left side, then top, then right, the bottom\n";
                            } else if (choice2 == 2) //square clipping and circle clipping
                            {

                                target = 2;
                                cout << "Please enter the type of shape you want to clip\n";
                                cout << "1-Point\n2-Line\n";
                                cin >> choice3;
                                if (choice3 == 1) {
                                    cout << "Please draw your square then draw your point\n";
                                    target += 1;
                                } else if (choice3 == 2) {
                                    cout << "Please draw your square then draw your line\n";
                                    target += 2;
                                }
                            } else if (choice2 == 3) {
                                target = 2;
                                cout << "Please enter the type of shape you want to clip\n";
                                cout << "1-Point\n2-Line\n";
                                cin >> choice3;
                                if (choice3 == 1) {
                                    cout << "Please draw your circle then draw your point\n";
                                    target += 1;
                                } else if (choice3 == 2) {
                                    cout << "Please draw your circle then draw your line\n";
                                    target += 2;
                                }

                            }

                            break;
                        default:
                            target = 0;
                            break;
                    }
                    SetCursor(LoadCursorA(NULL, IDC_CROSS));
                    FillRectangleWithHermite(Checkconsle.left, Checkconsle.top, Checkconsle.right, Checkconsle.bottom,
                                             Green);
                    drawRectangle(shapeButtons[choice][0] + 2, shapeButtons[choice][2] + 2, shapeButtons[choice][1] - 2,
                                  shapeButtons[choice][3] - 2, Black);
                    cout.flush();
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
            glactivation();

            // Color pallet
            addColorsSection();

            // add shapes
            addShapesButtons();

            gldeactivation();
            break;
        case WM_SIZE:
            AdjustWindowFor2D(hdc, LOWORD(lp), HIWORD(lp));
            break;
        case WM_LBUTTONDOWN:
            x = LOWORD(lp), y = HIWORD(lp);
            if (y < toolsHigth) {
                if (x < shapesX) {
                    int choice = chooseTool(x, y, colorButtons);
                    if (choice < colorButtons.size()) {
                        drawingColor[0] = Colors[choice][0];
                        drawingColor[1] = Colors[choice][1];
                        drawingColor[2] = Colors[choice][2];
                        updateGUI();
                    }
                } else {
                    int choice = chooseTool(x, y, shapeButtons);
                    if (choice < shapeButtons.size()) {
                        SendMessage(hwnd, WM_COMMAND, Shape_choice, choice);
                    }
                }
            } else {
                if (inProcess) {
                    if (target != counter)
                        points[counter++] = Point(x, y);

                    if (counter == target) {
                        counter = 0;

                        switch (choice) {
                            case 0 :        /// Line
                                //"1-DDA, 2-MidPoint, 3-parametric\n";

                                if (choice2 == 1) {
                                    drawLineDDA(points[0].x, points[0].y, points[1].x, points[1].y, drawingColor);
                                } else if (choice2 == 2) {
                                    drawLineMidPoint(points[0].x, points[0].y, points[1].x, points[1].y,
                                                     drawingColor);
                                } else if (choice2 == 3) {
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
                                A = sqrt((points[1].x - points[0].x) * (points[1].x - points[0].x));
                                B = sqrt((points[2].y - points[0].y) * (points[2].y - points[0].y));
                                if (choice2 == 1) {
                                    drawEllipse(points[0].x, points[0].y, A, B, drawingColor);
                                } else if (choice2 == 2) {
                                    drawEllipsePolar(points[0].x, points[0].y, A, B, drawingColor);
                                } else if (choice2 == 3) {
                                    drawEllipseMidPoint(points[0].x, points[0].y, A, B, drawingColor);
                                } else {
                                    drawEllipsePolar(points[0].x, points[0].y, A, B, drawingColor);
                                }

                                break;
                            case 4:     /// Circle


                                //"1-Direct, 2-Polar, 3- Iterative polar, 4-Midpoint, 5-Modified Midpoint";
                                R = sqrt((points[1].x - points[0].x) * (points[1].x - points[0].x) +
                                         (points[1].y - points[0].y) * (points[1].y - points[0].y));
                                if (choice2 == 1) {
                                    drawCircle(points[0].x, points[0].y, R, drawingColor);
                                } else if (choice2 == 2) {
                                    drawCirclePolar(points[0].x, points[0].y, R, drawingColor);
                                } else if (choice2 == 3) {
                                    drawCirclePolarIterative(points[0].x, points[0].y, R, drawingColor);
                                } else if (choice2 == 4) {
                                    drawCircleMidPoint(points[0].x, points[0].y, R, drawingColor);
                                } else if (choice2 == 5) {
                                    drawCircleMidPointModified(points[0].x, points[0].y, R, drawingColor);
                                } else {
                                    drawCircle(points[0].x, points[0].y, R, drawingColor);
                                }

                                break;
                            case 5:     /// Fill Circle
                                cout << "Please choose the quarter you want to fill\n";
                                cin >> quarter;
                                //"1-With lines, 2-With Circles\n";
                                if (choice2 == 1) {
                                    FillQuarter(points[0].x, points[0].y, R, drawingColor, quarter);
                                } else if (choice2 == 2) {
                                    FillQuarterWithCircles(points[0].x, points[0].y, R, drawingColor, quarter);
                                } else {
                                    FillQuarter(points[0].x, points[0].y, R, drawingColor, quarter);
                                }
                                target = 0;
                                break;
                            case 6:     /// Convex and Non Convex Filling

                                drawPolygon(points, target, drawingColor);
                                if (choice2 == 1) {
                                    FillPolygonC(points, target, drawingColor);
                                } else {
                                    FillPolygon(points, target, drawingColor);
                                }

                                break;
                            case 7:     /// FloodFill

                                //implement floodfill
                                floodFill(x, y, toolsHigth, screenHeight + toolsHigth,
                                          screenWidth, drawingColor, drawingColor);
                                break;
                            case 8:     /// Curve
                                cardinalSplines(points, target, 1, drawingColor);
                                break;
                            case 9:     /// Clip
                                if (choice2 == 1)//rectangular clipping
                                {
                                    if (choice3 == 1) {//clip point
                                        clipPoint(points[4].x, points[4].y, points[0].x, points[1].y, points[2].x,
                                                  points[3].y, drawingColor);
                                    } else if (choice3 == 2)// clip line
                                    {
                                        clipLine(points[4].x, points[4].y, points[5].x, points[5].y, points[0].x,
                                                 points[1].y, points[2].x, points[3].y, drawingColor);
                                    } else if (choice3 == 3)//clip polygon
                                    {
                                        clipPolygon(points + 4, target - 4, points[0].x, points[1].y, points[2].x,
                                                    points[3].y, drawingColor);
                                    }
                                } else if (choice2 == 2)//square clipping
                                {
                                    dist = sqrt((points[1].x - points[0].x) * (points[1].x - points[0].x) +
                                                (points[1].y - points[0].y) * (points[1].y - points[0].y));

                                    if (choice3 == 1) {//clip point
                                        clipPoint(points[2].x, points[2].y, points[0].x, points[0].y,
                                                  points[0].x + dist, points[0].y + dist, drawingColor);
                                    } else if (choice3 == 2)// clip line
                                    {
                                        clipLine(points[2].x, points[2].y, points[3].x, points[3].y, points[0].x,
                                                 points[0].y, points[0].x + dist, points[0].y + dist, drawingColor);
                                    }
                                } else if (choice2 == 3)//circular clipping
                                {
                                    R = sqrt((points[1].x - points[0].x) * (points[1].x - points[0].x) +
                                             (points[1].y - points[0].y) * (points[1].y - points[0].y));
                                    if (choice3 == 1) {
                                        clipPointFromCircle(points[2].x, points[2].y, points[0].x, points[0].y, R,
                                                            drawingColor);
                                    } else if (choice3 == 2) {
                                        clipLineFromCircle(points[2].x, points[2].y, points[3].x, points[3].y,
                                                           points[0].x, points[0].y, R, drawingColor);
                                    }
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
            return
                    DefWindowProc(hwnd, mcode, wp, lp);
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
    wc.hCursor = LoadCursorA(NULL, IDC_CROSS);
    RegisterClass(&wc);
    HWND hwnd = CreateWindow((LPCSTR) L"MyClass", (LPCSTR) "Paintit",
                             WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, 0, 0, screenWidth, screenHeight,
                             NULL, NULL, hinst,
                             0);
    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);
    SendMessage(hwnd, SHOW_TOOLS, 1, 1);        /// show all the tools

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}