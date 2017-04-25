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
        rK = distOnSegment(b, a, h);
        rG = distOnSegment(b, c, h);

        rL = b;
        rF = b;
    } else {            //Угол < 90

    }

    //Возвращаем массив из 4-х точек
    QVector<Point> result;
    result.push_back(rK);
    result.push_back(rL);
    result.push_back(rF);
    result.push_back(rG);

    return result;
}

//Функция откладывает расстояние h на отрезке [a;b] от точки а
Point Tools::distOnSegment(Point a, Point b, double h) {
    Point Q;

    double tmp = qPow(b.getY() - a.getY(), 2) / qPow(b.getX() - a.getX(), 2);

    //Находим Qx
    double Qx1 = a.getX() - qPow(h, 2) / qSqrt(tmp + 1);
    double Qx2 = a.getX() + qPow(h, 2) / qSqrt(tmp + 1);

    //Выбирам координату, которая лежит между координатами точек отрезка
    if(((a.getX() <= b.getX()) && (Qx1 <= b.getX()) && (Qx1 >= a.getX())) ||
       ((a.getX() >= b.getX()) && (Qx1 >= b.getX()) && (Qx1 <= a.getX()))) {
        Q.setX(Qx1);
    } else {
        Q.setX(Qx2);
    }

    //Находим Qy
    double Qy1 = a.getY() - qSqrt(qPow(h, 2) - qPow(a.getX() - Q.getX(), 2));
    double Qy2 = a.getY() + qSqrt(qPow(h, 2) - qPow(a.getX() - Q.getX(), 2));

    //Выбирам координату, которая лежит между координатами точек отрезка
    if(((a.getY() <= b.getY()) && (Qy1 <= b.getY()) && (Qy1 >= a.getY())) ||
            ((a.getY() >= b.getY()) && (Qy1 >= b.getY()) && (Qy1 <= a.getY()))) {
        Q.setY(Qy1);
    } else {
        Q.setY(Qy2);
    }

    return Q;
}

//Косинус угла abc
double Tools::cos(Point a, Point b, Point c) {
    Point v1 = Point(a.getX() - b.getX(), a.getY() - b.getY());
    Point v2 = Point(c.getX() - b.getX(), c.getY() - b.getY());

    double result = (v1.getX() * v2.getX() + v1.getX() * v2.getX()) /
            (qSqrt(qPow(v1.getX(), 2) + qPow(v1.getY(), 2)) *
             qSqrt(qPow(v2.getX(), 2) + qPow(v2.getY(), 2)));

    return result;
}

double Tools::dist(Point a, Point b) {
    return qSqrt(qPow((a.getX() - b.getX()), 2) +
                 qPow((a.getY() - b.getY()),2));
}
