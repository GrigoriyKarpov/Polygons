#include <QtGui>
#include "glwidget.h"

#include <QPainter>
#include <QtCore/qmath.h>
#include <algorithm>
#include <QCursor>

GLWidget::GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    //Настройки виджета
    height = 300;
    width = 400;

    setFixedSize(width, height);
    setMouseTracking(true);
    setCursor(Qt::CrossCursor);

    //Инициализация переменных
    cax = 0;
    cay = 0;

    active = 8;         //По умолчанию = 8
    activePoint = -1;

    crossroad = false;
    leftBtn = false;
    mode = Draw;

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

    painter.fillRect(0, 0, 400, 300, QBrush(Qt::white));
    //painter.translate(300, 300);

    painter.save();


    //Рисуем полигон
    if (points.count() > 0) {
        QPen pointPen = polygonPen;
        pointPen.setWidth(polygonPen.width() + 3);

        painter.setPen(pointPen);
        painter.drawPoint(points[0][0], height - points[0][1]);
        painter.setPen(polygonPen);

        if (points.count() > 1) {
            for (int i = 0; i < points.count() - 1; i++) {
                painter.drawLine(points[i][0], height - points[i][1],
                                 points[i + 1][0], height - points[i + 1][1]);

                painter.setPen(pointPen);
                painter.drawPoint(points[i + 1][0], height - points[i + 1][1]);
                painter.setPen(polygonPen);
            }
        }

        //Как рисовать последнию линию (в начало или к курсору) — зависит от текщего режима
        if (mode == Edit) {
            painter.drawLine(points[0][0], height - points[0][1],
                             points[points.count() - 1][0],
                    height - points[points.count() - 1][1]);
        } else {
            if (crossroad) {
                painter.setPen(crossroadPen);
            } else {
                painter.setPen(polygonPen);
            }

            painter.drawLine(points[points.count() - 1][0],
                    height - points[points.count() - 1][1], cax, height - cay);
        }

        //Нарисовать выделение вокруг активной точки
        if (activePoint != -1) {

            painter.setPen(circlePen);
            QVector<int> tmp3 = points.value(activePoint);

            painter.drawEllipse(tmp3.value(0) - active,
                                (height - tmp3.value(1)) - active,
                                active * 2, active * 2);
        }
    }

    //Выводим координаты
    painter.setPen(textPen);
    painter.setFont(textFont);
    painter.drawText(QRect(362,  2, 38, 16), Qt::AlignLeft,
                     "x: " + QString::number(cax));
    painter.drawText(QRect(362, 18, 38, 16), Qt::AlignLeft,
                     "y: " + QString::number(cay));

    //Вывод текущего режима
    QString modeName = "Mode: Draw";
    if (mode == Edit) {
        modeName = "Mode: Edit";
    }
    painter.drawText(QRect(4, 2, 100, 16), Qt::AlignLeft, modeName);

    //Вывод ошибки
    if (crossroad) {
        QString errorLine = "Error: lines must not intersect";

        painter.setPen(QPen(QColor(255, 64, 64)));
        painter.drawText(QRect(4, 284, 400, 16), Qt::AlignLeft, errorLine);
    }

    painter.save();
    painter.end();

    update();

}

void GLWidget::setMode(GLWidget::Modes mode) {
    if (mode == Draw) {
        this->mode = mode;
        crossroad = false;
        activePoint = -1;
        points.clear();
        setCursor(Qt::CrossCursor);
    } else {
        this->mode = mode;
        if (isCrossroad()) {
            crossroad = true;
        } else {
            crossroad = false;
        }
        setCursor(Qt::ArrowCursor);
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
                    y22 = cay;
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
    tmp = points.value(0);

    if (mode == Draw) {
        if (points.count() > 2) {
            if (qPow(tmp.value(0) - me->x(), 2) +
                    qPow(tmp.value(1) - (height - me->y()), 2) < qPow(active, 2)) {
                activePoint = 0;

                cax = tmp.value(0);
                cay = tmp.value(1);
            } else {
                activePoint = -1;

                cax = me->x();
                cay = height - me->y();
            }

            //Проверка на пересечение
            if (isCrossroad() && mode == Draw) {
                crossroad = true;
                setCursor(Qt::ForbiddenCursor);
            } else {
                crossroad = false;
                setCursor(Qt::CrossCursor);
            }
        } else {
            cax = me->x();
            cay = height - me->y();
        }
    } else {
        cax = me->x();
        cay = height - me->y();

        //Проверить нет ли точек поблизоти курсора и выделить ближайшую точку
        for (int i = 0; i < points.count(); i++) {
            QVector<int> tmp = points.value(i);

            if (qPow(tmp.value(0) - cax, 2) +
                    qPow(tmp.value(1) - cay, 2) < qPow(active, 2)) {
                if (activePoint == -1) {
                    activePoint = i;
                } else if (!leftBtn) {
                    QVector<int> tmp2 = points.value(activePoint);

                    if (qSqrt(qPow(tmp.value(0) - cax, 2) +
                                 qPow(tmp.value(1) - cay, 2)) <
                           qSqrt(qPow(tmp2.value(0) - cax, 2) +
                                 qPow(tmp2.value(1) - cay, 2))) {
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
            points[activePoint][1] = cay;
        }

        //Если мышь покинула облась активной точки — снимаем выделение
        if (activePoint != -1 && !leftBtn) {
            QVector<int> tmp = points.value(activePoint);
            if (qPow(tmp.value(0) - cax, 2) +
                    qPow(tmp.value(1) - cay, 2) > qPow(active, 2)) {
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

    update();
}

void GLWidget::mousePressEvent(QMouseEvent *me) {
    if(me->button() == Qt::LeftButton) {
        leftBtn = true;

        if (mode == Draw) {
            if (!crossroad) {
                if (activePoint == 0) {
                    mode = Edit;
                } else {
                    QVector<int> tmp;
                    tmp.push_back(cax);
                    tmp.push_back(cay);
                    points.push_back(tmp);
                }
            }
        } else {
            if (activePoint != -1) {
                setCursor(Qt::ClosedHandCursor);
            }
        }
    }

    update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *me)
{
    if(leftBtn && me->button() == Qt::LeftButton){
        leftBtn = false;
        if (activePoint != -1 && mode == Edit) {
            setCursor(Qt::OpenHandCursor);
        }
    }

    update();
}
