#include "KeyScan.h"
#include "ui_KeyScan.h"
#include <iostream>
#include <QStringList>
#include <QList>
#include <Windows.h>
#include <QKeyEvent>
#include <QTimer>


KeyScan::KeyScan(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.NavbarLayout->setAlignment(Qt::AlignTop);

    const QList<QStringList> keyboardLayout = {

        {"Esc", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12"},

        {"`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "Backspace"},

        {"Tab", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "\\"},

        {"Caps Lock", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "Enter"},

        {"Shift", "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "Shift (Right)"},

        {"Ctrl", "Win", "Alt", "Space", "Alt (Right)", "Win (Right)", "Menu", "Ctrl (Right)"},
        {"Up"},
        {"Left", "Down", "Right"},
        {"Insert", "Home", "Pg Up"},
        {"Del", "End", "Pg Down" },
        {"Num Lock", "Numpad /", "Numpad *", "Numpad -",
         "Numpad 7", "Numpad 8", "Numpad 9", "Numpad +",
         "Numpad 4", "Numpad 5", "Numpad 6",
         "Numpad 1", "Numpad 2", "Numpad 3", "Numpad Enter",
         "Numpad 0", "Numpad ."}
    };


    QList<QHBoxLayout*> layouts = {
         ui.f1_layout,
         ui.row2_layout,
         ui.row3_layout,
         ui.row4_layout,
         ui.row5_layout,
         ui.row6_layout,
         ui.top_arrow_layout,
         ui.bottom_arrow_layout,
         ui.row_7_layout,
         ui.row_8_layout,
    };

    for (int i = 0; i < layouts.size(); i++) {
        for (const QString& keyLabel : keyboardLayout[i]) {
            QPushButton* button = new QPushButton(keyLabel); 
            layouts[i]->addWidget(button);  
            buttons[keyLabel] = button;
        }
    }
    setFocusPolicy(Qt::StrongFocus);
}

void KeyScan::keyPressEvent(QKeyEvent* event) {
    QString keyText = event->text().toUpper();

    if (buttons.contains(keyText)) {
        QPushButton* button = buttons[keyText];
        button->setStyleSheet("background-color: red;");
    }
    QMainWindow(keyPressEvent);
}

void KeyScan::keyReleaseEvent(QKeyEvent* event) {
    QString keyText = event->text().toUpper();

    if (buttons.contains(keyText)) {
        QPushButton* button = buttons[keyText];
        button->setStyleSheet("background-color: orange;");
    }
    QMainWindow(keyPressEvent);
}

void KeyScan::changeButtonColor(QPushButton* button, QString stylesheet) {
    button->setStyleSheet(stylesheet);
}

KeyScan::~KeyScan()
{
}
