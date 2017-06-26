#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE
class MainWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    void moveToCenter();

private slots:
    void about();

private:
    void createActions();
    void createMenus();

    MainWidget *mainWidget;
    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
};

#endif
