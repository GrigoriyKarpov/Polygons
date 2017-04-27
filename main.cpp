#include "window.h"
#include "tools.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;
    window.show();

    return app.exec();
}
