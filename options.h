#ifndef OPTIONS_H
#define OPTIONS_H

#include <QPen>

class Options
{
    Q_OBJECT

public:
    //options(QColor color, int width, Qt::PenStyle style);
    //set(QColor color, int width, Qt::PenStyle style);
    //Options get(QColor color, int width, Qt::PenStyle style);

private:
    QColor color;
    int width;
    Qt::PenStyle style;
};

#endif // OPTIONS_H
