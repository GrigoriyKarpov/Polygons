#include "point.h"

Point::Point() : x(0.0), y(0.0) {
}

Point::Point(double x, double y) {
    this->x = x;
    this->y = y;
}

double Point::getX() {
    return this->x;
}

double Point::getY() {
    return this->y;
}

void Point::setX(double x) {
    this->x = x;
}

void Point::setY(double y) {
    this->y = y;
}

QString Point::toString() {
    QString result;
    result.sprintf("(%.1f; %.1f)", this->x, this->y);
    return result;
}

//Здвиг точки
void Point::shift(Point s) {
    this->x += s.getX();
    this->y += s.getY();
}

void Point::shift(double x, double y) {
    this->x += x;
    this->y += y;
}

//Point &Point::operator+=(Point &point) {
//    return Point(this->getX() + point.getX(), this->getY() + point.getY());
//}

const Point Point::operator+(const Point &p) {
    return Point (x + p.x, y + p.y);
}

Point &Point::operator+=(Point &p)
{
    x += p.x;
    y += p.y;
}

Point &Point::operator/= (double d) {
    x /= d;
    y /= d;
}
