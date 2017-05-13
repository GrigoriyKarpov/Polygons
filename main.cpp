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
//    Point b = Point(0.0, 10.0);
//    Point c = Point(2.0, 0.0);
//    Point d = Point(1.999, 10.0);

//    Point res;

//    if (Tools::llInter(a, b, c, d, &res)) {
//        qDebug() << res.toString();
//    }

    return app.exec();
}
