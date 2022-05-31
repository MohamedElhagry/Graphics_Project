//
// Created by Mohamed on 5/31/2022.
//

#ifndef GRAPHICS_PROJECT_DRAWSIMPLESHAPES_H
#define GRAPHICS_PROJECT_DRAWSIMPLESHAPES_H

//
// Created by Mohamed on 5/27/2022.
//


#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

#include <gl\GLu.h>

struct Point {
    int x, y;

    Point(int x = 0, int y = 0) {
        this->x = x;
        this->y = y;
    }
};

struct Vector {
    double v[2];

    Vector(double x = 0, double y = 0) {
        v[0] = x;
        v[1] = y;
    }

    double &operator[](int i) {
        return v[i];
    }

    Vector operator-(Vector v2) {
        return Vector(this->v[0] - v2[0], this->v[1] - v2[1]);
    }

    Point operator (double multiplicand) {
        return Vector(this->v[0] * multiplicand, this->v[1] * multiplicand);
    }

};

inline void swap(int &x1, int &y1, int &x2, int &y2) {
    int tmp = x1;
    x1 = x2;
    x2 = tmp;
    tmp = y1;
    y1 = y2;
    y2 = tmp;
}


inline double max(double a, double b) {
    if (a > b)
        return a;
    return b;
}

inline int Round(double x) {
    return (int) (x + 0.5);
}

inline void drawPoint(int x, int y, GLfloat *drawingColor) {
    glBegin(GL_POINTS);
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);
    glVertex2d(x, y);

    glEnd();
    glFlush();
}


//Line algorithms

inline void drawLineMidPoint(int x1, int y1, int x2, int y2, GLfloat *drawingColor);

inline void drawLine(int x1, int y1, int x2, int y2, GLfloat *drawingColor) {
    drawLineMidPoint(x1, y1, x2, y2, drawingColor);
}

inline void drawLineDDA(int x1, int y1, int x2, int y2, GLfloat *drawingColor) {
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

inline void drawLineMidPoint(int x1, int y1, int x2, int y2, GLfloat *drawingColor) {
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

inline void drawLineParametric(int x1, int y1, int x2, int y2, GLfloat *drawingColor) {
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
inline void draw8Points(int xc, int yc, int x, int y) {
    glVertex2d(xc + x, yc + y);
    glVertex2d(xc + x, yc - y);
    glVertex2d(xc - x, yc - y);
    glVertex2d(xc - x, yc + y);
    glVertex2d(xc + y, yc + x);
    glVertex2d(xc + y, yc - x);
    glVertex2d(xc - y, yc - x);
    glVertex2d(xc - y, yc + x);
}


inline void drawCircle(int xc, int yc, int R, GLfloat *drawingColor) {

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

inline void drawCirclePolar(int xc, int yc, int R, GLfloat *drawingColor) {

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

inline void drawCirclePolarIterative(int xc, int yc, int R, GLfloat *drawingColor) {

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

inline void drawCircleMidPoint(int xc, int yc, int R, GLfloat *drawingColor) {

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

inline void drawCircleMidPointModified(int xc, int yc, int R, GLfloat *drawingColor) {

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

inline void Draw4Points(int xc, int yc, int x, int y) {
    glVertex2d(xc + x, yc + y);
    glVertex2d(xc - x, yc + y);
    glVertex2d(xc + x, yc - y);
    glVertex2d(xc - x, yc - y);
}

inline void drawEllipsePolar(int xc, int yc, int A, int B, GLfloat *drawingColor) {
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

inline void drawEllipseMidPoint(int xc, int yc, int A, int B, GLfloat *drawingColor) {
    glBegin(GL_POINTS);
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);

    int Asq = A * A;
    int Bsq = B * B;
    //7th quad
    int x = -A;
    int y = 0;
    int d = -4 * Bsq * A + 4 * Asq + Bsq;

    Draw4Points(xc, yc, x, y);
    while (abs(x) * Bsq > abs(y) * Asq) {
        if (d < 0) {
            d += 8 * Asq * y + 12 * Asq;
            y++;
        } else {
            d += 8 * Asq * y + 12 * Asq + 8 * Bsq * x + 8 * Bsq;
            x++;
            y++;
        }
        Draw4Points(xc, yc, x, y);
    }

    glEnd();
    glFlush();
}

inline void drawRectangle(int x1, int y1, int x3, int y3, GLfloat *c) {
    int x2 = x1, y2 = y3;
    int x4 = x3, y4 = y1;
    drawLine(x1, y1, x2, y2, c);
    drawLine(x2, y2, x3, y3, c);
    drawLine(x3, y3, x4, y4, c);
    drawLine(x4, y4, x1, y1, c);
}

#endif //GRAPHICS_PROJECT_DRAWSIMPLESHAPES_H
