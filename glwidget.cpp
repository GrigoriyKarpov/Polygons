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

    gX = 0;
    gY = 0;
    cbx = 0;
    cby = 0;
    ccx = 0;
    ccy = 0;

    active = 8;         //По умолчанию = 8
    activePoint = -1;

    move = false;
    crossroad = false;
    leftBtn = false;
    mode = Demo;

    textPen = QPen(QColor(96, 96, 96));
    textFont.setPixelSize(12);
    polygonPen = QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap);
    circlePen = QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap);
    crossroadPen = QPen(Qt::red, 2, Qt::DotLine, Qt::RoundCap);
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(0, 0, width, height, QBrush(Qt::white));
    //painter.translate(300, 300);

    painter.save();

    if (mode == Demo) {
        //--------------------- Инициализация переменных ---------------------//
        Point  a = Point(650.0, 100.0);
        Point  b = Point(150.0, 150.0);
        Point  c = Point(450.0, 550.0);
        double h = 100.0;                              //Длина сользящего отрезка
        double d1 = 50.0;                              //Раст от верш. скользящего отрезка до основания перпендикуляра
        double d2 = 300.0;                             //Длина перпендикуляра
        double angle = (M_PI * elapsed / 180.0) * 0.7; //Отрезок вращается с анимацией

        QVector<Point> tmp = Tools::slipInAngle(a, b, c, h, angle, d1, d2);

        Point r = tmp[0];  //Скользит по [b,a]
        Point q = tmp[1];  //Скользит по [b,c]
        Point t = tmp[2];  //Третья вершина
        path.push_back(t); //Траектория

        //Точки пересечения траектории и угла
        int acc = 5; //Точность
        if (inter.count() < 2 && path.count() > acc) {
            int k = path.count() - 1;
            Point res;

            if (inter.count() == 0 && Tools::intersection(b, c, path[k], path[k - acc], &res)) {
                inter.push_back(res);
            }
            if (Tools::intersection(b, a, path[k], path[k - acc], &res)) {
                inter.push_back(res);
            }
        }

        //Сброс анимации при
        if (angle > M_PI - qAcos(Tools::cos(a, b, c))) {
            elapsed = 0;
            path.clear();
            inter.clear();
        }

        //--------------------- Визуализация ---------------------//
        //Угол
        painter.setPen(polygonPen);
        painter.drawLine(a.getX() + gX, height - a.getY() + gY,
                         b.getX() + gX, height - b.getY() + gY);
        painter.drawLine(b.getX() + gX, height - b.getY() + gY,
                         c.getX() + gX, height - c.getY() + gY);

        //Отрезок
        painter.setPen(QPen(Qt::red, 2, Qt::SolidLine));
        painter.drawLine(q.getX() + gX, height - q.getY() + gY,
                         r.getX() + gX, height - r.getY() + gY);

        //Отрезки до третьей вершины
        painter.setPen(QPen(Qt::red, 2, Qt::DotLine));
        painter.drawLine(q.getX() + gX, height - q.getY() + gY,
                         t.getX() + gX, height - t.getY() + gY);
        painter.drawLine(r.getX() + gX, height - r.getY() + gY,
                         t.getX() + gX, height - t.getY() + gY);

        //Траектория
        if (path.count() > 1) {
            painter.setPen(QPen(Qt::gray, 2, Qt::SolidLine));
            for (int i = 0; i < path.count() - 1; i++) {
                painter.drawLine(path[i].getX() + gX, height - path[i].getY() + gY,
                                 path[i + 1].getX() + gX, height - path[i + 1].getY() + gY);
            }
        }

        //Вершины...
        painter.setPen(QPen(Qt::black, 8, Qt::SolidLine, Qt::RoundCap));

        //...угла
        painter.drawPoint(a.getX() + gX, height - a.getY() + gY);
        painter.drawPoint(b.getX() + gX, height - b.getY() + gY);
        painter.drawPoint(c.getX() + gX, height - c.getY() + gY);

        //...отрезка
        painter.drawPoint(r.getX() + gX, height - r.getY() + gY);
        painter.drawPoint(q.getX() + gX, height - q.getY() + gY);

        //Третья точка
        painter.drawPoint(t.getX() + gX, height - t.getY() + gY);

        //Точки пересечения траектории
        if (inter.count() > 0) {
            painter.setPen(QPen(Qt::red, 12, Qt::SolidLine, Qt::RoundCap));
            for (int i = 0; i < inter.count(); i++) {
                painter.drawPoint(inter[i].getX() + gX, height - inter[i].getY() + gY);
            }
        }

        //Текст
        painter.setPen(textPen);
        painter.setFont(textFont);
        //Текущий кадр
        painter.drawText(QRect(width / 2 - 50,  2, 100, 16), Qt::AlignLeft,
                         "time: " + QString::number(elapsed));
    }

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

    //Выводим координаты
    painter.setPen(textPen);
    painter.setFont(textFont);
    painter.drawText(QRect(width - 38,  2, 38, 16), Qt::AlignLeft,
                     "x: " + QString::number(cax));
    painter.drawText(QRect(width - 38, 18, 38, 16), Qt::AlignLeft,
                     "y: " + QString::number(height - cay));

    //Вывод текущего режима
    QString modeName = "Mode: Draw";
    if (mode == Draw) {
        modeName = "Mode: Draw";
    } else if (mode == Edit) {
        modeName = "Mode: Edit";
    } else if (mode == Demo) {
        modeName = "Mode: Demo";
    } if (move) {
        modeName = modeName + "/Move";
    }

    painter.drawText(QRect(4, 2, 150, 16), Qt::AlignLeft, modeName);

    //Вывод ошибки
    if (crossroad) {
        QString errorMessage = "Warning: lines must not intersect!";
        painter.setPen(QPen(QColor(255, 64, 64)));
        painter.drawText(QRect(4, height - 16, 400, 16), Qt::AlignLeft, errorMessage);
    }

    painter.save();
    painter.end();

    update();

}

//Анимация
void GLWidget::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000; //Каждые 300 у.е. происходит сброс таймера на 0 и он работает в цикле
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
            setCursor(Qt::OpenHandCursor);
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

void GLWidget::mouseMoveEvent(QMouseEvent *me) {
    QVector<int> tmp;

    if (move) {
        if (leftBtn) {
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
    }

    update();
}

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
    }

    update();
}
