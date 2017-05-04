#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "tools.h"
#include <options.h>
#include <QGLWidget>
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent);
    enum Modes {Draw, Edit, Demo, Move};
    QPen getPolygonPen();
    void setMode(GLWidget::Modes mode);

public slots:
    void recieveOptions(QPen pen);
    void animate();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *me);       // Метод реагирует на перемещение указателя, но по умолчанию setMouseTracking(false)
    void mousePressEvent(QMouseEvent *me);      // Реагирует на нажатие кнопок мыши
    void mouseReleaseEvent(QMouseEvent *me);    // Метод реагирует на "отжатие" кнопки мыши
    void wheelEvent(QWheelEvent *me);

private:
    //Переменные для перемещения вида
    int gX;
    int gY;
    int cbx;
    int cby;
    int ccx;
    int ccy;

    double gScale;

    QPainter painter;

    //Ширина и высота виджета
    int height;
    int width;

    //Таймер для анимации
    int elapsed;

    //Текущие координаты мыши на виджете
    int cax;
    int cay;

    //Радиус выделения вершины
    int active;

    //Флажки
    bool inter_f;
    bool slip;
    bool axis;
    bool move;
    bool leftBtn;
    bool wheel;
    bool crossroad;
    Modes mode;

    //Инструменты рисования
    QFont textFont;
    QPen textPen;
    QPen polygonPen;
    QPen circlePen;
    QPen crossroadPen;

    //Массив добавленных вершин
    QVector<QVector <int> > points;
    QVector<Point> path;
    QVector<Point> inter;
    QVector<int> inter1;
    //Индекс точки, на которую наведен курсор (по умолчанию -1)
    int activePoint;

    bool isCrossroad();

    int ii;
    int ij;
    int ik;

    QVector<QString> log;

    void gLine(Point a, Point b);
    void gLine(double a, double b, double c, double d);
    void gPoint(Point a);
    void gPoint(double a, double b);
};

#endif
