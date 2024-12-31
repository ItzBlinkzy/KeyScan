#include "KeyScan.h"
#include "ui_KeyScan.h"
#include <iostream>
#include <QStringList>
#include <QList>
#include <Windows.h>
#include <QKeyEvent>
#include <QTimer>
#include "KeyRemapper.h"

RecentKeyList recent_keys;
KeyRemapper* remapper;

KeyScan::KeyScan(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(1280, 720);
    ui.setupUi(this);

    ui.reset_keyboard_button->setFocusPolicy(Qt::NoFocus);

    //deubgging purposes
    remapper->addKeyMapping('A', 'B');
    remapper->addKeyMapping(VK_F1, VK_F10);
    remapper->addKeyMapping(VK_SPACE, VK_RETURN);


    // make default screen to be key test widget
    ui.stackedWidget->setCurrentWidget(ui.key_test_widget);

    // menubar click handlers
    connect(ui.action_key_test, &QAction::triggered, this, [this]() {
        ui.stackedWidget->setCurrentWidget(ui.key_test_widget);
        resetKeyboard(&ui);
        remapper->stopHook();
    });
    connect(ui.action_standard_typing_test, &QAction::triggered, this, &KeyScan::onMenuTypingTestClicked);
    connect(ui.action_key_remapper, &QAction::triggered, this, [this]() {
        ui.stackedWidget->setCurrentWidget(ui.remapper_page);
        remapper->startHook();
    });

    // other button click handlers
    connect(ui.reset_keyboard_button, &QPushButton::clicked, this, [this]() {
        resetKeyboard(&ui);
        remapper->stopHook();
    });

    for (auto& [key, value] : remapper->getRemappedKeys()) { 
        QString to_key = KeyNameFromScanCode(MapVirtualKeyW(key, MAPVK_VK_TO_VSC));
        QString from_key = KeyNameFromScanCode(MapVirtualKeyW(value, MAPVK_VK_TO_VSC));

        QLabel* label = new QLabel("Mapped: " + to_key + " -> " + from_key, this);
        ui.bindings_layout->addWidget(label);
    }

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

    for (size_t i = 0; i < layouts.size(); i++) {
        for (const KeyMapping& key : keyboard_layout[i]) {
            QPushButton* button = new QPushButton(key.keyText); 
            layouts[i]->addWidget(button);  
            button->setFocusPolicy(Qt::NoFocus);
            buttons[key.virtualKey] = button;
        }
    }
}

// optimize later with early returns etc.
QString findKeyText(uint16_t virtual_key, const QList<QList<KeyMapping>>& layout) {
    if (virtual_key == VK_SHIFT) {
        if ((GetKeyState(VK_LSHIFT) & 0x8000)) {
            virtual_key = VK_LSHIFT;
        }
        else if ((GetKeyState(VK_RSHIFT) & 0x8000)) {
            virtual_key = VK_RSHIFT;
        }
    }
    else if (virtual_key == VK_CONTROL) {
        if ((GetKeyState(VK_LCONTROL) & 0x8000)) {
            virtual_key = VK_LCONTROL;
        }
        else if ((GetKeyState(VK_RCONTROL) & 0x8000)) {
            virtual_key = VK_RCONTROL;
        }
    }
    else if (virtual_key == VK_MENU) {
        if ((GetKeyState(VK_LMENU) & 0x8000)) {
            virtual_key = VK_LMENU;
        }
        else if ((GetKeyState(VK_RMENU) & 0x8000)) {
            virtual_key = VK_RMENU;
        }
    }

    for (const auto& row : layout) {
        for (const auto& key : row) {
            if (key.virtualKey == virtual_key) {
                return key.keyText;
            }
        }
    }

    return QString("UNK");
}


// need checks to run certain pieces of code in keyreleaseevent for different screens e.g changing stylesheet of button when in diff screen


void KeyScan::keyPressEvent(QKeyEvent* event) {

    quint32 virtual_key = event->nativeVirtualKey();
    quint32 scan_code = event->nativeScanCode();
    qDebug() << "Windows Virtual Key Code:" << virtual_key;
    qDebug() << "Real Key Name" << findKeyText(virtual_key, keyboard_layout);
    
    if (ui.stackedWidget->currentWidget() == ui.remapper_page) {
        qDebug() << "CURRENTLY ON REMAPPER PAGE IM GONNA RETURN HERE because hook handling";
        return;
    }
    recent_keys.add(findKeyText(virtual_key, keyboard_layout), ui.recent_key_layout);
    //recent_keys.add()
    // modifier keys and their right and left equivalents
    if (virtual_key == VK_SHIFT) {
        // store scancode to help identify which key was actually pressed
        quint32 scan_code = event->nativeScanCode();
        if ((GetKeyState(VK_LSHIFT) & 0x8000) && scan_code == 0x2A) {
            virtual_key = VK_LSHIFT;
        }
        else if ((GetKeyState(VK_RSHIFT) & 0x8000) && scan_code == 0x36) {
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


/* later refactoring will look something like this
* if (page1) handlepage1(virtual_key, event)
* elif (page2) handlepage2(virtual_key, event)
* else handlepage3(virtual_key, event)
* 
*/
void KeyScan::keyReleaseEvent(QKeyEvent* event) {


    quint32 virtual_key = event->nativeVirtualKey();
    qDebug() << "Windows Virtual Key Code:" << virtual_key;
    if (ui.stackedWidget->currentWidget() == ui.remapper_page) {
        qDebug() << "CURRENTLY ON REMAPPER PAGE IM GONNA RETURN HERE because hook handling ";
        return;
    }
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

void KeyScan::onMenuTypingTestClicked(bool checked) {
    ui.stackedWidget->setCurrentWidget(ui.standard_typing_test_widget);
}

void KeyScan::resetKeyboard(Ui::KeyScanClass* ui) {
    // reset the recent keys list to empty
    recent_keys.resetRecentKeyList(ui->recent_key_layout);

    // reset keyboard keys to original colour
    QWidget* keyboard_container = ui->KeyboardContainer;
    qDebug() << keyboard_container->styleSheet();

    for (QPushButton* button : ui->KeyboardContainer->findChildren<QPushButton*>()) {
        button->setStyleSheet("background-color: #e0f2fe;");
    }
}

QString KeyScan::KeyNameFromScanCode(const unsigned scanCode)
{
    wchar_t buf[32]{};
    GetKeyNameTextW(scanCode << 16, buf, sizeof(buf));
    return QString::fromStdWString(buf);
}


KeyScan::~KeyScan()
{
}

