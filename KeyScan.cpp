#include "KeyScan.h"
#include "ui_KeyScan.h"
#include <iostream>



KeyScan::KeyScan(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.NavbarLayout->setAlignment(Qt::AlignTop);
#include <QStringList>
#include <QList>

    const QList<QStringList> keyboardLayout = {
        // Function Keys Row
        {"Esc", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12"},

        // Number Row
        {"`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "Backspace"},

        // Top Row
        {"Tab", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "\\"},

        // Home Row
        {"Caps Lock", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "Enter"},

        // Bottom Row
        {"Shift", "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "Shift (Right)"},

        // Space and Modifier Keys Row
        {"Ctrl", "Win", "Alt", "Space", "Alt (Right)", "Win (Right)", "Menu", "Ctrl (Right)"},

        // Arrow Keys Row
        {"Up", "Down", "Left", "Right"},

        // Navigation Keys Row
        {"Insert", "Delete", "Home", "End", "Page Up", "Page Down"},

        // Numeric Keypad Row (assuming Num Lock is active)
        {"Num Lock", "Numpad /", "Numpad *", "Numpad -",
         "Numpad 7", "Numpad 8", "Numpad 9", "Numpad +",
         "Numpad 4", "Numpad 5", "Numpad 6",
         "Numpad 1", "Numpad 2", "Numpad 3", "Numpad Enter",
         "Numpad 0", "Numpad ."}
    };
    const QStringList layoutList = { "f1_layout", "row2_layout", "row3_layout", "row4_layout", "row5_layout" };

    // Assume we have a QMap to map layout names to actual QHBoxLayout pointers
    QList<QHBoxLayout*> layouts = {
         ui.f1_layout,
         ui.row2_layout,
         ui.row3_layout,
         ui.row4_layout,
         ui.row5_layout,
         ui.row6_layout,
    };

    // Loop through the first 5 rows and add buttons to each layout
    for (int i = 0; i < layouts.size(); i++) {
        for (const QString& keyLabel : keyboardLayout[i]) {
            QPushButton* button = new QPushButton(keyLabel); // Create button with key label
            layouts[i]->addWidget(button);                   // Add button to the corresponding layout
        }
    }

}

KeyScan::~KeyScan()
{
}
