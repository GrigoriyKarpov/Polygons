#include "dialog.h"
#include "window.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

Dialog::Dialog()
{
    //Опции окна: заголовок, размер, убрать кнопки максимизации и минимизации
    setWindowTitle("Line options");
    setFixedSize(0, 0);
    setWindowFlags(Qt::Dialog
                   | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint);

    // Создание надписей и выпадающих списков с опциями
    QLabel *colorLbl = new QLabel("Line color:");
    QLabel *widthLbl = new QLabel("Line width:");
    QLabel *styleLbl = new QLabel("Line type:");

    color = new QComboBox;

    color->addItem(QIcon(":/color/black.bmp"), "Black");
    color->addItem(QIcon(":/color/gray.bmp"), "Gray");
    color->addItem(QIcon(":/color/red.bmp"), "Red");
    color->addItem(QIcon(":/color/green.bmp"), "Green");
    color->addItem(QIcon(":/color/blue.bmp"), "Blue");
    color->addItem(QIcon(":/color/cyan.bmp"), "Cyan");
    color->addItem(QIcon(":/color/magenta.bmp"), "Magenta");
    color->addItem(QIcon(":/color/yellow.bmp"), "Yellow");

    width = new QComboBox;
    width->setIconSize(QSize(47, 11));

    width->addItem(QIcon(":/width/1.png"), "1 px");
    width->addItem(QIcon(":/width/2.png"), "2 px");
    width->addItem(QIcon(":/width/3.png"), "3 px");
    width->addItem(QIcon(":/width/4.png"), "4 px");
    width->addItem(QIcon(":/width/5.png"), "5 px");

    style = new QComboBox;
    style->setIconSize(QSize(47, 11));

    style->addItem(QIcon(":/style/solid.png"), "Solid");
    style->addItem(QIcon(":/style/dash.png"), "Dash");
    style->addItem(QIcon(":/style/dot.png"), "Dot");
    style->addItem(QIcon(":/style/dash_dot.png"), "Dash-dot");
    style->addItem(QIcon(":/style/dash_dot_dot.png"), "Dash-dot-dot");

    //Добавить кнопки Ok и Cancel
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    //добавление элементов на слои
    QVBoxLayout *left = new QVBoxLayout;
    left->addWidget(colorLbl);
    left->addWidget(widthLbl);
    left->addWidget(styleLbl);

    QVBoxLayout *right = new QVBoxLayout;
    right->addWidget(color);
    right->addWidget(width);
    right->addWidget(style);

    QHBoxLayout *buttons = new QHBoxLayout;
    buttons->addStretch();
    buttons->addWidget(buttonBox);
    buttons->addStretch();

    QHBoxLayout *options = new QHBoxLayout;
    options->addLayout(left);
    options->addLayout(right);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(options);
    layout->addLayout(buttons);

    setLayout(layout);
}

void Dialog::accept() {
    QColor c;
    int w;
    Qt::PenStyle s;

    if (color->currentIndex() == 0) {
        c = Qt::black;
    } else if (color->currentIndex() == 1) {
        c = Qt::gray;
    } else if (color->currentIndex() == 2) {
        c = Qt::red;
    } else if (color->currentIndex() == 3) {
        c = Qt::green;
    } else if (color->currentIndex() == 4) {
        c = Qt::blue;
    } else if (color->currentIndex() == 5) {
        c = Qt::cyan;
    } else if (color->currentIndex() == 6) {
        c = Qt::magenta;
    } else if (color->currentIndex() == 7) {
        c = Qt::yellow;
    }

    if (width->currentIndex() == 0) {
        w = 1;
    } else if (width->currentIndex() == 1) {
        w = 2;
    } else if (width->currentIndex() == 2) {
        w = 3;
    } else if (width->currentIndex() == 3) {
        w = 4;
    } else if (width->currentIndex() == 4) {
        w = 5;
    }

    if (style->currentIndex() == 0) {
        s = Qt::SolidLine;
    } else if (style->currentIndex() == 1) {
        s = Qt::DashLine;
    } else if (style->currentIndex() == 2) {
        s = Qt::DotLine;
    } else if (style->currentIndex() == 3) {
        s = Qt::DashDotLine;
    } else if (style->currentIndex() == 4) {
        s = Qt::DashDotDotLine;
    }

    QPen pen = QPen(c, w, s, Qt::RoundCap);

    emit sendOptions(pen);

    this->hide();
}

void Dialog::recieveOptions(QPen pen) {
    if (pen.style() == Qt::SolidLine) {
        style->setCurrentIndex(0);
    } else if (pen.style() == Qt::DashLine) {
        style->setCurrentIndex(1);
    } else if (pen.style() == Qt::DotLine) {
        style->setCurrentIndex(2);
    } else if (pen.style() == Qt::DashDotLine) {
        style->setCurrentIndex(3);
    } else if (pen.style() == Qt::DashDotDotLine) {
        style->setCurrentIndex(4);
    }

    if (pen.width() == 1) {
        width->setCurrentIndex(0);
    } else if (pen.width() == 2) {
        width->setCurrentIndex(1);
    } else if (pen.width() == 3) {
        width->setCurrentIndex(2);
    } else if (pen.width() == 4) {
        width->setCurrentIndex(3);
    } else if (pen.width() == 5) {
        width->setCurrentIndex(4);
    }

    if (pen.color() == Qt::black) {
        color->setCurrentIndex(0);
    } else if (pen.color() == Qt::gray) {
        color->setCurrentIndex(1);
    } else if (pen.color() == Qt::red) {
        color->setCurrentIndex(2);
    } else if (pen.color() == Qt::green) {
        color->setCurrentIndex(3);
    } else if (pen.color() == Qt::blue) {
        color->setCurrentIndex(4);
    } else if (pen.color() == Qt::cyan) {
        color->setCurrentIndex(5);
    } else if (pen.color() == Qt::magenta) {
        color->setCurrentIndex(6);
    } else if (pen.color() == Qt::yellow) {
        color->setCurrentIndex(7);
    }
}
