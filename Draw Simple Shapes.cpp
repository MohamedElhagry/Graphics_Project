//
// Created by Mohamed on 5/27/2022.
//


void swap(int &x1, int &y1, int &x2, int &y2) {
    int tmp = x1;
    x1 = x2;
    x2 = tmp;
    tmp = y1;
    y1 = y2;
    y2 = tmp;
}

void DrawPoint(int x, int y, GLfloat *drawingColor) {
    glBegin(GL_POINTS);
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);
    glVertex2d(x, y);

    glEnd();
    glFlush();
}

void DrawLine(int x1, int y1, int x2, int y2, GLfloat *drawingColor) {
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

int Round(double x) {
    return (int) (x + 0.5);
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


void DrawCircle(int xc, int yc, int R, GLfloat *drawingColor) {

    glBegin(GL_POINTS);
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);
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

