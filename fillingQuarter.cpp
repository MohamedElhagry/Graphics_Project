//#include "Functions.h"
//void draw2Lines(int xc, int yc, int x, int y, GLfloat * drawingColor, int quarter)
//{
//    switch (quarter)
//    {
//        case 1:
//            drawLine( xc, yc, xc + x, yc + y, drawingColor);
//            drawLine( xc, yc, xc + y, yc + x, drawingColor);
//            break;
//        case 2:
//            drawLine( xc, yc, xc + x, yc - y, drawingColor);
//            drawLine( xc, yc, xc + y, yc - x, drawingColor);
//            break;
//        case 3:
//            drawLine( xc, yc, xc - y, yc - x, drawingColor);
//            drawLine( xc, yc, xc - x, yc - y, drawingColor);
//            break;
//        case 4:
//            drawLine( xc, yc, xc - y, yc + x, drawingColor);
//            drawLine( xc, yc, xc - x, yc + y, drawingColor);
//            break;
//    }
//}
//
//void FillQuarter(int xc, int yc, int R, GLfloat *drawingColor, int quarter)
//{
//    int x = 0, y = R;
//    int d = 1 - R, d1 = 3, d2 = 5 - 2 * R;
//    do {
//        draw2Lines(xc, yc, x, y, drawingColor, quarter);
//        if (d < 0)
//        {
//            d += d1;
//            d2 += 2;
//        }
//        else
//        {
//            d += d2;
//            d2 += 4;
//            y--;
//        }
//        d1 += 2;
//        x++;
//
//    } while (x < y);
//}
