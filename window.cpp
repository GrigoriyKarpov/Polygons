#include <QtGui>
#include "window.h"

Window::Window() : QWidget() {
    setWindowTitle("Polygon editor");
    setFixedSize(0, 0);
    setWindowFlags(Qt::Widget
                   | Qt::WindowMinimizeButtonHint
                   | Qt::WindowCloseButtonHint
                   | Qt::WindowTitleHint);

    widgetOpenGL = new GLWidget(this);

    drawBtn = new QPushButton("Draw");
    drawBtn->setCheckable(true);
    editBtn = new QPushButton("Edit");
    editBtn->setCheckable(true);
    demoBtn = new QPushButton("Demo");
    demoBtn->setCheckable(true);
    moveBtn = new QPushButton("Move");
    moveBtn->setCheckable(true);
    saveBtn = new QPushButton("Save");
    optionBtn = new QPushButton("Options");
    optionBtn->setCheckable(true);

    btnsLayout = new QHBoxLayout;
    btnsLayout->addWidget(drawBtn);
    btnsLayout->addWidget(editBtn);
    btnsLayout->addWidget(demoBtn);
    btnsLayout->addWidget(moveBtn);
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
    connect(moveBtn, SIGNAL(clicked()), this, SLOT(moveBtnClick()));
    connect(demoBtn, SIGNAL(clicked()), this, SLOT(demoBtnClick()));

    //
    connect(saveBtn, SIGNAL(clicked()), this, SLOT(saveBtnClick()));


    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), widgetOpenGL, SLOT(animate()));
    timer->start(1);
}

void Window::optionBtnClick() {
    dialog->show();
    emit sendOptions(widgetOpenGL->getPolygonPen());
}


void Window::drawBtnClick() {
    widgetOpenGL->setMode(GLWidget::Draw);
    editBtn->setChecked(false);
    moveBtn->setChecked(false);
    demoBtn->setChecked(false);
}

void Window::editBtnClick() {
    widgetOpenGL->setMode(GLWidget::Edit);
    drawBtn->setChecked(false);
    moveBtn->setChecked(false);
    demoBtn->setChecked(false);
}

void Window::saveBtnClick() {
}

void Window::moveBtnClick()
{
    widgetOpenGL->setMode(GLWidget::Move);
}

void Window::demoBtnClick()
{
    widgetOpenGL->setMode(GLWidget::Demo);
    drawBtn->setChecked(false);
    editBtn->setChecked(false);
    moveBtn->setChecked(false);
}

void Window::moveToCenter() {
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
}
