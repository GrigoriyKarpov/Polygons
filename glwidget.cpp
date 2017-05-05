#include <QtGui>
#include "glwidget.h"
#include "tools.h"

#include <QPainter>
#include <QtCore/qmath.h>
#include <algorithm>
#include <QCursor>
#include <QDebug>

GLWidget::GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    //Настройки виджета
    height = 600;
    width = 800;

    setFixedSize(width, height);
    setMouseTracking(true);

    //Инициализация переменных
    cax = 0;
    cay = 0;

    gX = width / 2;
    gY = - height / 2;
    cbx = 0;
    cby = 0;
    ccx = width / 2;
    ccy = - height / 2;

    gScale = 1.0;

    ii = 0;
    ij = 1;
    ik = 2;

    active = 8;         //По умолчанию = 8
    activePoint = -1;

    inter_f = true;
    slip = false;
    axis = true;
    move = false;
    crossroad = false;
    leftBtn = false;
    wheel = false;
    mode = Demo;

    textPen = QPen(QColor(96, 96, 96));
    textFont.setPixelSize(12);
    polygonPen = QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap);
    circlePen = QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap);
    crossroadPen = QPen(Qt::red, 2, Qt::DotLine, Qt::RoundCap);
}


void GLWidget::paintEvent(QPaintEvent *event) {
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(0, 0, width, height, QBrush(Qt::white));
    //painter.translate(300, 300);

    //painter.save();

    if (axis) {
        int startX =  -gX / gScale;
        int endX   = (-gX + width) / gScale;
        int startY =   gY / gScale;
        int endY   =  (gY + height) / gScale;
        int step   =   50;

        while (qFabs(endX - startX) / step > 50) {
            step *= 2;
        }
        while (qFabs(endX - startX) / step < 4) {
            step /= 4;
        }

        //Сетка
        painter.setPen(QPen(QColor(225, 225, 225), 1, Qt::SolidLine));
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
        gLine(startX, 0, endX, 0);
        gLine(0, startY, 0, endY);
    }

    if (mode == Demo) {
        //--------------------- Инициализация переменных ---------------------//
        QVector<Point> polygon;
        polygon.push_back(Point(200.0, -250.0));
        polygon.push_back(Point(-150.0, -150.0));
        polygon.push_back(Point(50.0, 130.0));
        polygon.push_back(Point(200.0, 0.0));

        double h  = 100.0;   //Длина сользящего отрезка
        double d1 = 0.0;     //Раст от верш. скользящего отрезка до основания перпендикуляра
        double d2 = 300.0;   //Длина перпендикуляра
        double speed = 5.0;  //Коэффициент скорости анимации

        Point r;
        Point q;
        Point t;

        double angle = 0;

        if (inter_f) {
            //Выбор между скольжением в угле и скольжением по грани
            if (!slip) {
                angle = M_PI * elapsed / 180.0 * speed;  //Отрезок вращается с анимацией

                QVector<Point> tmp = Tools::slipInAngle(polygon[ii], polygon[ij], polygon[ik], h, angle, d1, d2);

                r = tmp[0];  //Скользит по [b,a]
                q = tmp[1];  //Скользит по [b,c]
                t = tmp[2];  //Третья вершина

                //Переход к скольжению по грани ij, ik
                if (angle > M_PI - qAcos(Tools::cos(polygon[ii], polygon[ij], polygon[ik]))) {
                    slip    = true;
                    elapsed = 0;
                } else {
                    path.push_back(t);
                }
            } else {
                //Скользим по грани ij, ik
                Point s;

                s.setX(((polygon[ik].getX() - polygon[ij].getX()) / 100.0) * elapsed * speed);
                s.setY(((polygon[ik].getY() - polygon[ij].getY()) / 100.0) * elapsed * speed);

                angle = M_PI - qAcos(Tools::cos(polygon[ii], polygon[ij], polygon[ik]));
                QVector<Point> tmp = Tools::slipInAngle(polygon[ii], polygon[ij], polygon[ik], h, angle, d1, d2);

                for (int i = 0; i < 3; i++) {
                    tmp[i].shift(s);
                }

                r = tmp[0];  //Скользит по [b,a]
                q = tmp[1];  //Скользит по [b,c]
                t = tmp[2];  //Третья вершина

                //Если достигли вершины ik - переход на следующий угол
                if (Tools::dist(polygon[ij], polygon[ik]) <= Tools::dist(polygon[ij], q)) {
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
                        //inter.clear();
                        //path.clear();
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

                    if (Tools::intersection(polygon[i], polygon[j], path[k - 1], path[k], &res)) {
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

        //Траектория
        if (path.count() > 1) {
            painter.setPen(QPen(Qt::gray, 2, Qt::SolidLine));
            for (int i = 0; i < path.count() - 1; i++) {
                gLine(path[i], path[i + 1]);
            }
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

    //Рисуем полигон
    if (points.count() > 0) {
        QPen pointPen = polygonPen;
        pointPen.setWidth(polygonPen.width() + 3);

        painter.setPen(pointPen);
        painter.drawPoint(points[0][0] + gX, height - points[0][1] + gY);
        painter.setPen(polygonPen);

        if (points.count() > 1) {
            for (int i = 0; i < points.count() - 1; i++) {
                painter.drawLine(points[i][0] + gX, height - points[i][1] + gY,
                                 points[i + 1][0] + gX, height - points[i + 1][1] + gY);

                painter.setPen(pointPen);
                painter.drawPoint(points[i + 1][0] + gX, height - points[i + 1][1] + gY);
                painter.setPen(polygonPen);
            }
        }

        //Как рисовать последнию линию (в начало или к курсору) — зависит от текщего режима
        if (mode == Edit) {
            painter.drawLine(points[0][0] + gX,
                             height - points[0][1] + gY,
                             points[points.count() - 1][0] + gX,
                             height - points[points.count() - 1][1] + gY);
        } else if (mode == Draw) {
            if (crossroad) {
                painter.setPen(crossroadPen);
            } else {
                painter.setPen(polygonPen);
            }

            painter.drawLine(points[points.count() - 1][0] + gX,
                    height - points[points.count() - 1][1] + gY,
                    cax + gX,
                    cay + gY);
        }

        //Нарисовать выделение вокруг активной точки
        if (activePoint != -1) {

            painter.setPen(circlePen);
            QVector<int> tmp3 = points.value(activePoint);

            painter.drawEllipse(tmp3.value(0) - active + gX,
                                (height - tmp3.value(1)) - active + gY,
                                active * 2, active * 2);
        }
    }

    //Вывод блока с информацией
    printInfo();

    painter.setPen(textPen);
    painter.setFont(textFont);

    //Лог
    if (log.count() > 0) {
        for (int i = 0; i < log.count(); i++) {
            painter.drawText(QRect(width / 2 - 200,  2 + 16 * i, 400, 16), Qt::AlignLeft, log[i]);
        }
        log.clear();
    }

    //Вывод ошибки
    if (crossroad) {
        QString errorMessage = "Warning: lines must not intersect!";
        painter.setPen(QPen(QColor(255, 64, 64)));
        painter.drawText(QRect(4, height - 16, 400, 16), Qt::AlignLeft, errorMessage);
    }

    painter.save();
    painter.restore();
    painter.end();

    update();
}

//Анимация
void GLWidget::animate() {
    int limit = 10000; //Лимит кадров
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % limit;

    if (elapsed < 0) {
        elapsed = 0;
    }

    repaint();
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
        if (isCrossroad()) {
            crossroad = true;
        } else {
            crossroad = false;
        }
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
            } else if (this->mode == Edit) {
                setCursor(Qt::ArrowCursor);
            } else if (this->mode == Demo) {
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

//Проверка на пересечение (надо переделать)
bool GLWidget::isCrossroad() {
    int x11, y11, x12, y12, x21, y21, x22, y22;
    for (int i = 0; i < points.count() - 2; i++) {
        //Первый отрезок
        x11 = points[i][0];
        y11 = points[i][1];
        x12 = points[i + 1][0];
        y12 = points[i + 1][1];

        for (int j = i + 1; j < points.count(); j++) {
            //Второй отрезок
            if (j != points.count() - 1) {
                x21 = points[j][0];
                y21 = points[j][1];
                x22 = points[j + 1][0];
                y22 = points[j + 1][1];
            } else {
                x21 = points[j][0];
                y21 = points[j][1];

                if (mode == Edit) {
                    x22 = points[0][0];
                    y22 = points[0][1];
                } else {
                    x22 = cax;
                    y22 = (height - cay);
                }
            }

            //Проверяем пересечения
            int v1 =(x22-x21)*(y11-y21)-(y22-y21)*(x11-x21);
            int v2 =(x22-x21)*(y12-y21)-(y22-y21)*(x12-x21);
            int v3 =(x12-x11)*(y21-y11)-(y12-y11)*(x21-x11);
            int v4 =(x12-x11)*(y22-y11)-(y12-y11)*(x22-x11);

            if (v1 * v2 < 0 && v3 * v4 < 0) {
                return true;
            }
        }
    }

    return false;
}

void GLWidget::gLine(Point a, Point b) {
    gLine(a.getX(), a.getY(), b.getX(), b.getY());
}

void GLWidget::gLine(double a, double b, double c, double d) {
    painter.drawLine(a * gScale + gX,
                     height - b * gScale + gY,
                     c * gScale + gX,
                     height - d * gScale + gY);
}

void GLWidget::gPoint(Point a) {
    gPoint(a.getX(), a.getY());
}

void GLWidget::gPoint(double a, double b) {
    painter.drawPoint(a * gScale + gX,
                      height - b * gScale + gY);
}

void GLWidget::printInfo() {
    int w = 85;
    int h = 16;
    int t = 6;
    int l = width - w;

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
    }
    painter.drawText(QRect(l, t + h * 0, w, h), Qt::AlignLeft, modeName);

    //Выводим координаты
    QString str_x = "x: " + QString::number((int) (cax * 1.0 / gScale));
    QString str_y = "y: " + QString::number((int) ((height - cay) * 1.0 / gScale));

    painter.drawText(QRect(l, t + h * 1, w, h), Qt::AlignLeft, str_x);
    painter.drawText(QRect(l, t + h * 2, w, h), Qt::AlignLeft, str_y);

    //Вывод масштаба
    QString str_scale = "Scale: " + QString::number(gScale);
    painter.drawText(QRect(l, t + h * 3, w, h), Qt::AlignLeft, str_scale);

    //Текущий кадр
    painter.drawText(QRect(l, t + h * 4, w, h), Qt::AlignLeft,
                     "Time: " + QString::number(elapsed));
}


//
void GLWidget::mouseMoveEvent(QMouseEvent *me) {
    QVector<int> tmp;

    if (move) {
        if (leftBtn || wheel) {
            gX = me->x() - cbx + ccx;
            gY = me->y() - cby + ccy;
        } else {
            setCursor(Qt::OpenHandCursor);
            cax = me->x() - gX;
            cay = me->y() - gY;
        }
    } else {
        tmp = points.value(0);
        cax = me->x() - gX;
        cay = me->y() - gY;

        if (mode == Draw) {
            if (points.count() > 2) {
                if (qPow(tmp.value(0) - cax, 2) +
                    qPow(tmp.value(1) - (height - cay), 2) < qPow(active, 2)) {
                    activePoint = 0;

                    cax = tmp.value(0);
                    cay = height - tmp.value(1);
                } else {
                    activePoint = -1;
                }

                //Проверка на пересечение
                if (isCrossroad() && mode == Draw) {
                    crossroad = true;
                    setCursor(Qt::ForbiddenCursor);
                } else {
                    crossroad = false;
                    setCursor(Qt::CrossCursor);
                }
            }
        } else if (mode == Edit) {
            //Проверить нет ли точек поблизоти курсора и выделить ближайшую точку
            for (int i = 0; i < points.count(); i++) {
                QVector<int> tmp = points.value(i);

                if (qPow(tmp.value(0) - cax, 2) +
                    qPow(tmp.value(1) - (height - cay), 2) < qPow(active, 2)) {
                    if (activePoint == -1) {
                        activePoint = i;
                    } else if (!leftBtn) {
                        QVector<int> tmp2 = points.value(activePoint);

                        if (qSqrt(qPow(tmp.value(0) - cax, 2) +
                            qPow(tmp.value(1) - (height - cay), 2)) <
                            qSqrt(qPow(tmp2.value(0) - cax, 2) +
                            qPow(tmp2.value(1) - (height - cay), 2))) {
                            activePoint = i;
                        }
                    }

                    if (!leftBtn) {
                        setCursor(Qt::OpenHandCursor);
                    }
                }
            }

            if (leftBtn && activePoint != -1) {
                points[activePoint][0] = cax;
                points[activePoint][1] = height - cay;
            }

            //Если мышь покинула облась активной точки — снимаем выделение
            if (activePoint != -1 && !leftBtn) {
                QVector<int> tmp = points.value(activePoint);
                if (qPow(tmp.value(0) - cax, 2) +
                    qPow(tmp.value(1) - (height - cay), 2) > qPow(active, 2)) {
                    activePoint = -1;
                    setCursor(Qt::ArrowCursor);
                }
            }

            //Проверка на пересечения
            if (isCrossroad()) {
                crossroad = true;
                if (leftBtn && activePoint != -1) {
                    setCursor(Qt::ForbiddenCursor);
                }
            } else {
                crossroad = false;
                if (leftBtn && activePoint != -1) {
                    setCursor(Qt::ClosedHandCursor);
                }
            }
        }
    }

    update();
}

//
void GLWidget::mousePressEvent(QMouseEvent *me) {
    if(me->button() == Qt::LeftButton) {
        leftBtn = true;

        if (move) {
            cbx = me->x();
            cby = me->y();
            setCursor(Qt::ClosedHandCursor);
        } else {
            if (mode == Draw) {
                if (!crossroad) {
                    if (activePoint == 0) {
                        mode = Edit;
                    } else {
                        QVector<int> tmp;
                        tmp.push_back(cax);
                        tmp.push_back(height - cay);
                        points.push_back(tmp);
                    }
                }
            } else if (mode == Edit) {
                if (activePoint != -1) {
                    setCursor(Qt::ClosedHandCursor);
                }
            }
        }
    } else if (me->button() == Qt::MiddleButton) {
        wheel = true;
        setMode(Move);
        if (move) {
            cbx = me->x();
            cby = me->y();
        }
    }

    update();
}

//Отжатие кнопки мыши
void GLWidget::mouseReleaseEvent(QMouseEvent *me) {
    if(leftBtn && me->button() == Qt::LeftButton) {
        leftBtn = false;

        if (this->cursor().shape() == Qt::ClosedHandCursor) {
            setCursor(Qt::OpenHandCursor);
        }

        if (move) {
            ccx += me->x() - cbx;
            ccy += me->y() - cby;
        }
    } else if (wheel && me->button() == Qt::MiddleButton) {
        wheel = false;

        if (move) {
            ccx += me->x() - cbx;
            ccy += me->y() - cby;
        }

        setMode(Move);
    }

    update();
}

//Масштабирование
void GLWidget::wheelEvent(QWheelEvent *me) {
    double oldX = (me->x() - gX) * 1.0 / gScale;
    double oldY = (height - (me->y() - gY)) * 1.0 / gScale;

    double delta = me->delta();
    gScale *= 1 + delta / 600.0;

    if (gScale > 55.0) {
        gScale = 55.0;
    } else if (gScale < 0.0001) {
        gScale = 0.0001;
    } else if (gScale > 0.9 && gScale < 1.1) {
        gScale = 1.0;
    }

    //x
    double newX = (me->x() - gX) * 1.0 / gScale;
    if (newX < oldX) {
        while (newX < oldX) {
            gX--;
            newX = (me->x() - gX) * 1.0 / gScale;
        }
    } else if (newX > oldX) {
        while (newX > oldX) {
            gX++;
            newX = (me->x() - gX) * 1.0 / gScale;
        }
    }
    ccx = gX;

    //y
    double newY = (height - (me->y() - gY)) * 1.0 / gScale;
    if (newY < oldY) {
        while (newY < oldY) {
            gY++;
            newY = (height - (me->y() - gY)) * 1.0 / gScale;
        }
    } else if (newY > oldY) {
        while (newY > oldY) {
            gY--;
            newY = (height - (me->y() - gY)) * 1.0 / gScale;
        }
    }
    ccy = gY;

    update();
}
