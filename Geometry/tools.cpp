#include "tools.h"
#include "point.h"

#include <qmath.h>
#include <QPair>
#include <QVector>

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

double Tools::slope(Point a, Point b) {
    return (b.getY() - a.getY()) / (b.getX() / a.getX());
}

//Пересечение 2-х прямых
bool Tools::lineIntersect(Point a, Point b, Point c, Point d, Point *p) {
    //Прямые параллельны, если у них одинаковый угловой коэффициент
    if (slope(a, b) == slope(c, d)) {
        return false;
    }

    Vector dir1 = Vector(a, b);
    Vector dir2 = Vector(c, d);

    double a2 = -dir2.getY();
    double b2 = +dir2.getX();
    double c2 = -(a2 * c.getX() + b2 * c.getY());

    double seg1_line2_start = a2 * a.getX() + b2 * a.getY() + c2;
    double seg1_line2_end   = a2 * b.getX() + b2 * b.getY() + c2;

    double u = seg1_line2_start / (seg1_line2_start - seg1_line2_end);

    *p = Point(dir1.getX() * u + a.getX(), dir1.getY() * u + a.getY());

    return true;
}

//Пересечение отрезков
bool Tools::segmentIntersect(Point a, Point b, Point c, Point d, Point *intersection) {
    Vector dir1 = Vector(a, b);
    Vector dir2 = Vector(c, d);

    //считаем уравнения прямых проходящих через отрезки
    double a1 = -dir1.getY();
    double b1 = +dir1.getX();
    double c1 = -(a1*a.getX() + b1*a.getY());

    double a2 = -dir2.getY();
    double b2 = +dir2.getX();
    double c2 = -(a2 * c.getX() + b2 * c.getY());

    //подставляем концы отрезков, для выяснения в каких полуплоскотях они
    double seg1_line2_start = a2 * a.getX() + b2 * a.getY() + c2;
    double seg1_line2_end   = a2 * b.getX() + b2 * b.getY() + c2;

    double seg2_line1_start = a1 * c.getX() + b1 * c.getY() + c1;
    double seg2_line1_end   = a1 * d.getX() + b1 * d.getY() + c1;

    //если концы одного отрезка имеют один знак, значит он в одной полуплоскости и пересечения нет.
    if (seg1_line2_start * seg1_line2_end >= 0 || seg2_line1_start * seg2_line1_end >= 0) {
        return false;
    }

    double u = seg1_line2_start / (seg1_line2_start - seg1_line2_end);
    *intersection =  Point(dir1.getX() * u + a.getX(), dir1.getY() * u + a.getY());

    return true;
}

int Tools::angleDirection(Point a, Point b, Point c) {
    return sign((a.getX() * b.getY() + b.getX() * c.getY() + c.getX() * a.getY()) -
                (a.getX() * c.getY() + b.getX() * a.getY() + c.getX() * b.getY()));
}

bool Tools::geometrySearch(Polygon pol, Point p) {
    for (int i = 1; i < pol.count() - 1; i++) {
        if (angleDirection(p, pol[0], pol[i + 1]) > 0 &&
            angleDirection(p, pol[0], pol[i]) < 0 &&
            angleDirection(pol[i], pol[i + 1], p) < 0) {
            return true;
        }
    }

    return false;
}

Polygon Tools::outCharArea(Polygon p1, Polygon p2, int o) {
    //1 Нормали многоугольников
    Polygon result;
    QVector< QPair< int, Point > > charPoints;
    QVector<Vector> n;
    QVector<Vector> m;

    for (int i = 0; i < p1.count(); i++) {
        int j = i + 1;
        if (i == p1.count() - 1) {
            j = 0;
        }

        n.push_back(Vector::normal(p1[i], p1[j]));
    }

    for (int i = 0; i < p2.count(); i++) {
        int j = i + 1;
        if (i == p2.count() - 1) {
            j = 0;
        }

        m.push_back(Vector::normal(p2[i], p2[j]));
    }

    //2 Нахождение векторов движения

    //2.1 Для каждого ребра неподв. находим верш. подвиж. которая может скользить по нему
    for (int i = 0; i < n.count(); i++) {
        int j = i + 1;
        if (i == n.count() - 1) {
            j = 0;
        }

        for (int k = 0; k < p2.count(); k++) {
            int t = k + 1;
            if (k == p2.count() - 1) {
                t = 0;
            }
            int l = k - 1;
            if (k == 0) {
                l = p2.count() - 1;
            }

            //Находим углы
            double a1 = n[i].cos(Vector(p2[k], p2[t]));
            double a2 = n[i].cos(Vector(p2[k], p2[l]));

            //Находим вершину, которая может скользить
            if (a1 >= 0 && a2 >= 0) {
                Point v2 = p1[j];
                v2 = Vector(p2[k], p2[o]).translate(v2);

                charPoints.push_back(QPair< int, Point >(i, v2));

                break;
            }
        }
    }

    //2.2.	Для кажд. ребра подвиж. находим верш. неподвиж.
    for (int i = m.count() - 1; i >= 0; i--) {
        int j = i + 1;
        if (i == m.count() - 1) {
            j = 0;
        }

        for (int k = 0; k < p1.count() - 1; k++) {
            int t = k + 1;
            if (k == p1.count() - 1) {
                t = 0;
            }
            int l = k - 1;
            if (k == 0) {
                l = p1.count() - 1;
            }

            //Находим углы
            double a1 = m[i].cos(Vector(p1[k], p1[t]));
            double a2 = m[i].cos(Vector(p1[k], p1[l]));

            //Находим вершину, которая может скользить
            if (a1 >= 0 && a2 >= 0) {
                Point v2 = p1[k];
                v2 = Vector(p2[j], p2[o]).translate(v2);

                charPoints.push_back(QPair< int, Point >(k, v2));

                break;
            }
        }
    }


    //3. Построим характеристический многоугольник, для этого упорядочим пары
    for (int i = 0; i < p1.count(); i++) {
        for (int j = charPoints.count() - 1; j >= 0 ; j--) {
            if (charPoints[j].first == i) {
                result.addPoint(charPoints[j].second);
            }
        }
    }

    return result;
}

Polygon Tools::inCharArea(Polygon p1, Polygon p2, int o) {

    QVector<Vector> n;
    QVector<Vector> m;

    //Внутр
    for (int i = 0; i < p1.count(); i++) {
        int j = i + 1;
        if (i == p1.count() - 1) {
            j = 0;
        }
        n.push_back(Vector::normal(p1[j], p1[i]));
    }

    //Внешние нормали
    for (int i = 0; i < p2.count(); i++) {
        int j = i + 1;
        if (i == p2.count() - 1) {
            j = 0;
        }

        m.push_back(Vector::normal(p2[i], p2[j]));
    }
    QVector<Point> charPoints;

    for (int i = 0; i < n.count(); i++) {
        int j = i + 1;
        if (i == n.count() - 1) {
            j = 0;
        }

        for (int k = 0; k < p2.count(); k++) {
            int t = k + 1;
            if (k == p2.count() - 1) {
                t = 0;
            }
            int l = k - 1;
            if (k == 0) {
                l = p2.count() - 1;
            }

            //Находим углы
            double a1 = n[i].cos(Vector(p2[k], p2[t]));
            double a2 = n[i].cos(Vector(p2[k], p2[l]));

            //Находим вершину, которая может скользить
            if (a1 >= 0 && a2 >= 0) {
                Point v1 = p1[i];
                v1 = Vector(p2[k], p2[o]).translate(v1);
                Point v2 = p1[j];
                v2 = Vector(p2[k], p2[o]).translate(v2);

                charPoints.push_back(v1);
                charPoints.push_back(v2);

                break;
            }
        }
    }

    Polygon::Polygon inter;

    for (int i = 0; i < charPoints.count(); i += 2) {
        int j = i + 1;
        int k = i + 2;
        int t = i + 3;

        if (i == charPoints.count() - 1) {
            j = 0;
            k = 1;
            t = 2;
        }
        if (i == charPoints.count() - 2) {
            k = 0;
            t = 1;
        }
        if (i == charPoints.count() - 3) {
            t = 0;
        }

        Point res;

        if (Tools::segmentIntersect(charPoints[i], charPoints[j],
                                    charPoints[k], charPoints[t], &res)) {
            inter.addPoint(res);
        }

        int m = t + 1;
        if (t == charPoints.count() - 1) {
            m = 0;
        }
        if (charPoints[t] == charPoints[m]) {
            inter.addPoint(charPoints[t]);
        }
    }

    return inter;
}

Point Tools::center(Point a, Point b) {
    return Point(a.getX() + (b.getX() - a.getX()) / 2.0, a.getY() + (b.getY() - a.getY()) / 2.0);
}

//Косинус угла abc
double Tools::cos(Point a, Point b, Point c) {
    Vector v1 = Vector(b, a);
    Vector v2 = Vector(b, c);

    return v1.cos(v2);
}
