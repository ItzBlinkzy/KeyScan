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
    this->setFixedSize(1280, 720);
    remapper = new KeyRemapper(this);
    ui.setupUi(this);
    key_input_widget = new KeyInputWidget(ui.from_key_label, ui.to_key_label, ui.from_key_button, ui.to_key_button, this);
    key_input_widget->setAttribute(Qt::WA_TransparentForMouseEvents);

    ui.reset_keyboard_button->setFocusPolicy(Qt::NoFocus);
    ui.bindings_layout->setAlignment(Qt::AlignTop);
    ui.bindings_layout->setSpacing(0);
    ui.bindings_layout->setContentsMargins(0, 0, 0, 0);


    // make default screen to be key test widget
    ui.stackedWidget->setCurrentWidget(ui.key_test_page);
    // default focus
    ui.stackedWidget->setFocus();


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
        this->setFocus();
        key_input_widget->onCaptureFromKeyButtonClicked();
    });

    connect(ui.to_key_button, &QPushButton::clicked, this, [this]() {
        this->setFocus();
        key_input_widget->onCaptureToKeyButtonClicked();
    });

    connect(ui.create_new_binding_button, &QPushButton::clicked, this, [this]() {
        quint32 from_key = key_input_widget->from_key_value;
        quint32 to_key = key_input_widget->to_key_value;
        bool remap_success = remapper->addKeyMapping(from_key, to_key, key_input_widget);

        if (remap_success) {
            KeyScan::clearLayout(ui.bindings_layout);
            remapper->drawCurrentBinds(ui.bindings_list_container, ui.bindings_layout);
            ui.from_key_label->setText("");
            ui.to_key_label->setText("");

            key_input_widget->from_key_value = -1;
            key_input_widget->to_key_value = -1;
        }
    });

    // preload the bindings if there are any before (may possibly implement)
    //for (auto& [key, value] : remapper->getRemappedKeys()) {
    //    QString to_key = KeyNameFromVirtualKey(key);
    //    QString from_key = KeyNameFromVirtualKey(value);

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
    virtual_key = handleModifierKeys(event, virtual_key);

    if (buttons.contains(virtual_key)) {
        QPushButton* button = buttons[virtual_key];
        modifyButtonStyle(button, "background-color: #ea580c;");
    }
}



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
    virtual_key = handleModifierKeys(event, virtual_key);

    if (buttons.contains(virtual_key)) {
        QPushButton* button = buttons[virtual_key];
        //f19e0e
        modifyButtonStyle(button, "background-color: #fdba74;");

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
        button->setStyleSheet("background-color: #feece0;");
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

quint32 KeyScan::handleModifierKeys(QKeyEvent* event, quint32 virtual_key) {
    const quint32 scanCode = event->nativeScanCode();

    switch (virtual_key) {
        case VK_SHIFT:
            return (scanCode == 0x2A) ? VK_LSHIFT : (scanCode == 0x36 ? VK_RSHIFT : virtual_key);

        case VK_CONTROL:
            return (scanCode == 0x1D) ? VK_LCONTROL : (scanCode == 0x11D ? VK_RCONTROL : virtual_key);

        case VK_MENU:
            return (scanCode == 0x38) ? VK_LMENU : (scanCode == 0x138 ? VK_RMENU : virtual_key);

        default:
            return virtual_key;
    }
}




KeyScan::~KeyScan()
{
}
