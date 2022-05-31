//
// Created by El belkasy on 5/31/2022.
//
#ifndef GRAPHICS_PROJECT_DRAW_SIMPLE_SHAPES_H
#define GRAPHICS_PROJECT_DRAW_SIMPLE_SHAPES_H

#include <gl\GLu.h>
#include <math.h>
#include <bits/stdc++.h>
#include "Draw Simple Shapes.cpp"
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

struct Point;

void swap(int &x1, int &y1, int &x2, int &y2);

void swap(double &x1, double &x2);

double max(double a, double b);

int Round(double x);

void drawPoint(int x, int y, GLfloat *drawingColor);

void drawLineMidPoint(int x1, int y1, int x2, int y2, GLfloat *drawingColor);

void drawLine(int x1, int y1, int x2, int y2, GLfloat *drawingColor);

void drawLineDDA(int x1, int y1, int x2, int y2, GLfloat *drawingColor);

void drawEllipseMidPoint(int xc, int yc, int A, int B, GLfloat *drawingColor);

void drawRectangle(int x1, int y1, int x3, int y3, GLfloat *c);

void drawLineMidPoint(int x1, int y1, int x2, int y2, GLfloat *drawingColor);

void drawLineParametric(int x1, int y1, int x2, int y2, GLfloat *drawingColor);

void draw8Points(int xc, int yc, int x, int y);

void drawEllipsePolar(int xc, int yc, int A, int B, GLfloat *drawingColor);

void drawCircle(int xc, int yc, int R, GLfloat *drawingColor);

void draw4Points(int xc, int yc, int x, int y);

void drawCirclePolar(int xc, int yc, int R, GLfloat *drawingColor);

void drawCircleMidPointModified(int xc, int yc, int R, GLfloat *drawingColor);

void drawCirclePolarIterative(int xc, int yc, int R, GLfloat *drawingColor);

void drawCircleMidPoint(int xc, int yc, int R, GLfloat *drawingColor);

union OutCode;

OutCode getOutCode(double x, double y, int left, int top, int right, int bot);

Point VIntersect(double xs, double ys, double xe, double ye, int x);

Point HIntersect(double xs, double ys, double xe, double ye, int y);

void clipPoint(int x, int y, int left, int top, int right, int bot, GLfloat *drawingColor);

void clipLine(int xs, int ys, int xe, int ye, int left, int top, int right, int bot, GLfloat *c);

typedef std::vector<Point> PointList;

typedef bool (*IsInFunc)(Point &v, int edge);

typedef Point (*IntersectFunc)(Point &p1, Point &p2, int edge);

bool inLeft(Point &p, int edge);

bool inRight(Point &p, int edge);

bool inTop(Point &p, int edge);

bool inBottom(Point &p, int edge);

void clipPolygon(Point *points, int n, int left, int top, int right, int bottom);

bool inside(int x, int y, int xc, int yc, double R);

bool pointInsideLine(int x, int y, int xs, int ys, int xe, int ye);

bool CIntersect(double xs, double ys, double xe, double ye, double xc, double yc, double R, Point &p1, Point &p2);

void clipPointFromCircle(int x, int y, int xc, int yc, int R, GLfloat *c);

void clipLineFromCircle(int xs, int ys, int xe, int ye, int xc, int yc, int R, GLfloat *c);

void draw2Lines(int xc, int yc, int x, int y, GLfloat *drawingColor, int quarter);

void FillQuarter(int xc, int yc, int R, GLfloat *drawingColor, int quarter);

void DrawHermiteCurve(Point p1, Point T1, Point p2, Point T2, GLfloat *drawingColor);

void FillRectangleWithHermite(int x1, int y1, int x3, int y3, GLfloat *drawingColor);

void DrawBezierCurve(Point p1, Point p2, Point p3, Point p4, GLfloat *drawingColor);

void FillRectangleWithBezier(int x1, int y1, int x3, int y3, GLfloat *drawingColor);

struct EdgeRec;

typedef std::vector<EdgeRec> EdgeTable[N];

void EdgeToTable(Point p1, Point p2, EdgeTable table);

void PolygonToTable(std::vector<Point> points, EdgeTable table);

void TableToScreenForNonConvex(EdgeTable table, GLfloat *drawingColor);

void FillPolygon(std::vector<Point> &points, GLfloat *drawingColor);

void EdgeToTable_convex(Point p1, Point p2, EdgeTable_convex table);

void InitEdgeTable_convex(EdgeTable_convex table);

void EdgeToTable_convex(Point p1, Point p2, EdgeTable_convex table);

void PolygonToTable_convex(std::vector<Point> points, EdgeTable_convex table);

void TableToScreen_convex(EdgeTable_convex table, GLfloat *drawingColor);

void FillPolygon_convex(std::vector<Point> points, GLfloat *drawingColor);

#endif //GRAPHICS_PROJECT_DRAW_SIMPLE_SHAPES_H
