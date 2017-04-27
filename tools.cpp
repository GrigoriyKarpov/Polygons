#include "tools.h"
#include <qmath.h>
#include <QDebug>

//Point
Point::Point():x(0),y(0){}

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
    QString result = "Point( " +
            QString::number(this->x) + " ; " +
            QString::number(this->y) + " )";

    return result;
}

//Line
Line::Line(Point a, Point b) {
    this->a = a;
    this->b = b;
}

Point Line::getA() {
    return this->a;
}

Point Line::getB() {
    return this->b;
}

void Line::setA(Point a) {
    this->a = a;
}

void Line::setB(Point b) {
    this->b = b;
}

QString Line::toString() {
    QString result = "Line( " +
            QString::number(this->a.getX()) + " ; " +
            QString::number(this->a.getY()) + " ; " +
            QString::number(this->b.getX()) + " ; " +
            QString::number(this->b.getY()) + " )";

    return result;
}

//Tools
QVector<Point> Tools::lineSlipInCorner(Point a, Point b, Point c, double h) {
    Point rK, rL, rF, rG;

    //Косинус заданного угла
    double cos = Tools::cos(a, b, c);

    if (cos <= 0) {     //Угол >= 90
        //Откладываем точки на отрезках угла согласно расстоянию h
        rK = distOnSegment(b, a, b, h);
        rG = distOnSegment(b, c, b, h);

        rL = b;
        rF = b;
    } else {            //Угол < 90
        double cor = qAcos(cos);

        rL = distOnSegment(b, a, b, h / qTan(cor));
        rG = distOnSegment(b, c, b, h / qSin(cor));
        rF = distOnSegment(b, c, b, h / qTan(cor));
        rK = distOnSegment(b, a, b, h / qSin(cor));
    }

    //Возвращаем массив из 4-х точек
    QVector<Point> result;
    result.push_back(rK);
    result.push_back(rL);
    result.push_back(rF);
    result.push_back(rG);

    return result;
}

//Функция вычисляет пересечение отрезка [a;b] и окружности с центром в точке c и радиусом h
Point Tools::distOnSegment(Point a, Point b, Point c, double h) {
    Point Q;

    double t = (b.getY() - a.getY()) / (b.getX() - c.getX());
    double k = a.getY() - c.getY();
    double m = a.getX() - c.getX();

    double tmp = qSqrt(2 * t * k * m - qPow(t * m, 2) + qPow(h, 2) - qPow(k, 2) + qPow(h * t, 2));

    //Находим Qx
    double Qx1 = (a.getX() * qPow(t, 2) - k * t + c.getX() - tmp) / (qPow(t, 2) + 1);
    double Qx2 = (a.getX() * qPow(t, 2) - k * t  + c.getX() + tmp) / (qPow(t, 2) + 1);

    //Выбирам координату, которая лежит между координатами точек отрезка
    if(((a.getX() <= b.getX()) && (Qx1 <= b.getX()) && (Qx1 >= a.getX())) ||
       ((a.getX() >= b.getX()) && (Qx1 >= b.getX()) && (Qx1 <= a.getX()))) {
        Q.setX(Qx1);
    } else {
        Q.setX(Qx2);
    }

    //Находим Qy
    double Qy = a.getY() + (((Q.getX() - a.getX()) * (b.getY() - a.getY())) / (b.getX() - a.getX()));
    Q.setY(Qy);

    return Q;
}

//Косинус угла abc
double Tools::cos(Point a, Point b, Point c) {
    Point v1 = Point(a.getX() - b.getX(), a.getY() - b.getY());
    Point v2 = Point(c.getX() - b.getX(), c.getY() - b.getY());

    double result = (v1.getX() * v2.getX() + v1.getY() * v2.getY()) /
            (qSqrt(qPow(v1.getX(), 2) + qPow(v1.getY(), 2)) *
             qSqrt(qPow(v2.getX(), 2) + qPow(v2.getY(), 2)));

    return result;
}

//расстояние между двумя точками
double Tools::dist(Point a, Point b) {
    return qSqrt(qPow((a.getX() - b.getX()), 2) +
                 qPow((a.getY() - b.getY()),2));
}
