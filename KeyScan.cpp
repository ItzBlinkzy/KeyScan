#include "KeyScan.h"
#include "ui_KeyScan.h"
#include <iostream>
#include <QStringList>
#include <QList>
#include <Windows.h>
#include <QKeyEvent>
#include <QTimer>

RecentKeyList recent_keys;


KeyScan::KeyScan(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(1280, 720);
    ui.setupUi(this);

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
         //ui.numpad_layout
    };

    for (int i = 0; i < layouts.size(); i++) {
        for (const KeyMapping& key : keyboard_layout[i]) {
            QPushButton* button = new QPushButton(key.keyText); 
            layouts[i]->addWidget(button);  
            button->setFocusPolicy(Qt::NoFocus);
            buttons[key.virtualKey] = button;
        }
    }
}


QString findKeyText(uint16_t virtualKey, const QList<QList<KeyMapping>>& layout) {
    if (virtualKey == VK_SHIFT) {
        if ((GetKeyState(VK_LSHIFT) & 0x8000)) {
            virtualKey = VK_LSHIFT;
        }
        else if ((GetKeyState(VK_RSHIFT) & 0x8000)) {
            virtualKey = VK_RSHIFT;
        }
    }
    else if (virtualKey == VK_CONTROL) {
        if ((GetKeyState(VK_LCONTROL) & 0x8000)) {
            virtualKey = VK_LCONTROL;
        }
        else if ((GetKeyState(VK_RCONTROL) & 0x8000)) {
            virtualKey = VK_RCONTROL;
        }
    }
    else if (virtualKey == VK_MENU) {
        if ((GetKeyState(VK_LMENU) & 0x8000)) {
            virtualKey = VK_LMENU;
        }
        else if ((GetKeyState(VK_RMENU) & 0x8000)) {
            virtualKey = VK_RMENU;
        }
    }

    for (const auto& row : layout) {
        for (const auto& key : row) {
            if (key.virtualKey == virtualKey) {
                return key.keyText;
            }
        }
    }

    return QString("UNK");
}



void KeyScan::keyPressEvent(QKeyEvent* event) {
    quint32 virtual_key = event->nativeVirtualKey();
    qDebug() << "Windows Virtual Key Code:" << virtual_key;
    qDebug() << "Real Key Name" << findKeyText(virtual_key, keyboard_layout);
    
    recent_keys.add(findKeyText(virtual_key, keyboard_layout), ui.recent_key_layout);

    // modifier keys and their right and left equivalents
    if (virtual_key == VK_SHIFT) {
        // store scancode to help identify which key was actually pressed
        quint32 scanCode = event->nativeScanCode();
        if ((GetKeyState(VK_LSHIFT) & 0x8000) && scanCode == 0x2A) {
            virtual_key = VK_LSHIFT;
        }
        else if ((GetKeyState(VK_RSHIFT) & 0x8000) && scanCode == 0x36) {
            virtual_key = VK_RSHIFT;
        }
    }
    else if (virtual_key == VK_CONTROL) {
        quint32 scanCode = event->nativeScanCode();
        if ((GetKeyState(VK_LCONTROL) & 0x8000) && scanCode == 0x1D) {
            virtual_key = VK_LCONTROL;
        }
        else if ((GetKeyState(VK_RCONTROL) & 0x8000) && scanCode == 0x11D) {
            virtual_key = VK_RCONTROL;
        }
    }
    // alt keys
    else if (virtual_key == VK_MENU) {  
        quint32 scanCode = event->nativeScanCode();
        if ((GetKeyState(VK_LMENU) & 0x8000) && scanCode == 0x38) {
            virtual_key = VK_LMENU;
        }
        else if ((GetKeyState(VK_RMENU) & 0x8000) && scanCode == 0x138) {
            virtual_key = VK_RMENU;
        }
    }

    if (buttons.contains(virtual_key)) {
        QPushButton* button = buttons[virtual_key];
        modifyButtonStyle(button, "background-color: #ffcc2c;");
    }
}



void KeyScan::keyReleaseEvent(QKeyEvent* event) {
    quint32 virtual_key = event->nativeVirtualKey();
    qDebug() << "Windows Virtual Key Code:" << virtual_key;
    // modifier keys and their right and left equivalents

    if (virtual_key == VK_SHIFT) {
        quint32 scanCode = event->nativeScanCode();
        if (scanCode == 0x2A) {  
            virtual_key = VK_LSHIFT;
        }
        else if (scanCode == 0x36) {  
            virtual_key = VK_RSHIFT;
        }
    }
    else if (virtual_key == VK_CONTROL) {
        quint32 scanCode = event->nativeScanCode();
        if (scanCode == 0x1D) { 
            virtual_key = VK_LCONTROL;
        }
        else if (scanCode == 0x11D) {  
            virtual_key = VK_RCONTROL;
        }
    }

    // alt keys
    else if (virtual_key == VK_MENU) {
        quint32 scanCode = event->nativeScanCode();
        if (scanCode == 0x38) { 
            virtual_key = VK_LMENU;
        }
        else if (scanCode == 0x138) {  
            virtual_key = VK_RMENU;
        }
    }

    if (buttons.contains(virtual_key)) {
        QPushButton* button = buttons[virtual_key];
        modifyButtonStyle(button, "background-color: #00635f;color: white;");

    }
}
void KeyScan::modifyButtonStyle(QPushButton* button, QString stylesheet) {
    button->setStyleSheet(stylesheet);
}


KeyScan::~KeyScan()
{
}
