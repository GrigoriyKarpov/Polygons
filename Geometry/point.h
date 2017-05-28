#ifndef POINT_H
#define POINT_H

#include <QString>
#include <QPointF>

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
    double &rx();
    double &ry();

    double dist(Point p);

    Point operator+  (Point p);
    Point operator+  (double d);
    Point operator-  (Point p);
    Point operator-  (double d);
    Point operator*  (double d);
    Point operator/  (double d);

    Point &operator+= (Point p);
    Point &operator+= (double d);
    Point &operator-= (Point p);
    Point &operator-= (double d);
    Point &operator*= (double d);
    Point &operator/= (double d);

    Point &operator= (const QPointF & p);

    bool operator== (Point p);

    static Point toPoint(const QPointF & p);

    QString toString();
};

#endif // POINT_H
