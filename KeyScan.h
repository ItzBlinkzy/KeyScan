#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_KeyScan.h"

class KeyScan : public QMainWindow
{
    Q_OBJECT

public:
    KeyScan(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void changeButtonColor(QPushButton* button, QString stylesheet);
    ~KeyScan();
    void addButtonToKeyboardLayout();

private:
    Ui::KeyScanClass ui;
    QMap<QString, QPushButton*> buttons;
};
