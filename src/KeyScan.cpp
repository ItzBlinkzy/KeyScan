#include "KeyScan.h"
#include <iostream>
#include <QStringList>
#include <QList>
#include <Windows.h>
#include <QKeyEvent>
#include <QTimer>
#include "KeyRemapper.h"
#include "KeyInputWidget.h"
#include "RecentKeyList.h"

KeyScan::KeyScan(QWidget *parent)
    : QMainWindow(parent)
{
    remapper = new KeyRemapper(this);
    key_input_widget = new KeyInputWidget(this);

    this->setFixedSize(1280, 720);
    ui.setupUi(this);

    ui.reset_keyboard_button->setFocusPolicy(Qt::NoFocus);
    ui.bindings_layout->setAlignment(Qt::AlignTop);  
    ui.bindings_layout->setSpacing(0);            
    ui.bindings_layout->setContentsMargins(0, 0, 0, 0);


    // make default screen to be key test widget
    ui.stackedWidget->setCurrentWidget(ui.key_test_page);
    // default focus
    ui.key_test_page->setFocus();


    // menubar click handlers
    connect(ui.action_key_test, &QAction::triggered, this, [this]() {
        ui.stackedWidget->setCurrentWidget(ui.key_test_page);
        resetKeyboard(&ui);
        ui.key_test_page->setFocus();
    });
    connect(ui.action_standard_typing_test, &QAction::triggered, this, &KeyScan::onMenuTypingTestClicked);

    connect(ui.action_key_remapper, &QAction::triggered, this, [this]() {
        ui.stackedWidget->setCurrentWidget(ui.remapper_page);
        ui.remapper_page->setFocus();
    });

    // other button click handlers
    connect(ui.reset_keyboard_button, &QPushButton::clicked, this, [this]() {
        resetKeyboard(&ui);
    });

    connect(ui.change_remap_state_button, &QPushButton::clicked, this, [this]() {
        // not hooked 
        if (!(remapper->isHooked())) {
            // will probably refactor to return bool if it succesfully hooks or not
            remapper->startHook();
            ui.change_remap_state_button->setText("STOP");
            ui.is_running_label->setText("RUNNING");

            ui.remapper_state_widget->setStyleSheet(
                "QWidget#remapper_state_widget {"
                "background-color: #4ade80;"
                "border-radius: 5%;"
                "border: 1px solid #737373;"
                "}"
            );
        }
        else {
            remapper->stopHook();
            ui.change_remap_state_button->setText("START");
            ui.is_running_label->setText("NOT RUNNING");

            ui.remapper_state_widget->setStyleSheet(
                "QWidget#remapper_state_widget {"
                "background-color: #fca5a5;"
                "border-radius: 5%;"
                "border: 1px solid #737373;"
                "}"
            );
        }
    });

    connect(ui.clear_bindings_button, &QPushButton::clicked, this, [this]() {
        remapper->clearKeyMappings(ui.bindings_layout);
    });

    connect(ui.from_key_button, &QPushButton::clicked, this, [this]() {
        key_input_widget->onCaptureFromKeyButtonClicked(ui.from_key_label);
    });

    connect(ui.to_key_button, &QPushButton::clicked, this, [this]() {
        key_input_widget->onCaptureToKeyButtonClicked(ui.to_key_label);
    });

    connect(ui.create_new_binding_button, &QPushButton::clicked, this, [this]() {
        quint32 from_key = key_input_widget->from_key_value;
        quint32 to_key = key_input_widget->to_key_value;
        bool remap_success = remapper->addKeyMapping(from_key, to_key, key_input_widget);

        if (remap_success) {
            KeyScan::clearLayout(ui.bindings_layout);
            remapper->drawCurrentBinds(ui.bindings_list_container, ui.bindings_layout);
            key_input_widget->from_key_label->setText("From Key: ");
            key_input_widget->to_key_label->setText("To Key: ");

            key_input_widget->from_key_value = -1;
            key_input_widget->to_key_value = -1;
        }
    });

    // preload the bindings if there are any before (may possibly implement)
    //for (auto& [key, value] : remapper->getRemappedKeys()) { 
    //    QString to_key = KeyNameFromScanCode(MapVirtualKeyW(key, MAPVK_VK_TO_VSC));
    //    QString from_key = KeyNameFromScanCode(MapVirtualKeyW(value, MAPVK_VK_TO_VSC));

    //    QLabel* label = new QLabel(to_key + " -> " + from_key, this);
    //    //QPushButton* remove_button = new QPushButton()
    //    ui.bindings_layout->addWidget(label);
    //}

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

// need checks to run certain pieces of code in keyreleaseevent for different screens e.g changing stylesheet of button when in diff screen

void KeyScan::keyPressEvent(QKeyEvent* event) {

    quint32 virtual_key = event->nativeVirtualKey();
    quint32 scan_code = event->nativeScanCode();
    qDebug() << "Windows Virtual Key Code:" << virtual_key;
    qDebug() << "Real Key Name" << KeyNameFromVirtualKeyCode(virtual_key);
    
    if (ui.stackedWidget->currentWidget() == ui.remapper_page) {
        key_input_widget->keyPressEvent(event);
        return;
    }

    else if (ui.stackedWidget->currentWidget() == ui.standard_typing_test_page) {
        // custom handling of this page keyevents. 
        return;
    }

    recent_keys.add(KeyNameFromVirtualKeyCode(virtual_key), ui.recent_key_layout);
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
        qDebug() << "keyRelease not needed for remapper page.";
        return;
    }

    else if (ui.stackedWidget->currentWidget() == ui.standard_typing_test_page) {
        qDebug() << "keyRelease not needed in this typing test page for now.";
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
    ui.stackedWidget->setCurrentWidget(ui.standard_typing_test_page);
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


void KeyScan::clearLayout(QLayout* layout) {
    if (!layout) return;

    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item; 
    }
}

QString KeyScan::KeyNameFromScanCode(const unsigned scanCode)
{
    wchar_t buf[32]{};
    GetKeyNameTextW(scanCode << 16, buf, sizeof(buf));
    return QString::fromStdWString(buf);
}

QString KeyScan::KeyNameFromVirtualKeyCode(const unsigned virtualKeyCode)
{
    return KeyNameFromScanCode(MapVirtualKeyW(virtualKeyCode, MAPVK_VK_TO_VSC));
}



KeyScan::~KeyScan()
{
}

