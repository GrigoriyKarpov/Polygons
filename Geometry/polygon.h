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

    void addPoint(Point p);
    int count();
    void clear();
    void removeLast();
    void remove(int i);
    Point center();
    QString toString();
    Point pointLabel(int i, double dist);

    bool isConvex();
    bool isPolygon();

    Point& operator[] (int n);
};

#endif // POLYGON_H
