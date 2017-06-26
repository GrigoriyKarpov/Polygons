#include <QtGui>

#include "mainwindow.h"
#include "mainwidget.h"

MainWindow::MainWindow() {
    setWindowTitle(trUtf8("Редактор многоугольников"));
    //setFixedSize(0, 0);
    /*setWindowFlags(Qt::Widget
                   | Qt::WindowMinimizeButtonHint
                   | Qt::WindowCloseButtonHint
                   | Qt::WindowTitleHint);*/

    mainWidget = new MainWidget;
    setCentralWidget(mainWidget);

    createActions();
    createMenus();
}

void MainWindow::about() {
    QMessageBox::information(this, trUtf8("О программе Редактор многоугольников"),
                      trUtf8("Редактор многоугольников 1.0.\n\n"
                      "© 2017 Институт Вычислительной Математики и Математической Геофизики"));
}

void MainWindow::createActions() {
    exitAct = new QAction(trUtf8("В&ыход"), this);
    exitAct->setShortcut(trUtf8("Ctrl+Q", "Выход"));

    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(trUtf8("&О программе"), this);
    aboutAct->setShortcut(Qt::Key_F1);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(trUtf8("О программе &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus() {
    QMenu *fileMenu = menuBar()->addMenu(trUtf8("&Файл"));
    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

    QMenu *helpMenu = menuBar()->addMenu(trUtf8("&Помощь"));
    helpMenu->addAction(aboutAct);
    //helpMenu->addAction(aboutQtAct);
}

void MainWindow::moveToCenter() {
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
}
