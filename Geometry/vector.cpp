#include "vector.h"
#include "point.h"
#include "qmath.h"

Vector::Vector(): x(0.0), y(0.0) {
}

Vector::Vector(double x, double y) {
    this->x = x;
    this->y = y;
}

Vector::Vector(Point p1, Point p2) {
    this->x = p2.getX() - p1.getX();
    this->y = p2.getY() - p1.getY();
}

double Vector::getX() {
    return this->x;
}

double Vector::getY() {
    return this->y;
}

void Vector::setX(double x) {
    this->x = x;
}

void Vector::setY(double y) {
    this->y = y;
}

QString Vector::toString() {
    QString result;
    result.sprintf("(%.1f; %.1f)", this->x, this->y);
    return result;
}

Vector Vector::normal(Point p1, Point p2) {
    return Vector(p1.getY() - p2.getY(), p2.getX() - p1.getX());
}

double Vector::cos(Vector v1, Vector v2) {

    double a = v1.getX() * v2.getX() + v1.getY() * v2.getY();

    double b = qSqrt(qPow(v1.getX(), 2) + qPow(v1.getY(), 2)) *
                qSqrt(qPow(v2.getX(), 2) + qPow(v2.getY(), 2));

    return a / b;
}

double Vector::cos(Vector v) {

    double a = x * v.getX() + y * v.getY();

    double b = qSqrt(qPow(x, 2) + qPow(y, 2)) *
                qSqrt(qPow(v.getX(), 2) + qPow(v.getY(), 2));

    return a / b;
}

Point Vector::translate(Point p) {
    return Point(p.getX() + x, p.getY() + y);
}
