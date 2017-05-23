#include "point.h"
#include <qmath.h>

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

double &Point::rx() {
    return x;
}

double &Point::ry() {
    return y;
}

double Point::dist(Point p) {
    return qSqrt(qPow((x - p.x), 2) + qPow((y - p.y), 2));
}

QString Point::toString() {
    QString result;
    result.sprintf("(%.1f; %.1f)", this->x, this->y);
    return result;
}

Point Point::operator+(Point p) {
    return Point (x + p.x, y + p.y);
}

Point Point::operator+(double d) {
    return Point (x * d, y * d);
}

Point Point::operator-(Point p) {
    return Point (x - p.x, y - p.y);
}

Point Point::operator-(double d) {
    return Point (x - d, y - d);
}

Point Point::operator*(double d) {
    return Point (x * d, y * d);
}

Point Point::operator/(double d) {
    return Point (x / d, y / d);
}

Point &Point::operator+=(Point p) {
    x += p.x;
    y += p.y;
    return *this;
}

Point &Point::operator+=(double d) {
    x += d;
    y += d;
    return *this;
}

Point &Point::operator-=(Point p) {
    x -= p.x;
    y -= p.y;
    return *this;
}

Point &Point::operator-=(double d) {
    x -= d;
    y -= d;
    return *this;
}

Point &Point::operator*=(double d) {
    x *= d;
    y *= d;
    return *this;
}

Point &Point::operator/= (double d) {
    x /= d;
    y /= d;
    return *this;
}

Point &Point::operator=(const QPointF &p) {
    x = p.x();
    y = p.y();
    return *this;
}

Point Point::toPoint(const QPointF &p) {
    return Point(p.x(), p.y());
}
