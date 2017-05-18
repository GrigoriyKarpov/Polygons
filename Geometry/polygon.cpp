#include "polygon.h"

Polygon::Polygon() {

}

Polygon::Polygon(QVector<Point> points) {
    this->points = points;
}

Point &Polygon::operator[](int n) {
    return points[n];
}

int Polygon::count() {
    return points.count();
}

void Polygon::addPoint(Point p) {
    points.push_back(p);
}

QString Polygon::toString() {
    QString result;

    for (int i = 0; i < points.count(); i++) {
        result += QString().sprintf("(%.1f; %.1f) ", points[i].getX(), points[i].getY());
    }

    return result;
}

Point Polygon::center() {
    Point result;

    int count = points.count();

    for (int i = 0; i < count; i++) {
        result += points[i];
    }

    result /= (double) count;

    return result;
}
