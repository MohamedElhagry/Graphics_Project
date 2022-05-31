//
////#include "Functions.h"
//
//#ifndef HERMITEFILLING
//#define HERMITEFILLING
//
//
//void DrawHermiteCurve(Point p1, Point T1, Point p2, Point T2, GLfloat *drawingColor) {
//
//
//    double a0 = p1.x,
//            a1 = T1.x,
//            a2 = -3 * p1.x - 2 * T1.x + 3 * p2.x - T2.x,
//            a3 = 2 * p1.x + T1.x - 2 * p2.x + T2.x;
//    double b0 = p1.y,
//            b1 = T1.y,
//            b2 = -3 * p1.y - 2 * T1.y + 3 * p2.y - T2.y,
//            b3 = 2 * p1.y + T1.y - 2 * p2.y + T2.y;
//    for (double t = 0; t <= 1; t += 0.001) {
//        double t2 = t * t, t3 = t2 * t;
//        double x = a0 + a1 * t + a2 * t2 + a3 * t3;
//        double y = b0 + b1 * t + b2 * t2 + b3 * t3;
//        glVertex2d(round(x), round(y));
//    }
//
//}
//
//void FillRectangleWithHermite(int x1, int y1, int x3, int y3, GLfloat *drawingColor) {
//    glBegin(GL_POINTS);
//    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);
//
//    Point T1(0, 0);
//    Point T2(0, 0);
//    for (int x = x1; x <= x3; x++)
//        DrawHermiteCurve(Point(x, y1), T1, Point(x, y3), T2, drawingColor);
//
//    glEnd();
//    glFlush();
//}
//
//#endif
