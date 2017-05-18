#ifndef POLYGON_H
#define POLYGON_H

#include "point.h"
#include <QVector>

class Polygon {
private:
    QVector<Point> points;

public:
    Polygon();
    Polygon(QVector<Point> points);

    Point& operator[] (int n);

    int count();

    void addPoint(Point p);
    Point center();
    QString toString();
};

#endif // POLYGON_H
