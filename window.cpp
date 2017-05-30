#include <QtGui>
#include <QDesktopWidget>
#include "window.h"

Window::Window() : QWidget() {
    setWindowTitle("Polygon editor");
    setFixedSize(0, 0);
    setWindowFlags(Qt::Widget
                   | Qt::WindowMinimizeButtonHint
                   | Qt::WindowCloseButtonHint
                   | Qt::WindowTitleHint);

    widgetOpenGL = new GLWidget(this);

    listOfPolygons = new QListWidget(this);
    listOfPolygons->setMinimumWidth(200);

    lblList = new QLabel(QString::fromUtf8("Многоугольники:"));
    lblMode = new QLabel(QString::fromUtf8("Показать:"));

    addBtn = new QPushButton(QString::fromUtf8("Добавить"));
    editBtn = new QPushButton(QString::fromUtf8("Изменить"));
    editBtn->setCheckable(true);
    removeBtn = new QPushButton(QString::fromUtf8("Удалить"));
    editBtn->setEnabled(false);
    removeBtn->setEnabled(false);

    inCharCB = new QCheckBox(QString::fromUtf8("Внутренняя характеристическая область"));
    outCharCB = new QCheckBox(QString::fromUtf8("Внешняя характеристическая область"));
    geometrySearchCB = new QCheckBox(QString::fromUtf8("Геометрический поиск"));

    viewBtn = new QPushButton(QString::fromUtf8("Обзор"));
    viewBtn->setCheckable(true);
    inCharBtn = new QPushButton(QString::fromUtf8("Внутренняя характеристическая область"));
    inCharBtn->setCheckable(true);
    outCharBtn = new QPushButton(QString::fromUtf8("Внешняя характеристическая область"));
    outCharBtn->setCheckable(true);

    editBtnCheck = false;

    polygonBtnsLayout = new QHBoxLayout;
    polygonBtnsLayout->addWidget(addBtn);
    polygonBtnsLayout->addWidget(editBtn);
    polygonBtnsLayout->addWidget(removeBtn);

    demoBtnsLayout = new QVBoxLayout;
    demoBtnsLayout->addWidget(lblMode);
    demoBtnsLayout->addWidget(inCharCB);
    demoBtnsLayout->addWidget(outCharCB);
    demoBtnsLayout->addWidget(geometrySearchCB);

    optionsLayout = new QVBoxLayout;
    optionsLayout->addWidget(lblList);
    optionsLayout->addWidget(listOfPolygons);
    optionsLayout->addLayout(polygonBtnsLayout);
    optionsLayout->addLayout(demoBtnsLayout);
    optionsLayout->addStretch();

    widgetsLayout = new QHBoxLayout;
    widgetsLayout->addWidget(widgetOpenGL);
    widgetsLayout->addLayout(optionsLayout);

    mainlayout = new QVBoxLayout;
    mainlayout->addLayout(widgetsLayout);

    setLayout(mainlayout);

    connect(addBtn, SIGNAL(clicked()), this, SLOT(addBtnClick()));
    connect(editBtn, SIGNAL(clicked()), this, SLOT(editBtnClick()));
    connect(removeBtn, SIGNAL(clicked()), this, SLOT(removeBtnClick()));
    connect(widgetOpenGL, SIGNAL(endDraw()), this, SLOT(endDraw()));
//    connect(viewBtn, SIGNAL(clicked()), this, SLOT(viewBtnClick()));
//    connect(inCharBtn, SIGNAL(clicked()), this, SLOT(inCharBtnClick()));
//    connect(outCharBtn, SIGNAL(clicked()), this, SLOT(outCharBtnClick()));
    connect(inCharCB, SIGNAL(clicked()), this, SLOT(inCharCBClick()));
    connect(outCharCB, SIGNAL(clicked()), this, SLOT(outCharCBClick()));
    connect(geometrySearchCB, SIGNAL(clicked()), this, SLOT(geometrySearchCBClick()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), widgetOpenGL, SLOT(animate()));
    timer->start(1);
}

void Window::endDraw() {
    if (listOfPolygons->count() < 2) {
        addBtn->setEnabled(true);
        editBtn->setEnabled(true);
        removeBtn->setEnabled(true);
    } else {
        editBtn->setEnabled(true);
        removeBtn->setEnabled(true);
    }
}

void Window::addBtnClick() {
    addBtn->setEnabled(false);
    editBtn->setEnabled(false);
    removeBtn->setEnabled(false);

    inCharBtn->setChecked(false);
    outCharBtn->setChecked(false);
    viewBtn->setChecked(false);

    QChar name;
    if (listOfPolygons->count() > 0 && QChar(listOfPolygons->item(0)->text()[0]) > QChar('A')) {
        name = QChar('A');
    } else {
        name = QChar('A' + listOfPolygons->count());
    }
    new QListWidgetItem(name, listOfPolygons);
    widgetOpenGL->addPolygon(name);
}

void Window::editBtnClick() {
    if (editBtnCheck) {
        if (widgetOpenGL->activePolygonIsConvex()) {
            widgetOpenGL->endEdit();
            if (listOfPolygons->count() < 2) {
                addBtn->setEnabled(true);
            }
            removeBtn->setEnabled(true);
            editBtnCheck = false;
            editBtn->setChecked(false);
        } else {
            editBtn->setChecked(true);
        }
    } else {
        if (listOfPolygons->currentRow() != -1) {
            widgetOpenGL->editPolygon(listOfPolygons->currentRow());
            addBtn->setEnabled(false);
            removeBtn->setEnabled(false);

            inCharBtn->setChecked(false);
            outCharBtn->setChecked(false);
            viewBtn->setChecked(false);
            editBtnCheck = true;
            editBtn->setChecked(true);
        } else {
            editBtn->setChecked(false);
        }
    }
}

void Window::removeBtnClick() {
    widgetOpenGL->removePolygon(listOfPolygons->currentRow());
    widgetOpenGL->setMode(GLWidget::View);

    inCharBtn->setChecked(false);
    outCharBtn->setChecked(false);
    viewBtn->setChecked(false);

    QListWidgetItem *it = listOfPolygons->item(listOfPolygons->currentRow());
    delete it;

    if (listOfPolygons->count() < 2) {
        addBtn->setEnabled(true);
        editBtn->setEnabled(true);
        removeBtn->setEnabled(true);
    } else {
        editBtn->setEnabled(true);
        removeBtn->setEnabled(true);
    }
}

void Window::viewBtnClick() {
    viewBtn->setChecked(true);
    outCharBtn->setChecked(false);
    inCharBtn->setChecked(false);
    widgetOpenGL->setMode(GLWidget::View);
}

void Window::inCharBtnClick() {
    inCharBtn->setChecked(true);
    outCharBtn->setChecked(false);
    viewBtn->setChecked(false);
    widgetOpenGL->setMode(GLWidget::Demo4);
}

void Window::outCharBtnClick() {
    outCharBtn->setChecked(true);
    inCharBtn->setChecked(false);
    viewBtn->setChecked(false);
    widgetOpenGL->setMode(GLWidget::Demo3);
}

void Window::inCharCBClick() {
    widgetOpenGL->setDrawInChar(inCharCB->isChecked());
}

void Window::outCharCBClick() {
    widgetOpenGL->setDrawOutChar(outCharCB->isChecked());
}

void Window::geometrySearchCBClick() {
    widgetOpenGL->setDrawGeometrySearch(geometrySearchCB->isChecked());
}

void Window::moveToCenter() {
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
}
