//
// Created by Mohamed on 5/27/2022.
//

#include "Draw Simple Shapes.h"
using namespace std;


/// Simple Shapes

struct Point {
public:
    double x, y;

    Point(int x = 0, int y = 0) {
        this->x = x;
        this->y = y;
    }
};

void swap(int &x1, int &y1, int &x2, int &y2) {
    int tmp = x1;
    x1 = x2;
    x2 = tmp;
    tmp = y1;
    y1 = y2;
    y2 = tmp;
}


double max(double a, double b) {
    if (a > b)
        return a;
    return b;
}

int Round(double x) {
    return (int) (x + 0.5);
}

void drawPoint(int x, int y, GLfloat *drawingColor) {
    glBegin(GL_POINTS);
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);
    glVertex2d(x, y);

    glEnd();
    glFlush();
}


//Line algorithms

void drawLineMidPoint(int x1, int y1, int x2, int y2, GLfloat *drawingColor);

void drawLine(int x1, int y1, int x2, int y2, GLfloat *drawingColor) {
    drawLineMidPoint(x1, y1, x2, y2, drawingColor);
}

void drawLineDDA(int x1, int y1, int x2, int y2, GLfloat *drawingColor) {
    glBegin(GL_POINTS);
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);

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

void drawLineMidPoint(int x1, int y1, int x2, int y2, GLfloat *drawingColor) {
    glBegin(GL_POINTS);
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);

    if (x1 > x2) {
        swap(x1, y1, x2, y2);
    }

    int dx = x2 - x1;
    int dy = y2 - y1;
    int x = x1;
    int y = y1;

    if (abs(dx) >= abs(dy)) {

        if (dy > 0) {

            int d = dx - 2 * dy;
            int d1 = -2 * dy;
            int d2 = 2 * (dx - dy);
            glVertex2d(x, y);
            while (x < x2) {
                if (d >= 0) {
                    d += d1;
                } else {
                    y++;
                    d += d2;
                }
                x++;
                glVertex2d(x, y);
            }


        } else {//dy < 0

            int d = -dx - 2 * dy;
            int d1 = -2 * (dx + dy);
            int d2 = -2 * dy;
            glVertex2d(x, y);
            while (x < x2) {
                if (d >= 0) {
                    y--;
                    d += d1;
                } else {
                    d += d2;
                }
                x++;
                glVertex2d(x, y);
            }

        }
    } else { // slope > 1
        if (dy > 0) {

            int d = 2 * dx - dy;
            int d1 = 2 * (dx - dy);
            int d2 = 2 * dx;
            glVertex2d(x, y);
            while (y < y2) {
                if (d >= 0) {
                    x++;
                    d += d1;
                } else {
                    d += d2;
                }
                y++;
                glVertex2d(x, y);
            }

        } else {// dy < 0

            int d = -2 * dx - dy;
            int d1 = -2 * dx;
            int d2 = -2 * (dx + dy);
            glVertex2d(x, y);
            while (y > y2) {
                if (d >= 0) {
                    d += d1;
                } else {
                    x++;
                    d += d2;
                }
                y--;
                glVertex2d(x, y);
            }

        }
    }
    glEnd();
    glFlush();
}

void drawLineParametric(int x1, int y1, int x2, int y2, GLfloat *drawingColor) {
    glBegin(GL_POINTS);
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);

    int dx = x2 - x1;
    int dy = y2 - y1;
    double x, y;

    for (double t = 0.0; t <= 1.0; t += 1.0 / max(abs(dx), abs(dy))) {
        x = x1 + t * dx;
        y = y1 + t * dy;
        glVertex2d(Round(x), Round(y));
    }

    glEnd();
    glFlush();
}

//Circle algorithms
void draw8Points(int xc, int yc, int x, int y) {
    glVertex2d(xc + x, yc + y);
    glVertex2d(xc + x, yc - y);
    glVertex2d(xc - x, yc - y);
    glVertex2d(xc - x, yc + y);
    glVertex2d(xc + y, yc + x);
    glVertex2d(xc + y, yc - x);
    glVertex2d(xc - y, yc - x);
    glVertex2d(xc - y, yc + x);
}


void drawCircle(int xc, int yc, int R, GLfloat *drawingColor) {

    glBegin(GL_POINTS);
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);
    int x = 0;
    double y = R;
    draw8Points(xc, yc, 0, R);
    while (x < y) {
        x++;
        y = sqrt((double) R * R - x * x);
        draw8Points(xc, yc, x, Round(y));
    }

    glEnd();
    glFlush();
}

void drawCirclePolar(int xc, int yc, int R, GLfloat *drawingColor) {

    glBegin(GL_POINTS);
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);
    double x = R;
    double y = 0;
    double theta = 0;
    double dTheta = 1.0 / R;

    while (x > y) {
        theta += dTheta;
        x = R * cos(theta);
        y = R * sin(theta);
        draw8Points(xc, yc, Round(x), Round(y));
    }

    glEnd();
    glFlush();
}

void drawCirclePolarIterative(int xc, int yc, int R, GLfloat *drawingColor) {

    glBegin(GL_POINTS);
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);
    double x = R;
    double y = 0;

    double dTheta = 1.0 / R;
    double cdTheta = cos(dTheta);
    double sdTheta = sin(dTheta);

    while (x > y) {
        x = x * cdTheta - y * sdTheta;
        y = x * sdTheta + y * cdTheta;
        draw8Points(xc, yc, Round(x), Round(y));
    }

    glEnd();
    glFlush();
}

void drawCircleMidPoint(int xc, int yc, int R, GLfloat *drawingColor) {

    glBegin(GL_POINTS);
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);

    int x = 0;
    int y = R;
    int d = 1 - R;
    draw8Points(xc, yc, x, y);

    while (x < y) {

        if (d < 0) {
            d += 2 * x + 3;
            x++;
        } else {
            d += 2 * (x - y) + 5;
            x++;
            y--;
        }

        draw8Points(xc, yc, x, y);
    }


    glEnd();
    glFlush();
}

void drawCircleMidPointModified(int xc, int yc, int R, GLfloat *drawingColor) {

    glBegin(GL_POINTS);
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);

    int x = 0;
    int y = R;
    int d = 1 - R;
    int d1 = 3;
    int d2 = 5 - 2 * R;
    draw8Points(xc, yc, x, y);

    while (x < y) {

        if (d < 0) {
            d += d1;
            d2 += 2;
            x++;
        } else {
            d += d2;
            d2 += 4;
            x++;
            y--;
        }
        d1 += 2;

        draw8Points(xc, yc, x, y);
    }


    glEnd();
    glFlush();
}


//Ellipse Algorithms

void Draw4Points( int xc, int yc, int x, int y) {
    glVertex2d( xc + x, yc + y);
    glVertex2d( xc - x, yc + y);
    glVertex2d( xc + x, yc - y);
    glVertex2d( xc - x, yc - y);
}

void drawEllipsePolar(int xc, int yc, int A, int B, GLfloat *drawingColor) {
    glBegin(GL_POINTS);
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);

    double x = A;
    double y = 0;

    Draw4Points(xc, yc, Round(x), Round(y));
    int mx = A;
    if (B > mx)
        mx = B;

    double dtheta = 1.0 / mx;
    double stheta = sin(dtheta);
    double ctheta = cos(dtheta);
    int cntr = 0;

    while (cntr < 2 * mx) {
        double x1 = x * ctheta - (A * y * stheta / B);
        y = y * ctheta + (B * x * stheta / A);
        cntr++;

        x = x1;
        Draw4Points(xc, yc, Round(x), Round(y));
    }

    glEnd();
    glFlush();
}

void drawEllipseMidPoint(int xc, int yc, int A, int B, GLfloat *drawingColor) {
    glBegin(GL_POINTS);
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);

    int Asq = A * A;
    int Bsq = B * B;
    //7th quad
    int x = -A;
    int y = 0;
    int d = -4 * Bsq * A + 4 * Asq + Bsq;

    Draw4Points( xc, yc, x, y);
    while (abs(x) * Bsq > abs(y) * Asq) {
        if (d < 0) {
            d += 8 * Asq * y + 12 * Asq;
            y++;
        } else {
            d += 8 * Asq * y + 12 * Asq + 8 * Bsq * x + 8 * Bsq;
            x++;
            y++;
        }
        Draw4Points( xc, yc, x, y);
    }

    glEnd();
    glFlush();
}

void drawRectangle(int x1, int y1, int x3, int y3, GLfloat *c) {
    int x2 = x1, y2 = y3;
    int x4 = x3, y4 = y1;
    drawLine(x1, y1, x2, y2, c);
    drawLine(x2, y2, x3, y3, c);
    drawLine(x3, y3, x4, y4, c);
    drawLine(x4, y4, x1, y1, c);
}


/// Clipping algorithms

union OutCode {
    unsigned All: 4;
    struct {
        unsigned left: 1, top: 1, right: 1, bot: 1;
    };
};

OutCode getOutCode(double x, double y, int left, int top, int right, int bot) {
    OutCode out;
    out.All = 0;
    if (x < left)
        out.left = 1;
    else if (x > right)
        out.right = 1;
    if (y < top)
        out.top = 1;
    else if (y > bot)
        out.bot = 1;

    return out;
}

Point VIntersect(double xs, double ys, double xe, double ye, int x) {
    Point p;
    p.x = x;
    p.y = ys + (ye - ys) * (x - xs) / (xe - xs);
    return p;
}

Point HIntersect(double xs, double ys, double xe, double ye, int y) {
    Point p;
    p.y = y;
    p.x = xs + (xe - xs) * (y - ys) / (ye - ys);
    return p;
}

void clipPoint(int x, int y, int left, int top, int right, int bot, GLfloat *drawingColor) {
    glBegin(GL_POINTS);
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);
    if (x >= left && x <= right && y >= top && y <= bot) {
        glVertex2d(x, y);
    }
    glEnd();
    glFlush();
}

//Cohen Sutherland
void clipLine(int xs, int ys, int xe, int ye, int left, int top, int right, int bot, GLfloat *c) {
    double x1 = xs, y1 = ys, x2 = xe, y2 = ye;
    OutCode oc1 = getOutCode(x1, y1, left, top, right, bot);
    OutCode oc2 = getOutCode(x2, y2, left, top, right, bot);

    while ((oc1.All || oc2.All) && !(oc1.All & oc2.All)) {
        Point p;
        if (oc1.All) {
            if (oc1.left)
                p = VIntersect(x1, y1, x2, y2, left);
            else if (oc1.right)
                p = VIntersect(x1, y1, x2, y2, right);
            else if (oc1.bot)
                p = HIntersect(x1, y1, x2, y2, bot);
            else p = HIntersect(x1, y1, x2, y2, top);

            x1 = p.x;
            y1 = p.y;
            oc1 = getOutCode(x1, y1, left, top, right, bot);
        } else {
            if (oc2.left)
                p = VIntersect(x1, y1, x2, y2, left);
            else if (oc2.right)
                p = VIntersect(x1, y1, x2, y2, right);
            else if (oc2.bot)
                p = HIntersect(x1, y1, x2, y2, bot);
            else p = HIntersect(x1, y1, x2, y2, top);

            x2 = p.x;
            y2 = p.y;
            oc2 = getOutCode(x2, y2, left, top, right, bot);
        }
    }

    if (!(oc1.All | oc2.All)) {
        drawLine(x1, y1, x2, y2, c);
    }

}

bool inLeft(Point &p, int edge) {
    return p.x >= edge;
}

bool inRight(Point &p, int edge) {
    return p.x <= edge;
}

bool inTop(Point &p, int edge) {
    return p.y >= edge;
}

bool inBottom(Point &p, int edge) {
    return p.y <= edge;
}


void clipPolygon(Point *points, int n, int left, int top, int right, int bottom) {
//    vector<Vertex>

}


//Circular clipping window

//utilities
bool inside(int x, int y, int xc, int yc, double R) {
    return (x - xc) * (x - xc) + (y - yc) * (y - yc) <= R * R;
}

bool pointInsideLine(int x, int y, int xs, int ys, int xe, int ye) {
    if (xs > xe)
        swap(xs, xe);
    if (ys > ye)
        swap(ys, ye);

    return x <= xe && x >= xs && y >= ys && y <= ye;
}

//still under development
bool CIntersect(double xs, double ys, double xe, double ye, double xc, double yc, double R, Point &p1, Point &p2) {
    double dy = ye - ys;
    double dx = xe - xs;
    double D = (ys - yc - dy / dx * xs);

    double a = 1 + (dy * dy) / (dx * dx);
    double b = 2 * D * dy / dx - 2 * xc;
    double c = xc * xc + D * D - R * R;

    if (b * b < 4 * a * c)
        return false;


    // greater x value
    double x1 = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
    double y1 = ys + (x1 - xs) / dx * dy;

    //smaller x value
    double x2 = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
    double y2 = ys + (x2 - xs) / dx * dy;


    if (xs > xe) {
        swap(xs, xe);
        swap(ys, ye);
    }


    if (pointInsideLine(x1, y1, xs, ys, xe, ye)) {
        p1.x = x1;
        p1.y = y1;
    } else {
        p1.x = xs;
        p1.y = ys;
    }

    if (pointInsideLine(x2, y2, xs, ys, xe, ye)) {
        p2.x = x2;
        p2.y = y2;
    } else {
        p2.x = xe;
        p2.y = ye;
    }

    return true;
}

void clipPointFromCircle(int x, int y, int xc, int yc, int R, GLfloat *c) {
    if (inside(x, y, xc, yc, R))
        drawPoint(x, y, c);
}

void clipLineFromCircle(int xs, int ys, int xe, int ye, int xc, int yc, int R, GLfloat *c) {
    Point p1, p2;
    bool intersects = CIntersect(xs, ys, xe, ye, xc, yc, R, p1, p2);
    if (!intersects)
        return;

    drawCircle(xc, yc, R, c);
    drawLine(p1.x, p1.y, p2.x, p2.y, c);
}

/// Filling Utilites

/// Filling Quarter

void draw2Lines(int xc, int yc, int x, int y, GLfloat *drawingColor, int quarter) {
    switch (quarter) {
        case 1:
            drawLine(xc, yc, xc + x, yc + y, drawingColor);
            drawLine(xc, yc, xc + y, yc + x, drawingColor);
            break;
        case 2:
            drawLine(xc, yc, xc + x, yc - y, drawingColor);
            drawLine(xc, yc, xc + y, yc - x, drawingColor);
            break;
        case 3:
            drawLine(xc, yc, xc - y, yc - x, drawingColor);
            drawLine(xc, yc, xc - x, yc - y, drawingColor);
            break;
        case 4:
            drawLine(xc, yc, xc - y, yc + x, drawingColor);
            drawLine(xc, yc, xc - x, yc + y, drawingColor);
            break;
    }
}

void FillQuarter(int xc, int yc, int R, GLfloat *drawingColor, int quarter) {
    int x = 0, y = R;
    int d = 1 - R, d1 = 3, d2 = 5 - 2 * R;
    do {
        draw2Lines(xc, yc, x, y, drawingColor, quarter);
        if (d < 0) {
            d += d1;
            d2 += 2;
        } else {
            d += d2;
            d2 += 4;
            y--;
        }
        d1 += 2;
        x++;

    } while (x < y);
}

/// Hermite Filling
void DrawHermiteCurve(Point p1, Point T1, Point p2, Point T2, GLfloat *drawingColor) {

    double a0 = p1.x,
            a1 = T1.x,
            a2 = -3 * p1.x - 2 * T1.x + 3 * p2.x - T2.x,
            a3 = 2 * p1.x + T1.x - 2 * p2.x + T2.x;
    double b0 = p1.y,
            b1 = T1.y,
            b2 = -3 * p1.y - 2 * T1.y + 3 * p2.y - T2.y,
            b3 = 2 * p1.y + T1.y - 2 * p2.y + T2.y;
    for (double t = 0; t <= 1; t += 0.001) {
        double t2 = t * t, t3 = t2 * t;
        double x = a0 + a1 * t + a2 * t2 + a3 * t3;
        double y = b0 + b1 * t + b2 * t2 + b3 * t3;
        glVertex2d(round(x), round(y));
    }

}

void FillRectangleWithHermite(int x1, int y1, int x3, int y3, GLfloat *drawingColor) {
    glBegin(GL_POINTS);
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);

    Point T1(0, 0);
    Point T2(0, 0);
    for (int x = x1; x <= x3; x++)
        DrawHermiteCurve(Point(x, y1), T1, Point(x, y3), T2, drawingColor);

    glEnd();
    glFlush();
}

/// bezier filling

void DrawBezierCurve(Point p1, Point p2, Point p3, Point p4, GLfloat *drawingColor) {
    double a0 = p1.x,
            a1 = -3 * p1.x + 3 * p2.x,
            a2 = 3 * p1.x - 6 * p2.x + 3 * p3.x,
            a3 = -p1.x + 3 * p2.x - 3 * p3.x + p4.x;
    double b0 = p1.y,
            b1 = -3 * p1.y + 3 * p2.y,
            b2 = 3 * p1.y - 6 * p2.y + 3 * p3.y,
            b3 = -p1.y + 3 * p2.y - 3 * p3.y + p4.y;
    for (double t = 0; t <= 1; t += 0.001) {
        double t2 = t * t, t3 = t2 * t;
        double x = a0 + a1 * t + a2 * t2 + a3 * t3;
        double y = b0 + b1 * t + b2 * t2 + b3 * t3;
        glVertex2d(x, y);
    }
}

void FillRectangleWithBezier(int x1, int y1, int x3, int y3, GLfloat *drawingColor) {
    glBegin(GL_POINTS);
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);

    for (int y = y3; y <= y1; y++)
        DrawBezierCurve(Point(x1, y), Point(x1, y), Point(x3, y), Point(x3, y), drawingColor);

    glEnd();
    glFlush();
}


/// non-convex
struct EdgeRec {
    double x, slopeInv;
    int yMax;

    EdgeRec(double x = 0, int yMax = 0, double slopeInv = 0) : x(x), yMax(yMax), slopeInv(slopeInv) {}
};

const int N = 1600;

typedef vector<EdgeRec> EdgeTable[N];

void EdgeToTable(Point p1, Point p2, EdgeTable table) {
    if (p1.y == p2.y)
        return;
    if (p1.y > p2.y)
        swap(p1, p2);
    double slopeInv = (p2.x - p1.x) / (double) (p2.y - p1.y);
    table[(int) p1.y].emplace_back(EdgeRec(p1.x, p2.y, slopeInv));
}

void PolygonToTable(vector<Point> points, EdgeTable table) {
    int n = points.size();
    Point p1 = points[n - 1];
    for (int i = 0; i < n; i++) {
        Point p2 = points[i];
        EdgeToTable(p1, p2, table);
        p1 = p2;
    }
}

void TableToScreenForNonConvex(EdgeTable table, GLfloat *drawingColor) {
    int y = 0;
    while (table[y].empty())
        y++;
    auto active = table[y];
    while (!active.empty()) {
        sort(active.begin(), active.end(), [](EdgeRec a, EdgeRec b) {
            return a.x < b.x;
        });

        for (int i = 0; i < active.size(); i += 2) {
            auto a = active[i], b = active[i + 1];
            drawLine(a.x, y, b.x, y, drawingColor);
        }

        y++;

        for (int i = 0; i < active.size(); i++)
            if (active[i].yMax == y) {
                active.erase(active.begin() + i);
                i--;
            }

        for (auto &node: active)
            node.x += node.slopeInv;

        for (auto &node: table[y])
            active.emplace_back(node);
    }
}

void FillPolygon(vector<Point> &points, GLfloat *drawingColor) {
    EdgeTable table;
    PolygonToTable(points, table);
    TableToScreenForNonConvex(table, drawingColor);
}

/// convex

typedef struct {
    int xLeft, xRight;
} EdgeTable_convex[N];

void InitEdgeTable_convex(EdgeTable_convex table) {
    for (int i = 0; i < N; i++) {
        table[i].xLeft = INT_MAX;
        table[i].xRight = INT_MIN;
    }
}

void EdgeToTable_convex(Point p1, Point p2, EdgeTable_convex table) {
    if (p1.y == p2.y)
        return;
    if (p1.y > p2.y)
        swap(p1, p2);
    double slopeInv = (p2.x - p1.x) / (double) (p2.y - p1.y);
    double x = p1.x;
    for (int y = p1.y; y < p2.y; y++, x += slopeInv) {
        table[y].xLeft = min(table[y].xLeft, (int) ceil(x));
        table[y].xRight = max(table[y].xRight, (int) floor(x));
    }
}

void PolygonToTable_convex(vector<Point> points, EdgeTable_convex table) {
    int n = points.size();
    Point p1 = points[n - 1];
    for (int i = 0; i < n; i++) {
        Point p2 = points[i];
        EdgeToTable_convex(p1, p2, table);
        p1 = p2;
    }
}

void TableToScreen_convex(EdgeTable_convex table, GLfloat *drawingColor) {
    for (int y = 0; y < N; y++)
        if (table[y].xLeft < table[y].xRight)
            drawLine(table[y].xLeft, y, table[y].xRight, y, drawingColor);
}

void FillPolygon_convex(vector<Point> points, GLfloat *drawingColor) {
    EdgeTable_convex table;
    InitEdgeTable_convex(table);
    PolygonToTable_convex(points, table);
    TableToScreen_convex(table, drawingColor);
}

