#ifndef WINDOW_H
#define WINDOW_H

#include "glwidget.h"
#include "dialog.h"

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPen>

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
    void moveToCenter();

signals:
    void sendOptions(QPen pen);
    void selectMode(GLWidget::Modes mode);

public slots:
    void optionBtnClick();
    void drawBtnClick();
    void editBtnClick();
    void saveBtnClick();
    void moveBtnClick();
    void demoBtnClick();

private:
    Dialog *dialog;
    GLWidget *widgetOpenGL;

    QPushButton *drawBtn;
    QPushButton *editBtn;
    QPushButton *saveBtn;
    QPushButton *optionBtn;
    QPushButton *moveBtn;
    QPushButton *demoBtn;

    QHBoxLayout *btnsLayout;
    QVBoxLayout *mainlayout;
};

#endif
