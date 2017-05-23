#include "setofpolygons.h"

SetOfPolygons::SetOfPolygons() {
    standartPen   = QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap);
    standartBrush = QBrush(QColor(255, 90, 90, 255), Qt::BDiagPattern);
}

SetOfPolygons::SetOfPolygons(QPen pen, QBrush brush) {
    standartPen   = pen;
    standartBrush = brush;
}

void SetOfPolygons::clear() {
    set.clear();
    names.clear();
    pens.clear();
    brushes.clear();
}

void SetOfPolygons::add(Polygon p, QString name, QPen pen, QBrush brush) {
    set.push_back(p);
    names.push_back(name);
    pens.push_back(pen);
    brushes.push_back(brush);
}

void SetOfPolygons::add(Polygon p) {
    add(p, "", standartPen, standartBrush);
}

void SetOfPolygons::add(Polygon p, QString name) {
    add(p, name, standartPen, standartBrush);
}

void SetOfPolygons::add(Polygon p, QString name, QPen pen) {
    add(p, name, pen, standartBrush);
}

void SetOfPolygons::add(Polygon p, QPen pen) {
    add(p, "", pen, standartBrush);
}

void SetOfPolygons::add(Polygon p, QPen pen, QBrush brush) {
    add(p, "", pen, brush);
}

int SetOfPolygons::count() {
    return set.count();
}

Polygon &SetOfPolygons::operator[](int n) {
    return set[n];
}
