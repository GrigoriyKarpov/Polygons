#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "Geometry/tools.h"
#include "Geometry/polygon.h"
#include "Geometry/setofpolygons.h"
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
    enum Modes {Draw, Edit, Demo, Move, Demo2, Demo3};
    QPen getPolygonPen();
    void setMode(GLWidget::Modes mode);

public slots:
    void recieveOptions(QPen pen);
    void animate();

protected:
    void paintEvent(QPaintEvent *event);

    //События с мышью
    void mouseMoveEvent(QMouseEvent *me);
    void mousePressEvent(QMouseEvent *me);
    void mouseReleaseEvent(QMouseEvent *me);
    void wheelEvent(QWheelEvent *me);

private:
    //Ширина и высота виджета
    int widgetHeight;
    int widgetWidth;

    //Режим
    Modes mode;

    //Навигация и управление
    //Текущее положение указателя мыши в мировых координатах
    Point gPos;
    //Текущее положение указателя мыши в координатах виджета
    Point mPos;

    //Инструмент перемещение вида
    //Включение режима перемещения вида
    bool move;

    //Сдвиг экранных координат относительно мировых
    Point shiftView;
    Point shiftBeforMove;
    Point startMovePos;
    Point shiftPos3;

    //Кнопки мыши
    bool leftBtn;
    bool rightBtn;
    bool wheel;

    //Коэффициент масштабирования экранных координат относительно мировых
    double gScale;
    double minScale;
    double maxScale;

    //Радиус выделения вершины
    int activationRadius;

    //Анимация
    int elapsed;
    int animationSuperior;

    //Флажки
    bool inter_f;
    bool slip;
    bool axis;
    bool crossroad;
    bool shortEdge;
    bool showLog;

    //Рисование
    QPainter painter;

    QFont textFont;
    QPen textPen;
    QPen polygonPen;
    QPen unacceptablePen;
    QPen activePen;
    QPen crossroadPen;

    //Данные
    SetOfPolygons setOfPolygons;
    Polygon::Polygon p1;
    QVector<QVector <int> > points;
    QVector<Point> path;
    QVector<Point> inter;
    QVector<int> inter1;

    //Активные элементы данных
    int activePoint;
    int activePolygon;

    //Индексы
    int ii;
    int ij;
    int ik;

    //Сообщения
    QVector<QString> log;

    //Визуализация элементов
    Point toMPoint(Point p); //Перевод из мировых координат в эранные
    Point toGPoint(Point p); //Перевод из экранных координат в мировые
    Point toGPoint(QPoint p); //

    void gPoint(Point a);
    void gPoint(double a, double b);

    void gLine(Point a, Point b);
    void gLine(double a, double b, double c, double d);

    void gVector(Point v);
    void gVector(Point s, Point v);

    void gPolygon(Polygon::Polygon p);
    void gPolygon(Polygon::Polygon p, bool close);
    void gPolygon(Polygon::Polygon p, QString name);
    void gPolygon(Polygon::Polygon p, QBrush fill);
    //void gPolygon(QVector<Point> polygon, QPen pen);
    //void gPolygon(QVector<Point> polygon, QPen, QBrush fill);
    //void gPolygon(QVector<Point> polygon, QString name, QBrush fill);
    //void gPolygon(QVector<Point> polygon, QString name, QPen pen);
    //void gPolygon(QVector<Point> polygon, QString name, QPen pen, QBrush fill);

    void gPolygon(QPolygon p);

    void gText(Point p, QString text);

    //Выделение вокруг активной точки
    void gActive();

    void drawGrid();

    QPoint gQPoint(Point a);

    void printInfo();
};

#endif
