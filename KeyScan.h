#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <QString>
#include <array>
#include <QPushButton>
#include <QLayout>
#include <QtWidgets/QMainWindow>
#include "ui_KeyScan.h"
#include "qlabel.h"
class KeyScan : public QMainWindow
{
    Q_OBJECT

public:
    KeyScan(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void modifyButtonStyle(QPushButton* button, QString stylesheet);
    void updateLastKeyPressedText(QLabel* label, uint16_t virtual_key);
    void updateTotalWorkingKeys(QLabel* label, uint16_t working_key_count);
    ~KeyScan();

private:
    Ui::KeyScanClass ui;
    QMap<uint8_t, QPushButton*> buttons;
};


class RecentKeyList {
public:
    static constexpr size_t MAX_SIZE = 5;
    using ValueType = QString;

    RecentKeyList() = default;

    void add(ValueType value, QLayout* layout) {
        
        while (layout->count() >= MAX_SIZE) {
            // remove and delete the oldest widget if reached max_size
            QLayoutItem* item = layout->takeAt(0);
            if (item) {
                delete item->widget(); 
                delete item;           
            }
        }

        // add most recent key pressed
        QPushButton* button = new QPushButton(value);
        layout->addWidget(button);
    }
};
