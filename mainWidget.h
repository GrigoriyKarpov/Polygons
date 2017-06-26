#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "glwidget.h"

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPen>
#include <QListWidget>
#include <QLabel>
#include <QCheckBox>
#include <QAction>
#include <QMenu>
#include <QGroupBox>
#include <QRadioButton>

class MainWidget : public QWidget {
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);

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

    void moveRadioClick();
    void rotateRadioClick();

    void geometrySearchCBClick();

    void saveToFile();
    void loadFromFile();

private:
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

    QPushButton *saveBtn;
    QPushButton *openBtn;

    QCheckBox *inCharCB;
    QCheckBox *outCharCB;
    QCheckBox *geometrySearchCB;

    bool editBtnCheck;

    QGroupBox    *moveOrRotateGB;
    QRadioButton *moveRadio;
    QRadioButton *rotateRadio;

    QHBoxLayout *saveBtnsLayout;
    QHBoxLayout *polygonBtnsLayout;
    QHBoxLayout *widgetsLayout;
    QVBoxLayout *demoBtnsLayout;
    QVBoxLayout *optionsLayout;
    QVBoxLayout *mainlayout;
    QVBoxLayout *moveOrRotateLayout;
};

#endif
