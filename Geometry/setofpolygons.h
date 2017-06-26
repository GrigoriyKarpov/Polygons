#ifndef SETOFPOLYGONS_H
#define SETOFPOLYGONS_H

#include "polygon.h"
#include <QVector>
#include <QPainter>

class SetOfPolygons {
private:
    QVector<Polygon> set;
    QVector<QString> names;
    QVector<QPen> pens;
    QVector<QBrush> brushes;

    QPen standartPen;
    QBrush standartBrush;

public:
    SetOfPolygons();
    SetOfPolygons(QPen pen, QBrush brush);

    void clear();

    void remove(int i);

    void add(Polygon p);
    void add(Polygon p, QString name);
    void add(Polygon p, QString name, QPen pen);
    void add(Polygon p, QString name, QPen pen, QBrush brush);
    void add(Polygon p, QPen pen);
    void add(Polygon p, QPen pen, QBrush brush);

    QString getName(int i);
    QPen getPen(int i);
    QString getXML(int i);

    int count();

    Polygon& operator[] (int n);
};

#endif // SETOFPOLYGONS_H
