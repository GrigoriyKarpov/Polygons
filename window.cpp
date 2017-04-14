#include <QtGui>
#include "window.h"

Window::Window() : QWidget()
{
    setWindowTitle("Polygon editor");
    setFixedSize(0, 0);
    setWindowFlags(Qt::Widget
                   | Qt::WindowMinimizeButtonHint
                   | Qt::WindowCloseButtonHint
                   | Qt::WindowTitleHint);

    widgetOpenGL = new GLWidget(this);

    drawBtn = new QPushButton("Draw");
    editBtn = new QPushButton("Edit");
    saveBtn = new QPushButton("Save");
    optionBtn = new QPushButton("Options");

    btnsLayout = new QHBoxLayout;
    btnsLayout->addWidget(drawBtn);
    btnsLayout->addWidget(editBtn);
    btnsLayout->addWidget(saveBtn);
    btnsLayout->addWidget(optionBtn);

    mainlayout = new QVBoxLayout;
    mainlayout->addLayout(btnsLayout);
    mainlayout->addWidget(widgetOpenGL);

    setLayout(mainlayout);

    dialog = new Dialog();

    //Реализация передачи данных между окном опций и виджетом OpenGL
    connect(optionBtn, SIGNAL(clicked()), this, SLOT(optionBtnClick()));
    connect(this, SIGNAL(sendOptions(QPen)), dialog, SLOT(recieveOptions(QPen)));
    connect(dialog, SIGNAL(sendOptions(QPen)), widgetOpenGL, SLOT(recieveOptions(QPen)));

    //Переключение режимов
    connect(drawBtn, SIGNAL(clicked()), this, SLOT(drawBtnClick()));
    connect(editBtn, SIGNAL(clicked()), this, SLOT(editBtnClick()));

    //
    connect(saveBtn, SIGNAL(clicked()), this, SLOT(saveBtnClick()));
}

void Window::optionBtnClick() {
    dialog->show();
    emit sendOptions(widgetOpenGL->getPolygonPen());
}


void Window::drawBtnClick() {
    widgetOpenGL->setMode(GLWidget::Draw);
}

void Window::editBtnClick() {
    widgetOpenGL->setMode(GLWidget::Edit);
}

void Window::saveBtnClick() {
}
