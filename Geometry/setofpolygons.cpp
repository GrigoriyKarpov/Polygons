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

void SetOfPolygons::remove(int i) {
    set.remove(i);
    names.remove(i);
    pens.remove(i);
    brushes.remove(i);
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

QString SetOfPolygons::getName(int i) {
    return names[i];
}

QPen SetOfPolygons::getPen(int i){
    return pens[i];
}

QString SetOfPolygons::getXML(int i) {
    QString result = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<polygon>\n";
    result += "\t<name>" + names[i] + "</name>\n";

    //----------------Точки-------------------
    result += "\t<points>\n";
    for (int j = 0; j < set[i].count(); j++) {
        result += "\t\t<point>\n\t\t\t<x>" + QString().sprintf("%f", set[i][j].getX()) + "</x>\n";
        result += "\t\t\t<y>" + QString().sprintf("%f", set[i][j].getY()) + "</y>\n\t\t</point>\n";
    }
    result += "\t</points>\n";

    result += "</polygon>";
    return result;
}

int SetOfPolygons::count() {
    return set.count();
}

Polygon &SetOfPolygons::operator[](int n) {
    return set[n];
}

