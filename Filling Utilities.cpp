////
//// Created by El belkasy on 5/31/2022.
////
//
//#include <vector>
//
//struct EdgeRec {
//    double x, slopeInv;
//    int yMax;
//
//    EdgeRec(double x = 0, int yMax = 0, double slopeInv = 0) : x(x), yMax(yMax), slopeInv(slopeInv) {}
//};
//
//typedef vector<EdgeRec> EdgeTable[800];
//
//void EdgeToTable(Point p1, Point p2, EdgeTable table) {
//    if (p1.y == p2.y)
//        return;
//    if (p1.y > p2.y)
//        swap(p1, p2);
//    double slopeInv = (p2.x - p1.x) / (double) (p2.y - p1.y);
//    table[p1.y].emplace_back(EdgeRec(p1.x, p2.y, slopeInv));
//}
//
//void PolygonToTable(vector<Point> points, EdgeTable table) {
//    int n = points.size();
//    Point p1 = points[n - 1];
//    for (int i = 0; i < n; i++) {
//        Point p2 = points[i];
//        EdgeToTable(p1, p2, table);
//        p1 = p2;
//    }
//}