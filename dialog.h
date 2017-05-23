#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QPen>

class Dialog : public QDialog {
    Q_OBJECT

signals:
    void sendOptions(QPen pen);

public slots:
    void recieveOptions(QPen pen);
    void accept();

public:
    Dialog();
    QDialogButtonBox *buttonBox;

private:
    QComboBox *style;
    QComboBox *color;
    QComboBox *width;
};

#endif // DIALOG_H
