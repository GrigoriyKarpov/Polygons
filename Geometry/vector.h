#ifndef VECTOR_H
#define VECTOR_H

#include <QString>
//#include "point.h"
class Point;

class Vector {
private:
    double x;
    double y;

public:
    Vector();
    Vector(double x, double y);
    Vector(Point p1, Point p2);

    double getX();
    double getY();
    void setX(double x);
    void setY(double y);

    Point translate(Point p);

    QString toString();

    static Vector normal(Point p1, Point p2);
    static double cos(Vector v1, Vector v2);
    double cos(Vector v);
};

#endif // VECTOR_H
