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
    void updateLastKeyPressedText(QLabel* label, uint16_t virtual_key);
    void updateTotalWorkingKeys(QLabel* label, uint16_t working_key_count);
    ~KeyScan();

private:
    Ui::KeyScanClass ui;
    QMap<uint8_t, QPushButton*> buttons;
};
