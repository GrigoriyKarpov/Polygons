#ifndef POINT_H
#define POINT_H

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
    void shift(Point s);
    void shift(double x, double y);

    const Point operator+  (const Point & p);
    Point &operator+= (Point & p);
    Point &operator/= (double d);

    QString toString();
};

#endif // POINT_H
