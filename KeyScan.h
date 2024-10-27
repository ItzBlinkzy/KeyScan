#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_KeyScan.h"

class KeyScan : public QMainWindow
{
    Q_OBJECT

public:
    KeyScan(QWidget *parent = nullptr);
    ~KeyScan();

private:
    Ui::KeyScanClass ui;
};
