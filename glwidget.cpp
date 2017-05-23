#include <QtGui>
#include "glwidget.h"
#include "Geometry/tools.h"
#include "Geometry/point.h"
#include "Geometry/polygon.h"

#include <QPainter>
#include <QtCore/qmath.h>
#include <QDesktopWidget>
#include <algorithm>
#include <QCursor>
#include <QDebug>

GLWidget::GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    //Настройки виджета
    widgetHeight = 600;
    widgetWidth = 800;

    setFixedSize(widgetWidth, widgetHeight);
    setMouseTracking(true);

    activePolygon = 0;
    setOfPolygons = SetOfPolygons();

    //Инициализация переменных
    elapsed = 0;

    gPos = Point();
    mPos = Point();
    shiftView = Point(widgetWidth / 2, -widgetHeight / 2);
    startMovePos = Point();
    shiftPos3 = shiftView;

    gScale = 1.0;
    minScale = 0.01;
    maxScale = 10.0;

    ii = 0;
    ij = 1;
    ik = 2;

    activationRadius  = 10;
    activePoint = -1;

    animationSuperior = 100;

    inter_f = true;
    slip = false;
    axis = true;
    move = false;
    crossroad = false;
    leftBtn = false;
    rightBtn = false;
    showLog = true;

    wheel = false;
    shortEdge = false;
    setMode(Draw);

    textPen = QPen(QColor(96, 96, 96));
    textFont.setPixelSize(12);
    polygonPen = QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap);
    unacceptablePen = QPen(Qt::red, 3, Qt::DotLine, Qt::RoundCap);
    activePen = QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap);
    crossroadPen = QPen(Qt::red, 2, Qt::DotLine, Qt::RoundCap);
}

void GLWidget::paintEvent(QPaintEvent *event) {
//    log.push_back("gPos           = " + gPos.toString());
//    log.push_back("mPos           = " + mPos.toString());
//    log.push_back("shiftView      = " + shiftView.toString());
//    log.push_back("startMovePos   = " + startMovePos.toString());
//    log.push_back("shiftBeforMove = " + shiftBeforMove.toString());
//    log.push_back("shiftPos3      = " + shiftPos3.toString());
//    log.push_back("");
//    Point ttt = mPos - startMovePos;
//    log.push_back("mPos - startMovePos = " + ttt.toString());

    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(event->rect(), QBrush(Qt::white));

    if (axis) {
        drawGrid();
    }

    if (mode == Demo) {
        //--------------------- Инициализация переменных ---------------------//
        QVector<Point> polygon;
        polygon.push_back(Point(200.0, -250.0));
        polygon.push_back(Point(-150.0, -150.0));
        polygon.push_back(Point(50.0, 130.0));
        polygon.push_back(Point(130.0, 130.0));
        polygon.push_back(Point(160.0, 80.0));

        double h  = 100.0;   //Длина скользящего отрезка
        double d1 = 0.0;     //Раст от верш. скользящего отрезка до основания перпендикуляра
        double d2 = 300.0;   //Длина перпендикуляра
        double speed = 1.0;  //Коэффициент скорости анимации

        Point r;
        Point q;
        Point t;

        double angle = 0;

//        log.push_back(QString::number(ii));
//        log.push_back(QString::number(ij));
//        log.push_back(QString::number(ik));

        if (inter_f) {
            //Выбор между скольжением в угле и скольжением по грани
            if (!slip) {
                angle = M_PI * elapsed / 180.0 * speed;  //Отрезок вращается с анимацией

                QVector<Point> tmp = Tools::slipInAngle(polygon[ii], polygon[ij], polygon[ik], h, angle, d1, d2);

                r = tmp[0];  //Скользит по [b,a]
                q = tmp[1];  //Скользит по [b,c]
                t = tmp[2];  //Третья вершина

                //Если грань маленькая
                int il = ik + 1;
                if (ii == polygon.count() - 3) {
                    il = 0;
                }

                if (polygon[ij].dist(polygon[ik]) < h &&
                    polygon[ij].dist(polygon[ik]) <= polygon[ij].dist(q) &&
                    polygon[il].dist(polygon[ik]) <= polygon[il].dist(q) + 1) {
                    shortEdge = true;
                }

                if (shortEdge) {
                    //log.push_back("true");
                    Point v_point;

                    if (Tools::lineIntersect(polygon[ii], polygon[ij], polygon[ik], polygon[il], &v_point)) {
                        tmp.clear();
                        tmp = Tools::slipInAngle(polygon[ii], v_point, polygon[ik], h, angle, d1, d2);

                        r = tmp[0];  //Скользит по [b,a]
                        q = tmp[1];  //Скользит по [b,c]
                        t = tmp[2];  //Третья вершина

                        path.push_back(t);

                        if (polygon[ii].dist(polygon[ij]) <= polygon[ii].dist(r)) {
                            shortEdge = false;

                            if (ii == polygon.count() - 3) {
                                ii++;
                                ij++;
                                ik = 0;
                            } else if (ii == polygon.count() - 2) {
                                ii++;
                                ij = 0;
                                ik++;
                            } else if (ii == polygon.count() - 1) {
                                ii = 0;
                                ij++;
                                ik++;
                                inter_f = false;
                                path.push_back(path[0]);
                            } else {
                                ii++;
                                ij++;
                                ik++;
                            }

                            elapsed = 0;
                            angle = M_PI * elapsed / 180.0 * speed;
                            tmp.clear();
                            tmp = Tools::slipInAngle(polygon[ii], polygon[ij], polygon[ik], h, angle, d1, d2);

                            while(polygon[ij].dist(polygon[ii]) < polygon[ij].dist(tmp[0])) {
                               elapsed++;
                               angle = M_PI * elapsed / 180.0 * speed;
                               tmp.clear();
                               tmp = Tools::slipInAngle(polygon[ii], polygon[ij], polygon[ik], h, angle, d1, d2);
                            }

                        }
                    }
                } else {
                    //log.push_back("false");
                    //Переход к скольжению по грани ij, ik
                    if (angle > M_PI - qAcos(Tools::cos(polygon[ii], polygon[ij], polygon[ik]))) {
                        slip    = true;
                        elapsed = 0;
                    } else {
                        path.push_back(t);
                    }
                }
            } else {
                //Скользим по грани ij, ik
                Point s;

                s.setX(((polygon[ik].getX() - polygon[ij].getX()) / 100.0) * elapsed * speed);
                s.setY(((polygon[ik].getY() - polygon[ij].getY()) / 100.0) * elapsed * speed);

                angle = M_PI - qAcos(Tools::cos(polygon[ii], polygon[ij], polygon[ik]));
                QVector<Point> tmp = Tools::slipInAngle(polygon[ii], polygon[ij], polygon[ik], h, angle, d1, d2);

                for (int i = 0; i < 3; i++) {
                    tmp[i] += s;
                }

                r = tmp[0];  //Скользит по [b,a]
                q = tmp[1];  //Скользит по [b,c]
                t = tmp[2];  //Третья вершина

                //Если достигли вершины ik - переход на следующий угол
                if (polygon[ij].dist(polygon[ik]) <= polygon[ij].dist(q)) {
                    if (ii == polygon.count() - 3) {
                        ii++;
                        ij++;
                        ik = 0;
                    } else if (ii == polygon.count() - 2) {
                        ii++;
                        ij = 0;
                        ik++;
                    } else if (ii == polygon.count() - 1) {
                        ii = 0;
                        ij++;
                        ik++;
                        inter_f = false;
                        path.push_back(path[0]);
                    } else {
                        ii++;
                        ij++;
                        ik++;
                    }

                    slip = false;
                    elapsed = 0;
                } else {
                    path.push_back(t);
                }
            }

            //Точки пересечения траектории и сторон многоугольника
            if (path.count() > 1) {
                int k = path.count() - 1;
                Point res;

                for (int i = 0; i < polygon.count(); i++) {
                    int j = i + 1;
                    if (i == polygon.count() - 1) {
                        j = 0;
                    }

                    if (Tools::segmentIntersect(polygon[i], polygon[j], path[k - 1], path[k], &res)) {
                        inter.push_back(res);
                        inter1.push_back(Tools::outPoint(polygon[i], polygon[j], path[k]));
                        break;
                    }
                }
            }
        } else {
            QVector<Point> tmp = Tools::slipInAngle(polygon[ii], polygon[ij], polygon[ik], h, angle, d1, d2);

            r = tmp[0];  //Скользит по [b,a]
            q = tmp[1];  //Скользит по [b,c]
            t = tmp[2];  //Третья вершина

            elapsed = 0;
        }
        //--------------------- Визуализация ---------------------//
        //Полигон
        painter.setPen(polygonPen);
        for (int i = 0; i < polygon.count(); i++) {
            int j = i + 1;
            if (i == polygon.count() - 1) {
                j = 0;
            }
            gLine(polygon[i], polygon[j]);
        }

        //Отрезок
        painter.setPen(QPen(Qt::red, 2, Qt::SolidLine));
        gLine(r, q);

        //Отрезки до третьей вершины
        painter.setPen(QPen(Qt::red, 2, Qt::DotLine));
        gLine(q, t);
        gLine(r, t);

        //Траектория и область
        if (path.count() > 1) {
            QPolygon charArea;
            QPainterPath charAreaPath;

            charArea << gQPoint(path[0]);

            painter.setPen(QPen(Qt::gray, 2, Qt::SolidLine));
            for (int i = 0; i < path.count() - 1; i++) {
                gLine(path[i], path[i + 1]);
                charArea << gQPoint(path[i + 1]);
            }

            charAreaPath.addPolygon(charArea);

            //Заливка
            QBrush fillBrush(QColor(255, 90, 90, 255), Qt::BDiagPattern); // Вариант со шриховкой
            //QBrush fillBrush(QColor(255, 230, 190, 180), Qt::SolidPattern); // Вариант полупрозрачной заливки

            painter.fillPath(charAreaPath, fillBrush);
        }

        //Вершины...
        painter.setPen(QPen(Qt::black, 8, Qt::SolidLine, Qt::RoundCap));

        //...полигона
        for (int i = 0; i < polygon.count(); i++) {
            gPoint(polygon[i]);
        }

        //...отрезка
        gPoint(r);
        gPoint(q);

        //Третья точка
        gPoint(t);

        //Точки пересечения траектории
        if (inter.count() > 0) {
            for (int i = 0; i < inter.count(); i++) {
                if (inter1[i] == 1) {
                    painter.setPen(QPen(Qt::red, 12, Qt::SolidLine, Qt::RoundCap));
                } else {
                    painter.setPen(QPen(Qt::green, 12, Qt::SolidLine, Qt::RoundCap));
                }
                gPoint(inter[i]);
            }
        }

        //Текст
        painter.setPen(textPen);
        painter.setFont(textFont);

        //Координаты точек пересечения траектории
        textFont.setBold(true);
        textFont.setPixelSize(14);
        painter.setFont(textFont);
        painter.drawText(QRect(4,  2, 200, 16), Qt::AlignLeft,
                                 QString::fromUtf8("Точки пересечения:"));

        textFont.setBold(false);
        painter.setFont(textFont);

        if (inter.count() > 0) {
            for (int i = 0; i < inter.count(); i++) {
                QString text = QString::number(i + 1) + ". "
                        + inter[i].toString() + " ";
                if (inter1[i] == 1) {
                    text += "out";
                } else {
                    text += "in";
                }
                QString::number(inter1[i]);
                painter.drawText(QRect(4,  20 + 18 * i, 200, 16), Qt::AlignLeft, text);
            }
        }
        textFont.setPixelSize(12);
        painter.setFont(textFont);
    }

    //--------------------------------- end demo mode ---------------------------------//

    if (mode == Demo2) {
        QVector<Point> polygon1;
        polygon1.push_back(Point(0.0, -250.0));
        polygon1.push_back(Point(-350.0, -150.0));
        polygon1.push_back(Point(-150.0, 130.0));
        polygon1.push_back(Point(-70.0, 130.0));
        polygon1.push_back(Point(-40.0, 80.0));

        int end = 600;
        if (elapsed > end) {
            elapsed = end;
        }
        double step = - elapsed;

        QVector<Point> polygon2;
        polygon2.push_back(Point(400.0 + step, -120.0));
        polygon2.push_back(Point(100.0 + step, -150.0));
        polygon2.push_back(Point(150.0 + step, 180.0));
        polygon2.push_back(Point(330.0 + step, 180.0));
        polygon2.push_back(Point(360.0 + step, 130.0));

        //Визуализация
        //Области внутри полигонов и пересечения
        QPolygon area1;
        for (int i = 0; i < polygon1.count(); i++) {
            area1 << gQPoint(polygon1[i]);
        }

        QPolygon area2;
        for (int i = 0; i < polygon2.count(); i++) {
            area2 << gQPoint(polygon2[i]);
        }

        QPolygon area_tmp1 = area1;

        QPolygon area3 = area1.intersected(area2);

        area1 = area1.subtracted(area2);
        area2 = area2.subtracted(area_tmp1);

        QPainterPath areaPath1;
        QPainterPath areaPath2;
        QPainterPath areaPath3;

        areaPath1.addPolygon(area1);
        areaPath2.addPolygon(area2);
        areaPath3.addPolygon(area3);

        //Заливка
        QBrush fillBrush1(QColor(255, 100, 100, 120), Qt::SolidPattern);
        QBrush fillBrush2(QColor(100, 100, 255, 120), Qt::SolidPattern);
        QBrush fillBrush3(QColor(100, 255, 100, 120), Qt::SolidPattern);

        painter.fillPath(areaPath1, fillBrush1);
        painter.fillPath(areaPath2, fillBrush2);
        painter.fillPath(areaPath3, fillBrush3);

        //Полигоны
        gPolygon(polygon1, "A");
        gPolygon(polygon2, "B");
    }

    //Внешнаяя характеристическая область
    if (mode == Demo3) {
        QVector<Point> polygon1;
        polygon1.push_back(Point(0.0, -250.0));
        polygon1.push_back(Point(-350.0, -150.0));
        polygon1.push_back(Point(-150.0, 130.0));
        polygon1.push_back(Point(-70.0, 130.0));
        polygon1.push_back(Point(-40.0, 80.0));

        QVector<Point> polygon2;
        polygon2.push_back(Point(250.0, -40.0));
        polygon2.push_back(Point(140.0, -50.0));
        polygon2.push_back(Point(150.0, 70.0));
        polygon2.push_back(Point(230.0, 80.0));
        polygon2.push_back(Point(260.0, 10.0));

        Polygon::Polygon outCharArea = Tools::outCharArea(polygon1, polygon2, 3);

        //Визуализация
        gPolygon(polygon1, "A");
        gPolygon(polygon2, "B");

        gPolygon(outCharArea, QBrush(QColor(255, 90, 90, 255), Qt::BDiagPattern));
    }

    if (mode == Draw) {
        while (setOfPolygons.count() <= activePolygon) {
            setOfPolygons.add(Polygon::Polygon());
        }

        setOfPolygons[activePolygon].addPoint(gPos);
        gPolygon(setOfPolygons[activePolygon], false);

        if (setOfPolygons[activePolygon].isPolygon() && !setOfPolygons[activePolygon].isConvex()) {
            QString errorMessage = QString::fromUtf8("Предупреждение: полигон невыпуклый!");
            painter.setPen(QPen(QColor(255, 64, 64)));
            painter.drawText(QRect(4, widgetHeight - 20, 400, 20), Qt::AlignLeft, errorMessage);
        }

        setOfPolygons[activePolygon].removeLast();

        //Выделение вокруг активной точки
        gActive();
    }

    if (mode == Edit) {
        gPolygon(setOfPolygons[activePolygon]);

        if (!setOfPolygons[activePolygon].isConvex()) {
            QString errorMessage = QString::fromUtf8("Предупреждение: полигон невыпуклый!");
            painter.setPen(QPen(QColor(255, 64, 64)));
            painter.drawText(QRect(4, widgetHeight - 20, 400, 20), Qt::AlignLeft, errorMessage);
        }

        //Выделение вокруг активной точки
        gActive();
    }

    //Вывод блока с информацией
    printInfo();

    //Лог
    QPen  logPen  = QPen(Qt::black);
    QFont logFont = QFont("Courier", 10, 1, false);
    painter.setPen(logPen);
    painter.setFont(logFont);

    if (log.count() > 0 && showLog) {
        for (int i = 0; i < log.count(); i++) {
            painter.fillRect(QRect(0, 16 * i, 400, 16), QBrush(Qt::white));
            painter.drawText(QRect(4, 16 * i, 400, 16), Qt::AlignLeft, log[i]);
        }
        log.clear();
    }

    //Вывод ошибки
    if (crossroad) {
        QString errorMessage = "Warning: lines must not intersect!";
        painter.setPen(QPen(QColor(255, 64, 64)));
        painter.drawText(QRect(4, widgetHeight - 16, 400, 16), Qt::AlignLeft, errorMessage);
    }

    painter.end();

    update();
}

//Анимация
void GLWidget::animate() {
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % animationSuperior;
}

void GLWidget::setMode(GLWidget::Modes mode) {
    if (mode == Draw) {
        this->mode = mode;
        this->move = false;
        crossroad = false;
        activePoint = -1;
        points.clear();
        setCursor(Qt::CrossCursor);
    } else if (mode == Edit) {
        this->mode = mode;
        this->move = false;
        setCursor(Qt::ArrowCursor);
    } else if (mode == Demo) {
        this->mode = mode;
        points.clear();
        this->move = false;
        setCursor(Qt::ArrowCursor);
    } else if (mode == Move) {
        if (this->move != true) {
            this->move = true;

            if (wheel) {
                setCursor(Qt::ClosedHandCursor);
            } else {
                setCursor(Qt::OpenHandCursor);
            }
        } else {
            this->move = false;

            if (this->mode == Draw) {
                setCursor(Qt::CrossCursor);
            } else {
                setCursor(Qt::ArrowCursor);
            }
        }
    }
}

QPen GLWidget::getPolygonPen() {
    return polygonPen;
}

void GLWidget::recieveOptions(QPen pen) {
    polygonPen = pen;
}

void GLWidget::gLine(Point a, Point b) {
    gLine(a.getX(), a.getY(), b.getX(), b.getY());
}

void GLWidget::gLine(double a, double b, double c, double d) {
    painter.drawLine(a * gScale + shiftView.getX(),
                     widgetHeight - b * gScale + shiftView.getY(),
                     c * gScale + shiftView.getX(),
                     widgetHeight - d * gScale + shiftView.getY());
}

void GLWidget::gVector(Point v) {
    gVector(Point(), v);
}

void GLWidget::gVector(Point s, Point v) {
    QPolygon p;
    double arrow_widgetWidth = 50.0;
    double arrow_len = 100.0;

    p << QPoint( 0, -arrow_widgetWidth );
    p << QPoint( -arrow_len, 0 );
    p << QPoint( 0, arrow_widgetWidth );

    v += s;
    gLine(s, v);
}

void GLWidget::gPoint(Point a) {
    gPoint(a.getX(), a.getY());
}

void GLWidget::gPoint(double a, double b) {
    painter.drawPoint(a * gScale + shiftView.getX(),
                      widgetHeight - b * gScale + shiftView.getY());
}

void GLWidget::gPolygon(Polygon::Polygon p) {
    gPolygon(p, true);
}

void GLWidget::gPolygon(Polygon::Polygon p, bool close) {
    QPen pointPen = polygonPen;
    pointPen.setWidth(pointPen.width() + 5);

    if (p.isPolygon()) {
        for (int i = 0; i < p.count() - 1; i++) {
            painter.setPen(polygonPen);

            if (!p.isConvex() && !close && i == p.count() - 2) {
                painter.setPen(unacceptablePen);
            }

            gLine(p[i], p[i + 1]);

            painter.setPen(pointPen);
            gPoint(p[i]);
        }

        gPoint(p[p.count() - 1]);

        if (close) {
            painter.setPen(polygonPen);
            gLine(p[p.count() - 1], p[0]);
        }
    } else {
        painter.setPen(pointPen);
        for (int i = 0; i < p.count(); i++) {
            gPoint(p[i]);
        }

        if (p.count() == 2) {
            painter.setPen(polygonPen);
            gLine(p[0], p[1]);
        }
    }
}

void GLWidget::gPolygon(Polygon::Polygon p, QString name) {
    gPolygon(p);

    //Подписи
    QPen textPolygonPen = QPen();
    textPolygonPen.setWidth(5);

    QFont polygonFont1;
    polygonFont1.setPixelSize(18);

    QFont polygonFont2;
    polygonFont2.setPixelSize(15);

    QFont polygonFont3;
    polygonFont3.setPixelSize(22);

    painter.setPen(textPolygonPen);

    double dist = 13.0 / gScale;

    for (int i = 0; i < p.count(); i++) {
        Point c = p.pointLabel(i, dist);
        painter.setFont(polygonFont1);
        gText(c, name);

        painter.setFont(polygonFont2);
        c += Point(12 / gScale, -6 / gScale);
        gText(c, QString::number(i));
    }

    painter.setFont(polygonFont3);
    gText(p.center(), name);
}

void GLWidget::gPolygon(Polygon::Polygon p, QBrush fill) {
    QPolygon area;

    for (int i = 0; i < p.count(); i++) {
        area << gQPoint(p[i]);
    }

    QPainterPath areaPath;
    areaPath.addPolygon(area);
    painter.fillPath(areaPath, fill);

    gPolygon(p);
}

void GLWidget::gPolygon(QPolygon p) {
    QPolygon gp;

    for (int i = 0; i < p.count(); i++) {
        gp << QPoint(p[i].x() * gScale + shiftView.getX(),
                     widgetHeight - p[i].y() * gScale + shiftView.getY());
    }

    painter.drawConvexPolygon(gp);
}

void GLWidget::gText(Point p, QString text) {
    int fontSize = painter.font().pixelSize();

    double w = fontSize * text.length() + 20;
    double h = fontSize + 4;

    QRect rect = QRect(p.getX() * gScale + shiftView.getX() - w / 2,
                       widgetHeight - p.getY() * gScale + shiftView.getY() - h / 2,
                       w, h);

    //painter.fillRect(rect, QBrush(Qt::cyan));
    painter.drawText(rect, Qt::AlignCenter, text);
}

void GLWidget::gActive() {
    //log.push_back(QString::number(activePoint));
    if (activePoint != -1) {
        painter.setPen(activePen);
        Point tmp = toMPoint(setOfPolygons[activePolygon][activePoint]);
        painter.drawEllipse(tmp.getX() - activationRadius, tmp.getY() - activationRadius,
                            activationRadius * 2, activationRadius * 2);
    }
}

//Нарисовать сетку и оси
void GLWidget::drawGrid() {
    int startX =  -shiftView.getX() / gScale - 1;
    int endX   = (-shiftView.getX() + widgetWidth) / gScale + 1;
    int startY =   shiftView.getY() / gScale - 1;
    int endY   =  (shiftView.getY() + widgetHeight) / gScale + 1;
    double step = 50.0;

    while (qFabs(endX - startX) / step > 30) {
        step *= 2;
    }
    while (qFabs(endX - startX) / step < 5) {
        step /= 2;
    }

    //Сетка
    painter.setPen(QPen(Qt::gray, 1, Qt::SolidLine));

    //Горизонтальные линии
    for (int i = 0; i < endY; i += step) {
        gLine(startX, i, endX, i);
    }
    for (int i = 0; i > startY; i -= step) {
        gLine(startX, i, endX, i);
    }

    //Вертикальные линии
    for (int i = 0; i < endX; i += step) {
        gLine(i, startY, i, endY);
    }
    for (int i = 0; i > startX; i -= step) {
        gLine(i, startY, i, endY);
    }

    //Оси
    painter.setPen(QPen(Qt::gray, 2, Qt::SolidLine));
    //x
    painter.drawLine(0, widgetHeight + shiftView.getY(), widgetWidth, widgetHeight + shiftView.getY());
    //y
    painter.drawLine(shiftView.getX(), 0, shiftView.getX(), widgetHeight);

    //Подписи
    /*int axisFontSize = 20;
    QFont axisFont;
    axisFont.setPixelSize(axisFontSize);

    painter.setFont(axisFont);

    //Центр координат
    painter.drawText(QRect(shiftView.getX() - axisFontSize - 4, widgetHeight + shiftView.getY(),
                           axisFontSize, axisFontSize + 4), Qt::AlignRight, "O");

    //Ось x
    if (shiftView.getX() < widgetWidth - axisFontSize) {
        painter.drawText(QRect(widgetWidth - axisFontSize - 4, widgetHeight + shiftView.getY(),
                               axisFontSize, axisFontSize + 4), Qt::AlignRight, "x");
    } else {
        painter.drawText(QRect(shiftView.getX() - 4, widgetHeight + shiftView.getY(),
                               axisFontSize, axisFontSize + 4), Qt::AlignRight, "x");
    }

    //Ось y
    if (shiftView.getY() > -widgetHeight + axisFontSize + 6) {
        painter.drawText(QRect(shiftView.getX() - axisFontSize - 4, -2, axisFontSize,
                               axisFontSize + 4), Qt::AlignRight, "y");
    } else {
        painter.drawText(QRect(shiftView.getX() - axisFontSize - 4, widgetHeight + shiftView.getY() - axisFontSize - 8,
                               axisFontSize, axisFontSize + 4), Qt::AlignRight, "y");
    }

    //Размеры клетки
    axisFont.setPixelSize(axisFontSize - 6);
    painter.setFont(axisFont);
    painter.setPen(QPen(QColor(225, 225, 225), 1, Qt::SolidLine));

    gText(Point(step, (-axisFontSize + 4) / gScale), QString::number(step));
    gText(Point((-axisFontSize + 4) / gScale, step), QString::number(step));*/
}

QPoint GLWidget::gQPoint(Point a) {
    return QPoint(a.getX() * gScale + shiftView.getX(),
                  widgetHeight - a.getY() * gScale + shiftView.getY());
}

void GLWidget::printInfo() {
    int w = 85;
    int h = 16;
    int t = 6;
    int l = widgetWidth - w;

    QRect rect = QRect(l - 6, 0, w + 6, h * 5 + 8);
    painter.fillRect(rect, QBrush(QColor(255, 255, 255, 200)));
    //painter.fillRect(rect, QBrush(Qt::cyan));

    painter.setPen(textPen);
    painter.setFont(textFont);

    //Вывод текущего режима
    QString modeName = "Mode: Draw";
    if (mode == Draw) {
        modeName = "Mode: Draw";
    } else if (mode == Edit) {
        modeName = "Mode: Edit";
    } else if (mode == Demo) {
        modeName = "Mode: Demo";
    } else if (mode == Demo2) {
        modeName = "Mode: Demo2";
    } else if (mode == Demo3) {
        modeName = "Mode: Demo3";
    }

    painter.drawText(QRect(l, t + h * 0, w, h), Qt::AlignLeft, modeName);

    //Выводим координаты
    QString str_x;
    str_x.sprintf("x: %.2f", gPos.getX());
    QString str_y;
    str_y.sprintf("y: %.2f", gPos.getY());

    painter.drawText(QRect(l, t + h * 1, w, h), Qt::AlignLeft, str_x);
    painter.drawText(QRect(l, t + h * 2, w, h), Qt::AlignLeft, str_y);

    //Вывод масштаба
    QString str_scale = "Scale: " + QString().sprintf("%.1f", gScale * 100) + "%";
    painter.drawText(QRect(l, t + h * 3, w, h), Qt::AlignLeft, str_scale);

    //Текущий кадр
    painter.drawText(QRect(l, t + h * 4, w, h), Qt::AlignLeft,
                     "Timer: " + QString::number(elapsed));
}


//
void GLWidget::mouseMoveEvent(QMouseEvent *me) {
    mPos = me->posF();

    if (move) {
        if (wheel) {
            shiftView = shiftBeforMove + mPos - startMovePos;
        } else {
            setCursor(Qt::OpenHandCursor);
            gPos = toGPoint(Point::toPoint(me->pos()));
        }
    } else {
        gPos = toGPoint(Point::toPoint(me->pos()));

        if (mode == Draw) {
            if (setOfPolygons[activePolygon].isPolygon()) {
                if (setOfPolygons[activePolygon][0].dist(gPos) < activationRadius / gScale) {
                    activePoint = 0;

                    //Примагничиваем указатель к вершине (фиктивное положение)
                    gPos = (setOfPolygons[activePolygon][0]);
                } else {
                    activePoint = -1;
                }
            }
        } else if (mode == Edit) {
            //Проверить нет ли точек поблизоти курсора и выделить ближайшую точку
            if (activePoint == -1) {
                for (int i = 0; i < setOfPolygons[activePolygon].count(); i++) {
                    if (setOfPolygons[activePolygon][i].dist(gPos) < activationRadius / gScale) {
                        activePoint = i;
                        setCursor(Qt::OpenHandCursor);
                        break;
                    }
                }
            }

            //Если зажата левая кнопка - переместить вершину
            if (leftBtn && activePoint != -1) {
                setOfPolygons[activePolygon][activePoint] = gPos;
            }

            //Если мышь покинула облась активной точки — снимаем выделение
            if (activePoint != -1 && setOfPolygons[activePolygon][activePoint].dist(gPos) > activationRadius / gScale) {
                activePoint = -1;
                setCursor(Qt::ArrowCursor);
            }
        }
    }
}

//Нажатие кнопки мыши
void GLWidget::mousePressEvent(QMouseEvent *me) {
    if(me->button() == Qt::LeftButton) {
        leftBtn = true;

        if (mode == Draw) {
            setOfPolygons[activePolygon].addPoint(gPos);

            if (!setOfPolygons[activePolygon].isConvex() && setOfPolygons[activePolygon].isPolygon()) {
                setOfPolygons[activePolygon].removeLast();
            } else if (activePoint == 0) {
                setOfPolygons[activePolygon].removeLast();
                setMode(Edit);
            }
        }

        if (mode == Edit) {
            if (activePoint != -1) {
                setCursor(Qt::ClosedHandCursor);
            }
        }
    } else if (me->button() == Qt::MiddleButton && !wheel) {
        wheel = true;
        setMode(Move);
        shiftBeforMove = shiftView;
        startMovePos = me->posF();
    } else if(me->button() == Qt::RightButton) {
        rightBtn = true;

        if (mode == Draw) {
            if (setOfPolygons[activePolygon].count() > 0) {
                setOfPolygons[activePolygon].removeLast();
            }

            if (!setOfPolygons[activePolygon].isPolygon()) {
                activePoint = -1;
            }
        }
    }
}

//Отжатие кнопки мыши
void GLWidget::mouseReleaseEvent(QMouseEvent *me) {
    if(leftBtn && me->button() == Qt::LeftButton) {
        leftBtn = false;
        if (activePoint != -1) {
            setCursor(Qt::OpenHandCursor);
        }
    } else if (wheel && me->button() == Qt::MiddleButton) {
        wheel = false;
        setMode(Move);
    } else if (rightBtn && me->button() == Qt::RightButton) {
        rightBtn = false;
    }
}

//Масштабирование
void GLWidget::wheelEvent(QWheelEvent *me) {
    if (!move) {
        gScale *= 1 + me->delta() / 1200.0;

        //Ограничения масштабирования
        if (gScale > maxScale) {
            gScale = maxScale;
        } else if (gScale < minScale) {
            gScale = minScale;
        } else if (gScale > 0.9 && gScale < 1.1) {
            gScale = 1.0;
        }

        //Вычисляем сдвиг
        shiftView = Point((gPos.getX() * gScale - me->x()) * -1,
                           gPos.getY() * gScale - widgetHeight + me->y());
        gPos = toGPoint(Point::toPoint(me->pos()));
    }
}

Point GLWidget::toMPoint(Point p) {
    return Point(p.getX() * gScale + shiftView.getX(),
                -p.getY() * gScale + shiftView.getY() + widgetHeight);
}

Point GLWidget::toGPoint(Point p) {
    return Point((p.getX() - shiftView.getX()) / gScale,
                 (widgetHeight - p.getY() + shiftView.getY()) / gScale);
}

Point GLWidget::toGPoint(QPoint p) {
    toGPoint(Point::toPoint(p));
}
