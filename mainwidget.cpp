#include <QtGui>
#include <QDesktopWidget>
#include <QMainWindow>
#include "MainWidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent) {
    widgetOpenGL = new GLWidget(this);

    listOfPolygons = new QListWidget(this);
    listOfPolygons->setMinimumWidth(200);

    lblList = new QLabel(QString::fromUtf8("Многоугольники:"));
    lblMode = new QLabel(QString::fromUtf8("Показать:"));

    saveBtn = new QPushButton(QString::fromUtf8("Сохранить в файл"));
    saveBtn->setEnabled(false);
    openBtn = new QPushButton(QString::fromUtf8("Добавить из файла"));

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

    saveBtnsLayout = new QHBoxLayout;
    saveBtnsLayout->addWidget(saveBtn);
    saveBtnsLayout->addWidget(openBtn);

    moveOrRotateGB = new QGroupBox(trUtf8("Тип манипуляций с многоугольниками"));
    moveRadio = new QRadioButton(trUtf8("Двигать"));
    rotateRadio = new QRadioButton(trUtf8("Вращать"));
    moveRadio->setChecked(true);
    moveOrRotateLayout = new QVBoxLayout;
    moveOrRotateLayout->addWidget(moveRadio);
    moveOrRotateLayout->addWidget(rotateRadio);
    moveOrRotateGB->setLayout(moveOrRotateLayout);

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
    optionsLayout->addLayout(saveBtnsLayout);
    optionsLayout->addLayout(demoBtnsLayout);
    optionsLayout->addWidget(moveOrRotateGB);
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
    connect(moveRadio, SIGNAL(clicked()), this, SLOT(moveRadioClick()));
    connect(rotateRadio, SIGNAL(clicked()), this, SLOT(rotateRadioClick()));
    connect(openBtn, SIGNAL(clicked()), this, SLOT(loadFromFile()));
    connect(saveBtn, SIGNAL(clicked()), this, SLOT(saveToFile()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), widgetOpenGL, SLOT(animate()));
    timer->start(1);
}

void MainWidget::endDraw() {
    saveBtn->setEnabled(true);

    if (listOfPolygons->count() < 2) {
        addBtn->setEnabled(true);
        editBtn->setEnabled(true);
        removeBtn->setEnabled(true);
        openBtn->setEnabled(true);
    } else {
        editBtn->setEnabled(true);
        removeBtn->setEnabled(true);
        openBtn->setEnabled(false);
    }
}

void MainWidget::addBtnClick() {
    addBtn->setEnabled(false);
    editBtn->setEnabled(false);
    removeBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    openBtn->setEnabled(false);

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

void MainWidget::editBtnClick() {
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

void MainWidget::removeBtnClick() {
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
        openBtn->setEnabled(true);
    } else {
        editBtn->setEnabled(true);
        removeBtn->setEnabled(true);
        openBtn->setEnabled(false);
    }

    if (listOfPolygons->count() == 0) {
        saveBtn->setEnabled(false);
    }
}

void MainWidget::viewBtnClick() {
    viewBtn->setChecked(true);
    outCharBtn->setChecked(false);
    inCharBtn->setChecked(false);
    widgetOpenGL->setMode(GLWidget::View);
}

void MainWidget::inCharBtnClick() {
    inCharBtn->setChecked(true);
    outCharBtn->setChecked(false);
    viewBtn->setChecked(false);
    widgetOpenGL->setMode(GLWidget::Demo4);
}

void MainWidget::outCharBtnClick() {
    outCharBtn->setChecked(true);
    inCharBtn->setChecked(false);
    viewBtn->setChecked(false);
    widgetOpenGL->setMode(GLWidget::Demo3);
}

void MainWidget::inCharCBClick() {
    widgetOpenGL->setDrawInChar(inCharCB->isChecked());
}

void MainWidget::outCharCBClick() {
    widgetOpenGL->setDrawOutChar(outCharCB->isChecked());
}

void MainWidget::moveRadioClick() {
    if (moveRadio->isChecked()) {
        widgetOpenGL->setRotate(false);
    }
}

void MainWidget::rotateRadioClick() {
    if (rotateRadio->isChecked()) {
        widgetOpenGL->setRotate(true);
    }
}

void MainWidget::geometrySearchCBClick() {
    widgetOpenGL->setDrawGeometrySearch(geometrySearchCB->isChecked());
}

void MainWidget::saveToFile() {
    if (listOfPolygons->currentRow() != -1) {
        QString data = widgetOpenGL->getPolygonXML(listOfPolygons->currentRow());

        QString fileName = QFileDialog::getSaveFileName(this,
            trUtf8("Сохранить полигон в файл"), "",
            trUtf8("XML (*.xml);;Все файлы (*)"));

        if (fileName.isEmpty()) {
            return;
        } else {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QMessageBox::information(this, trUtf8("Невозможно открыть файл"),
                    file.errorString());
                return;
            }
            QTextStream out(&file);
            out << data;
            file.close();
        }
    } else {
        QMessageBox::warning(this, trUtf8("Предупреждение"), trUtf8("Вы не выбрали многоугольник в списке!"));
    }
}

void MainWidget::loadFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this,
        trUtf8("Сохранить полигон в файл"), "",
        trUtf8("XML (*.xml);;Все файлы (*)"));

    if (fileName.isEmpty()) {
        return;
    } else {
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::information(this, trUtf8("Невозможно открыть файл"),
                file.errorString());
            return;
        }

        //ЧТЕНИЕ ФАЙЛА XML
        QXmlStreamReader reader(&file);
        QString name;
        Point point;
        Polygon::Polygon pol;
        bool success = false;

        if (reader.readNextStartElement()) {
            if (reader.name() == "polygon"){
                while(reader.readNextStartElement()){
                    if(reader.name() == "name"){
                        name = reader.readElementText();
                    } else if (reader.name() == "points") {
                        while(reader.readNextStartElement()) {
                            if(reader.name() == "point"){
                                success = true;

                                reader.readNextStartElement();
                                point.setX(reader.readElementText().toDouble());
                                reader.readNextStartElement();
                                point.setY(reader.readElementText().toDouble());

                                pol.addPoint(point);

                                reader.readNextStartElement();
                            }
                        }
                    }
                }
            } else {
                QMessageBox::information(this, trUtf8("Некорректный файл!"),
                    file.errorString());
            }
        }
        file.close(); // Закрываем файл

        if (success) {
            widgetOpenGL->addPolygonFromXML(pol, name);

            //добавить в список
            QChar n;
            if (listOfPolygons->count() > 0 && QChar(listOfPolygons->item(0)->text()[0]) > QChar('A')) {
                n = QChar('A');
            } else {
                n = QChar('A' + listOfPolygons->count());
            }
            new QListWidgetItem(n, listOfPolygons);

            //Выключить кнопки
            if (listOfPolygons->count() < 2) {
                addBtn->setEnabled(true);
                editBtn->setEnabled(true);
                removeBtn->setEnabled(true);
                openBtn->setEnabled(true);
            } else {
                editBtn->setEnabled(true);
                removeBtn->setEnabled(true);
                openBtn->setEnabled(false);
            }
        }
    }
}
