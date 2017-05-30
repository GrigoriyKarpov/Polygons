#ifndef TOOLS_H
#define TOOLS_H

#include "point.h"
#include "polygon.h"
#include "vector.h"

class Tools {

public:
    static QVector<Point> slipInAngle(Point a0, Point b0, Point c0, double h, double angle, double d1, double d2);
    static double cos(Point a, Point b, Point c);
    static Point distOnSegment(Point a, Point b, Point c, double h);
    static Point transformCS(Point p, Point o, double a, double x, double y);
    static int sign(double number);
    static int outPoint(Point a, Point b, Point c);
    static double slope(Point a, Point b);
    static bool lineIntersect(Point a, Point b, Point c, Point d, Point *p);
    static bool segmentIntersect(Point a, Point b, Point c, Point d, Point *p);
    static int angleDirection(Point a, Point b, Point c);
    static bool geometrySearch(Polygon pol, Point p);
    static Polygon outCharArea(Polygon p1, Polygon p2, int o);
    static Polygon inCharArea(Polygon p1, Polygon p2, int o);
    static Point center(Point a, Point b);
};

#endif // TOOLS_H
