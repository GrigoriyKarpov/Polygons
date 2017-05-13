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
    QString result;
    result.sprintf("(%.1f; %.1f)", this->x, this->y);
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
QVector<Point> Tools::slipInAngle(Point a0, Point b0, Point c0, double h, double angle, double d1, double d2) {
    QVector<Point> result;
    Point r, q, m, t, c, o;

    //Преобразуем систему координат
    double angleCS = qAtan((a0.getY() - b0.getY()) / (a0.getX() - b0.getX()));
    if (a0.getX() < b0.getX()) {
        angleCS += M_PI;
    }

    c = transformCS(c0, b0, angleCS, 0, 0);

    // Скользящий отрезок
    double sin   = qSin(angle);
    double cos   = qCos(angle);
    double ctg   = c.getX() / c.getY();

    r.setX(h * (sin * ctg + cos));

    q.setX(h * sin * ctg);
    q.setY(h * sin);

    //3-я вершина
    m.setX(h * sin * ctg + d1 * cos);
    m.setY((h - d1) * sin);

    t.setX(m.getX() + d2 * sin);
    t.setY(m.getY() + d2 * cos);

    //Преобразование системы координат к исходной
    r = transformCS(r, o, -angleCS, b0.getX(), b0.getY());
    q = transformCS(q, o, -angleCS, b0.getX(), b0.getY());
    t = transformCS(t, o, -angleCS, b0.getX(), b0.getY());

    result.push_back(r);
    result.push_back(q);
    result.push_back(t);

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

bool Tools::intersection(Point a, Point b, Point c, Point d, Point *intersection) {
    Point dir1 = Point(b.getX() - a.getX(), b.getY() - a.getY());
    Point dir2 = Point(d.getX() - c.getX(), d.getY() - c.getY());

    //считаем уравнения прямых проходящих через отрезки
    double a1 = -dir1.getY();
    double b1 = +dir1.getX();
    double c1 = -(a1*a.getX() + b1*a.getY());

    double a2 = -dir2.getY();
    double b2 = +dir2.getX();
    double c2 = -(a2*c.getX() + b2*c.getY());

    //подставляем концы отрезков, для выяснения в каких полуплоскотях они
    double seg1_line2_start = a2*a.getX() + b2*a.getY() + c2;
    double seg1_line2_end = a2*b.getX() + b2*b.getY() + c2;

    double seg2_line1_start = a1*c.getX() + b1*c.getY() + c1;
    double seg2_line1_end = a1*d.getX() + b1*d.getY() + c1;

    //если концы одного отрезка имеют один знак, значит он в одной полуплоскости и пересечения нет.
    if (seg1_line2_start * seg1_line2_end >= 0 || seg2_line1_start * seg2_line1_end >= 0) {
        return false;
    }

    double u = seg1_line2_start / (seg1_line2_start - seg1_line2_end);
    *intersection =  Point(dir1.getX() * u + a.getX(), dir1.getY() * u + a.getY());

    return true;
}

//Преобразование системы координат
Point Tools::transformCS(Point p, Point o, double a, double x, double y) {
    Point result;
    double cos = qCos(a);
    double sin = qSin(a);

    result.setX((p.getX() - o.getX()) * cos + (p.getY() - o.getY()) * sin + x);
    result.setY((p.getY() - o.getY()) * cos - (p.getX() - o.getX()) * sin + y);

    return result;
}

//Знак числа
int Tools::sign(double number)
{
    if (number < 0) return -1;
    if (number == 0) return 0;
    return 1;
}

//Принадлежит ли точка c прямой ab
int Tools::outPoint(Point a, Point b, Point c) {
    int result = 0;

    double A = a.getY() - b.getY();
    double B = -(a.getX() - b.getX());
    double C = -(A * a.getX() + B * a.getY());

    double equation = A * c.getX() + B * c.getY() + C;

    if (equation > 0) {
        result = 1;
    } else if (equation < 0) {
        result = -1;
    }

    return result;
}

//Пересечение 2-х прямых
bool Tools::llInter(Point a, Point b, Point c, Point d, Point *p) {
    Point dir1 = Point(b.getX() - a.getX(), b.getY() - a.getY());
    Point dir2 = Point(d.getX() - c.getX(), d.getY() - c.getY());

    //считаем уравнения прямых проходящих через отрезки
    double a1 = -dir1.getY();
    double b1 = +dir1.getX();
    double c1 = -(a1*a.getX() + b1*a.getY());

    double a2 = -dir2.getY();
    double b2 = +dir2.getX();
    double c2 = -(a2*c.getX() + b2*c.getY());

    //подставляем концы отрезков, для выяснения в каких полуплоскотях они
    double seg1_line2_start = a2*a.getX() + b2*a.getY() + c2;
    double seg1_line2_end = a2*b.getX() + b2*b.getY() + c2;

    double u = seg1_line2_start / (seg1_line2_start - seg1_line2_end);

    //Прямые параллельны
    if (u == INFINITY) {
        return false;
    }

    *p = Point(dir1.getX() * u + a.getX(), dir1.getY() * u + a.getY());

    return true;
}

//Здвиг точки
void Point::shift(Point s)
{
    this->x += s.getX();
    this->y += s.getY();
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
