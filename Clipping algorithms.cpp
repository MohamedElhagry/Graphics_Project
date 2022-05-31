//
// Created by Mohamed on 5/27/2022.
//
#include "iostream"
#include "Draw Simple Shapes.cpp"
using namespace std;

//Rectangular clipping window
//utilities
union OutCode{
    unsigned All:4;
    struct{unsigned left:1, top:1, right:1, bot:1;};
};

struct Point{
    double x, y;
};

OutCode getOutCode(double x, double y, int left, int top, int right, int bot)
{
    OutCode out;
    out.All = 0;
    if(x < left)
        out.left = 1;
    else if(x > right)
        out.right = 1;
    if(y < top)
        out.top = 1;
    else if(y > bot)
        out.bot = 1;

    return out;
}

Point VIntersect(double xs, double ys, double xe, double ye, int x){
    Point p;
    p.x = x;
    p.y = ys + (ye-ys)*(x-xs)/(xe-xs);
    return p;
}

Point HIntersect(double xs, double ys, double xe, double ye, int y){
    Point p;
    p.y = y;
    p.x = xs + (xe-xs)*(y-ys)/(ye-ys);
    return p;
}

void clipPoint(int x, int y, int left, int top, int right, int bot, GLfloat c){
    glBegin(GL_POINTS);
    glColor3f(1, 0, 0);
    if(x >= left && x <= right && y >= top && y <= bot){
        glVertex2d(x,y);
    }
    glEnd();
    glFlush();
}

//Cohen Sutherland
void clipLine(int xs, int ys, int xe, int ye, int left, int top, int right, int bot, GLfloat* c)
{
    double x1 = xs, y1 = ys, x2 = xe, y2 = ye;
    OutCode oc1 = getOutCode(x1,y1,left,top,right,bot);
    OutCode oc2 = getOutCode(x2,y2,left,top,right,bot);

    while((oc1.All || oc2.All) && !(oc1.All & oc2.All)){
        Point p;
        if(oc1.All){
            if(oc1.left)
                p = VIntersect(x1,y1,x2,y2,left);
            else if(oc1.right)
                p = VIntersect(x1,y1,x2,y2,right);
            else if(oc1.bot)
                p = HIntersect(x1,y1,x2,y2,bot);
            else p = HIntersect(x1,y1,x2,y2,top);

            x1 = p.x; y1 = p.y;
            oc1 = getOutCode(x1,y1,left,top,right,bot);
        }
        else
        {   if(oc2.left)
                p = VIntersect(x1,y1,x2,y2,left);
            else if(oc2.right)
                p = VIntersect(x1,y1,x2,y2,right);
            else if(oc2.bot)
                p = HIntersect(x1,y1,x2,y2,bot);
            else p = HIntersect(x1,y1,x2,y2,top);

            x2 = p.x; y2 = p.y;
            oc2 = getOutCode(x2,y2,left,top,right,bot);
        }
    }

    if( !(oc1.All | oc2.All)){
        drawLine(x1,y1,x2,y2, c );
    }

}

void clipPolygon()
{

}


//Circular clipping window

//utilities
bool inside(int x, int y, int xc, int yc, double R){
    return (x - xc) * (x - xc) + (y - yc) * (y - yc) <= R * R;
}

bool pointInsideLine(int x, int y, int xs, int ys, int xe, int ye){
    if(xs > xe)
        swap(xs, xe);
    if(ys > ye)
        swap(ys, ye);

    return x <= xe && x >= xs && y >= ys && y <= ye;
}

//still under development
bool CIntersect(double xs, double ys, double xe, double ye, double xc, double yc ,double R, Point &p1, Point &p2){
    double dy = ye - ys;
    double dx = xe - xs;
    double D = (ys - yc - dy/dx * xs);

    double a = 1 + (dy*dy) / (dx*dx);
    double b = 2 * D * dy/dx - 2 * xc;
    double c = xc*xc + D*D - R*R;

    if(b*b < 4*a*c)
        return false;


    // greater x value
    double x1 = (-b - sqrt(b*b - 4*a*c)) / (2*a);
    double y1 = ys + (x1 - xs) / dx * dy;

    //smaller x value
    double x2 = (-b + sqrt(b*b - 4*a*c)) / (2*a);
    double y2 = ys + (x2 - xs) / dx * dy;


    if(xs > xe)
    {
        swap(xs, xe);
        swap(ys,ye);
    }


    if(pointInsideLine(x1,y1, xs,ys,xe,ye)){
        p1.x = x1; p1.y = y1;
    }
    else{
        p1.x = xs; p1.y = ys;
    }

    if(pointInsideLine(x2,y2, xs,ys,xe,ye)){
        p2.x = x2; p2.y = y2;
    }
    else{
        p2.x = xe; p2.y = ye;
    }

    return true;
}

void clipPointFromCircle(int x, int y, int xc, int yc, int R, GLfloat *c){
    if(inside(x,y,xc,yc,R))
        drawPoint(x, y, c);
}

void clipLineFromCircle(int xs, int ys, int xe, int ye, int xc, int yc, int R, GLfloat *c){
    Point p1, p2;
    bool intersects = CIntersect(xs ,ys, xe, ye, xc, yc, R, p1, p2);
    if(!intersects)
        return;

    drawCircle(xc,yc,R, c);
    drawLine(p1.x,p1.y,p2.x,p2.y,c);
}
