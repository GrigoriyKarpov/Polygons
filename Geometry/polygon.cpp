#include "polygon.h"
#include "vector.h"
#include "tools.h"

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

void Polygon::clear() {
    points.clear();
}

void Polygon::removeLast() {
    points.remove(points.count() - 1);
}

void Polygon::remove(int i) {
    points.remove(i);
}

QString Polygon::toString() {
    QString result;

    for (int i = 0; i < points.count(); i++) {
        result += QString().sprintf("(%.1f; %.1f) ", points[i].getX(), points[i].getY());
    }

    return result;
}

bool Polygon::isConvex() {
    bool result = true;
    bool tmp = true;

    if (isPolygon()) {
        for (int i = 0; i < points.count(); i++) {
            int j = i + 1;
            int k = i + 2;

            if (i == points.count() - 1) {
                j = 0;
                k = 1;
            } else if (i == points.count() - 2) {
                k = 0;
            }

            if (Tools::outPoint(points[i], points[j], points[k]) == 1) {
                result = false;
            }

            if (Tools::outPoint(points[i], points[j], points[k]) == -1) {
                tmp = false;
            }
        }
    } else {
        result = false;
        tmp = false;
    }

    if (result || tmp) {
        return true;
    } else {
        return false;
    }
}

bool Polygon::isPolygon() {
    if (points.count() < 3) {
        return false;
    }
    return true;
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

Point Polygon::pointLabel(int i, double dist) {
    Vector v = Vector(center(), points[i]);

    double x = points[i].getX() + dist * v.getX() / points[i].dist(center());
    double y = points[i].getY() + dist * v.getY() / points[i].dist(center());

    return Point(x, y);
}
