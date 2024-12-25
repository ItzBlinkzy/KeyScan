#include "KeyScan.h"
#include "ui_KeyScan.h"
#include <iostream>
#include <QStringList>
#include <QList>
#include <Windows.h>
#include <QKeyEvent>
#include <QTimer>
struct KeyMapping {
    QString keyText;
    uint16_t virtualKey;
};

const QList<QList<KeyMapping>> keyboard_layout = {
    {{"Esc", VK_ESCAPE}, {"F1", VK_F1}, {"F2", VK_F2}, {"F3", VK_F3}, {"F4", VK_F4}, {"F5", VK_F5},
     {"F6", VK_F6}, {"F7", VK_F7}, {"F8", VK_F8}, {"F9", VK_F9}, {"F10", VK_F10}, {"F11", VK_F11}, {"F12", VK_F12}},
    {{"`", VK_OEM_8}, {"1", '1'}, {"2", '2'}, {"3", '3'}, {"4", '4'}, {"5", '5'}, {"6", '6'},
     {"7", '7'}, {"8", '8'}, {"9", '9'}, {"0", '0'}, {"-", VK_OEM_MINUS}, {"=", VK_OEM_PLUS}, {"Backspace", VK_BACK}},
    {{"Tab", VK_TAB}, {"Q", 'Q'}, {"W", 'W'}, {"E", 'E'}, {"R", 'R'}, {"T", 'T'}, {"Y", 'Y'},
     {"U", 'U'}, {"I", 'I'}, {"O", 'O'}, {"P", 'P'}, {"[", VK_OEM_4}, {"]", VK_OEM_6}, {"\\", VK_OEM_5}},
    {{"Caps Lock", VK_CAPITAL}, {"A", 'A'}, {"S", 'S'}, {"D", 'D'}, {"F", 'F'}, {"G", 'G'}, {"H", 'H'},
     {"J", 'J'}, {"K", 'K'}, {"L", 'L'}, {";", VK_OEM_1}, {"'", VK_OEM_3}, {"Enter", VK_RETURN}},
    {{"Left Shift", VK_LSHIFT}, {"Z", 'Z'}, {"X", 'X'}, {"C", 'C'}, {"V", 'V'}, {"B", 'B'}, {"N", 'N'},
     {"M", 'M'}, {",", VK_OEM_COMMA}, {".", VK_OEM_PERIOD}, {"/", VK_OEM_2}, {"Right Shift", VK_RSHIFT}},
    {{"Left Ctrl", VK_LCONTROL}, {"Windows", VK_LWIN}, {"Left Alt", VK_LMENU}, {"Space", VK_SPACE},
     {"Right Alt", VK_RMENU}, {"Menu", VK_APPS}, {"Right Ctrl", VK_RCONTROL}},
    {{"Up", VK_UP}},
    {{"Left", VK_LEFT}, {"Down", VK_DOWN}, {"Right", VK_RIGHT}},
    {{"INS", VK_INSERT}, {"HOME", VK_HOME}, {"PG UP", VK_PRIOR}},
    {{"DEL", VK_DELETE}, {"END", VK_END}, {"PG DWN", VK_NEXT}},
    //{{"Num Lock", VK_NUMLOCK}, {"Numpad /", VK_DIVIDE}, {"Numpad *", VK_MULTIPLY}, {"Numpad -", VK_SUBTRACT},
    // {"Numpad 7", VK_NUMPAD7}, {"Numpad 8", VK_NUMPAD8}, {"Numpad 9", VK_NUMPAD9}, {"Numpad +", VK_ADD},
    // {"Numpad 4", VK_NUMPAD4}, {"Numpad 5", VK_NUMPAD5}, {"Numpad 6", VK_NUMPAD6},
    // {"Numpad 1", VK_NUMPAD1}, {"Numpad 2", VK_NUMPAD2}, {"Numpad 3", VK_NUMPAD3},
    // {"Numpad 0", VK_NUMPAD0}, {"Numpad .", VK_DECIMAL}}
};

RecentKeyList recent_keys;


KeyScan::KeyScan(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(1280, 720);
    ui.setupUi(this);
    ui.NavbarLayout->setAlignment(Qt::AlignTop);



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
            buttons[key.virtualKey] = button;
        }
    }
}


QString findKeyText(uint16_t virtualKey, const QList<QList<KeyMapping>>& layout) {



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
    QMainWindow::keyPressEvent(event);
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
    QMainWindow::keyPressEvent(event);
}
void KeyScan::modifyButtonStyle(QPushButton* button, QString stylesheet) {
    button->setStyleSheet(stylesheet);
}

void KeyScan::updateLastKeyPressedText(QLabel* label, uint16_t virtual_key) {

}

void KeyScan::updateTotalWorkingKeys(QLabel* label, uint16_t working_key_count) {
}


KeyScan::~KeyScan()
{
}
