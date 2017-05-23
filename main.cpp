#include "window.h"
#include "Geometry/point.h"
#include "Geometry/vector.h"

#include <qmath.h>
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;

    window.show();
    window.moveToCenter();

//    Point a = Point(0.0, 0.0);
//    Point b = Point(10.0, 0.0);
//    Point c = Point(-10.0, -1.0);

//    qDebug() << Tools::outPoint(a, b, c);

    return app.exec();
}
