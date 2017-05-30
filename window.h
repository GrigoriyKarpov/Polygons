#ifndef WINDOW_H
#define WINDOW_H

#include "glwidget.h"
#include "dialog.h"

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPen>
#include <QListWidget>
#include <QLabel>
#include <QCheckBox>

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
    void endDraw();

    void addBtnClick();
    void editBtnClick();
    void removeBtnClick();
    void viewBtnClick();
    void inCharBtnClick();
    void outCharBtnClick();

    void inCharCBClick();
    void outCharCBClick();
    void geometrySearchCBClick();

private:
    Dialog *dialog;
    GLWidget *widgetOpenGL;
    QListWidget *listOfPolygons;

    QLabel *lblList;
    QLabel *lblMode;

    QPushButton *addBtn;
    QPushButton *editBtn;
    QPushButton *removeBtn;
    QPushButton *viewBtn;
    QPushButton *inCharBtn;
    QPushButton *outCharBtn;

    QCheckBox *inCharCB;
    QCheckBox *outCharCB;
    QCheckBox *geometrySearchCB;

    bool editBtnCheck;

    QHBoxLayout *polygonBtnsLayout;
    QHBoxLayout *widgetsLayout;
    QVBoxLayout *demoBtnsLayout;
    QVBoxLayout *optionsLayout;
    QVBoxLayout *mainlayout;
};

#endif
