//
// Created by Mohamed on 5/31/2022.
//

#ifndef GRAPHICS_PROJECT_CLIPPING_ALGORITHMS_H
#define GRAPHICS_PROJECT_CLIPPING_ALGORITHMS_H

#include "iostream"
#include "DrawSimpleShapes.h"
#include <math.h>
#include <vector>

using namespace std;

//Rectangular clipping window
//utilities
union OutCode {
    unsigned All: 4;
    struct {
        unsigned left: 1, top: 1, right: 1, bot: 1;
    };
};

inline OutCode getOutCode(double x, double y, int left, int top, int right, int bot) {
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

inline Point VIntersect(double xs, double ys, double xe, double ye, int x) {
    Point p;
    p.x = x;
    p.y = ys + (ye - ys) * (x - xs) / (xe - xs);
    return p;
}

inline Point HIntersect(double xs, double ys, double xe, double ye, int y) {
    Point p;
    p.y = y;
    p.x = xs + (xe - xs) * (y - ys) / (ye - ys);
    return p;
}

inline void clipPoint(int x, int y, int left, int top, int right, int bot, GLfloat *drawingColor) {
    glactivation();
    glColor3f(drawingColor[0], drawingColor[1], drawingColor[2]);
    if (x >= left && x <= right && y >= top && y <= bot) {
        glVertex2d(x, y);
    }
    gldeactivation();
    GLfloat borderColor[] = {0.0f, 1.0f, 1.0f};
    drawRectangle(left, top, right, bot, borderColor);
}

//Cohen Sutherland
inline void clipLine(int xs, int ys, int xe, int ye, int left, int top, int right, int bot, GLfloat *drawingColor) {
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
        drawLine(x1, y1, x2, y2, drawingColor);
    }

    GLfloat borderColor[] = {0.0f, 1.0f, 1.0f};
    drawRectangle(left, top, right, bot, borderColor);

}


struct Vertex {
    double x, y;

    Vertex(int x1 = 0, int y1 = 0) {
        x = x1;
        y = y1;
    }
};

typedef vector<Vertex> VertexList;

typedef bool (*IsInFunc)(Vertex &p, int edge);

typedef Vertex (*IntersectFunc)(Vertex &p1, Vertex &p2, int edge);

bool inLeft(Vertex &p, int edge) {
    return p.x >= edge;
}

bool inRight(Vertex &p, int edge) {
    return p.x <= edge;
}

bool inTop(Vertex &p, int edge) {
    return p.y >= edge;
}

bool inBottom(Vertex &p, int edge) {
    return p.y <= edge;
}

Vertex VIntersect(Vertex &v1, Vertex &v2, int xedge) {
    Vertex res;
    res.x = xedge;
    res.y = v1.y + (xedge - v1.x) * (v2.y - v1.y) / (v2.x - v1.x);
    return res;
}


Vertex HIntersect(Vertex &v1, Vertex &v2, int yedge) {
    Vertex res;
    res.y = yedge;
    res.x = v1.x + (yedge - v1.y) * (v2.x - v1.x) / (v2.y - v1.y);
    return res;
}

inline void drawPolygon(Vertex *p, int n, GLfloat *drawingColor) {
    VertexList vList;
    for (int i = 0; i < n; i++) vList.push_back(Vertex(p[i].x, p[i].y));
    Vertex v1 = vList[vList.size() - 1];
    for (int i = 0; i < (int) vList.size(); i++) {
        Vertex v2 = vList[i];
        drawLine(Round(v1.x), Round(v1.y), Round(v2.x), Round(v2.y), drawingColor);
        v1 = v2;
    }
}

inline void drawPolygon(Point *p, int n, GLfloat *drawingColor) {
    VertexList vList;
    for (int i = 0; i < n; i++) vList.push_back(Vertex(p[i].x, p[i].y));
    Vertex v1 = vList[vList.size() - 1];
    for (int i = 0; i < (int) vList.size(); i++) {
        Vertex v2 = vList[i];
        drawLine(Round(v1.x), Round(v1.y), Round(v2.x), Round(v2.y), drawingColor);
        v1 = v2;
    }
}


inline VertexList clipWithEdge(VertexList p, int edge, IsInFunc In, IntersectFunc Intersect) {
    VertexList outList;
    Vertex v1 = p[p.size() - 1];
    bool v1_in = In(v1, edge);
    for (int i = 0; i < (int) p.size(); i++) {
        Vertex v2 = p[i];
        bool v2_in = In(v2, edge);
        if (!v1_in && v2_in) {
            outList.push_back(Intersect(v1, v2, edge));
            outList.push_back(v2);
        } else if (v1_in && v2_in) outList.push_back(v2);
        else if (v1_in) outList.push_back(Intersect(v1, v2, edge));
        v1 = v2;
        v1_in = v2_in;
    }

    return outList;
}


inline void clipPolygon(Vertex *p, int n, int left, int top, int right, int bottom, GLfloat *drawingColor) {
    VertexList vList;
    for (int i = 0; i < n; i++) vList.push_back(Vertex(p[i].x, p[i].y));
    vList = clipWithEdge(vList, left, inLeft, VIntersect);
    vList = clipWithEdge(vList, top, inTop, HIntersect);
    vList = clipWithEdge(vList, right, inRight, VIntersect);
    vList = clipWithEdge(vList, bottom, inBottom, HIntersect);
    Vertex v1 = vList[vList.size() - 1];
    for (int i = 0; i < (int) vList.size(); i++) {
        Vertex v2 = vList[i];
        drawLine(Round(v1.x), Round(v1.y), Round(v2.x), Round(v2.y), drawingColor);
        v1 = v2;
    }
    //drawPolygon(p, n, drawingColor);
    GLfloat borderColor[] = {0.0f, 1.0f, 1.0f};
    drawRectangle(left, top, right, bottom, borderColor);
}


inline void clipPolygon(Point *p, int n, int left, int top, int right, int bottom, GLfloat *drawingColor) {
    Vertex verticies[n];
    for (int i = 0; i < n; i++)
        verticies[i] = Vertex(p[i].x, p[i].y);
    clipPolygon(verticies, n, left, top, right, bottom, drawingColor);
}



//Circular clipping window

//utilities
inline bool inside(int x, int y, int xc, int yc, double R) {
    return (x - xc) * (x - xc) + (y - yc) * (y - yc) <= R * R;
}

inline bool pointInsideLine(int x, int y, int xs, int ys, int xe, int ye) {
    if (xs > xe)
        swap(xs, xe);
    if (ys > ye)
        swap(ys, ye);

    return x <= xe && x >= xs && y >= ys && y <= ye;
}

//still under development
inline bool
CIntersect(double xs, double ys, double xe, double ye, double xc, double yc, double R, Point &p1, Point &p2) {
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

inline void clipPointFromCircle(int x, int y, int xc, int yc, int R, GLfloat *c) {
    if (inside(x, y, xc, yc, R))
        drawPoint(x, y, c);
    GLfloat borderColor[] = {0.0f, 1.0f, 1.0f};
    drawCircle(xc, yc, R, borderColor);
}

inline void clipLineFromCircle(int xs, int ys, int xe, int ye, int xc, int yc, int R, GLfloat *c) {
    Point p1, p2;
    bool intersects = CIntersect(xs, ys, xe, ye, xc, yc, R, p1, p2);
    if (!intersects)
        return;

    GLfloat borderColor[] = {0.0f, 1.0f, 1.0f};
    drawCircle(xc, yc, R, borderColor);
    drawLine(p1.x, p1.y, p2.x, p2.y, c);
}

#endif //GRAPHICS_PROJECT_CLIPPING_ALGORITHMS_H
