#include "window.h"
#include "tools.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
//    Window window;
//    window.show();

    Point  a = Point(10.2, 3.4);
    Point  b = Point(2.2, -3.1);
    Point  c = Point(-3.2, 8.6);
    double h = 1.0;

    QVector<Point> v = Tools::lineSlipInCorner(a, b, c, h);

    //qDebug() << result;
    qDebug() << v.value(0).toString();
    qDebug() << v.value(1).toString();
    qDebug() << v.value(2).toString();
    qDebug() << v.value(3).toString();

    return app.exec();
}
