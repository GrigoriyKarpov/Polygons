#include "window.h"
#include "Geometry/point.h"
#include "Geometry/polygon.h"

#include <QApplication>
#include <QDebug>
#include <QVector>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;

    window.show();
    window.moveToCenter();

    Polygon::Polygon p;

    p.addPoint(Point(10.0, 7.0));
    p.addPoint(Point(3.0, 2.0));
    p.addPoint(Point(9.0, 5.0));

    qDebug() << p[2].toString();

//    Point a = Point(0.0, 0.0);
//    Point b = Point(0.0, 10.0);
//    Point c = Point(2.0, 0.0);
//    Point d = Point(1.999, 10.0);

//    Point res;

//    if (Tools::llInter(a, b, c, d, &res)) {
//        qDebug() << res.toString();
//    }

    return app.exec();
}
