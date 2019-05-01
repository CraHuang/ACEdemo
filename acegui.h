#ifndef ACEGUI_H
#define ACEGUI_H

#include "D:\QTProjects\ACEdemo\ace.h"
#include <QMainWindow>

namespace Ui {
class ACEGUI;
class ACE;
}

class ACEGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit ACEGUI(QWidget *parent = nullptr);
    ~ACEGUI();

private slots:
    void on_ownerOpenButton_clicked(bool checked);
    void on_encryptButton_clicked(bool checked);
    void on_userOpenButton_clicked(bool checked);
    void on_decryptButton_clicked(bool checked);
    void on_initButton_clicked();
    void on_showPpushButton_clicked();
    void on_lineEdit_OwnerId_textChanged(const QString &arg1);
    void on_lineEdit_UserId_returnPressed();

private:
    Ui::ACEGUI *ui;
    ACE ace;
    KEY *keyset;
};

#endif // ACEGUI_H
