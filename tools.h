#ifndef TOOLS_H
#define TOOLS_H

#include <QString>

//Point
class Point {
private:
    double x;
    double y;

public:
    Point();
    Point(double x, double y);

    double getX();
    double getY();
    void setX(double x);
    void setY(double y);

    QString toString();
};

//Line
class Line {
private:
    Point a;
    Point b;

public:
    Line(Point a, Point b);

    Point getA();
    Point getB();
    void setA(Point a);
    void setB(Point b);

    QString toString();
};

class Tools {
public:

public:
    static QVector<Point> slipInAngle(Point a0, Point b0, Point c0, double h, double angle, double d1, double d2);
    static double dist(Point a, Point b);
    static double cos(Point a, Point b, Point c);
    static Point distOnSegment(Point a, Point b, Point c, double h);
    static bool intersection(Point a, Point b, Point c, Point d, Point *intersection);
    static Point transformCS(Point p, Point o, double a, double x, double y);
    static int sign(double number);
    static int outPoint(Point a, Point b, Point c);
};

#endif // TOOLS_H
