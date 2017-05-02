#include "window.h"
#include "tools.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;

    window.show();
    window.moveToCenter();

//    Point a = Point(0.0, 0.0);
//    Point b = Point(10.0, 10.0);
//    Point c = Point(10.0, 0.0);
//    Point d = Point(0.0, 10.0);

//    QVector<Point> t = Tools::intersect(a, b, c, d);

//    if (t.count() > 0) {
//        qDebug() << t[0].toString();
//    }

    return app.exec();
}
