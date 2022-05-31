//
//#include "bezierFilling.h"
//
//
//
//void DrawBezierCurve(Point p1, Point p2, Point p3, Point p4, GLfloat *drawingColor) {
//    double a0 = p1.x,
//            a1 = -3 * p1.x + 3 * p2.x,
//            a2 = 3 * p1.x - 6 * p2.x + 3 * p3.x,
//            a3 = -p1.x + 3 * p2.x - 3 * p3.x + p4.x;
//    double b0 = p1.y,
//            b1 = -3 * p1.y + 3 * p2.y,
//            b2 = 3 * p1.y - 6 * p2.y + 3 * p3.y,
//            b3 = -p1.y + 3 * p2.y - 3 * p3.y + p4.y;
//    for (double t = 0; t <= 1; t += 0.001) {
//        double t2 = t * t, t3 = t2 * t;
//        double x = a0 + a1 * t + a2 * t2 + a3 * t3;
//        double y = b0 + b1 * t + b2 * t2 + b3 * t3;
//        glVertex2d(x, y);
//    }
//}
//
//void FillRectangleWithBezier(int x1, int y1, int x3, int y3, GLfloat *drawingColor) {
//    glBegin(GL_POINTS);
//    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);
//
//    for (int y = y3; y <= y1; y++)
//        DrawBezierCurve(Point(x1, y), Point(x1, y), Point(x3, y), Point(x3, y), drawingColor);
//
//    glEnd();
//    glFlush();
//}
//
//
//LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
//{
//    HDC hdc;
//    static Point p[2];
//    static int index = 0;
//    switch (mcode)
//    {
//        case WM_LBUTTONDOWN:
//            p[index] = Point(LOWORD(lp), HIWORD(lp));
//            if (index == 1)
//            {
//                hdc = GetDC(hwnd);
//                DrawRectangle(hdc, p[0].x, p[0].y, p[1].x, p[1].y, RGB(255, 0, 0));
//                FillRectangleWithBezier(hdc, p[0].x, p[0].y, p[1].x, p[1].y, RGB(255, 0, 0));
//                ReleaseDC(hwnd, hdc);
//            }
//            index++;
//            break;
//        case WM_CLOSE:
//            DestroyWindow(hwnd);
//            break;
//        case WM_DESTROY:
//            PostQuitMessage(0);
//            break;
//        default:
//            return DefWindowProc(hwnd, mcode, wp, lp);
//    }
//    return 0;
//}