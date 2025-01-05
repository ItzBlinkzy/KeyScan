#include "KeyInputWidget.h"
#include "KeyScan.h"
#include "qwidget.h"


KeyInputWidget::KeyInputWidget(QWidget* parent)
    : QWidget(parent), current_mode(CaptureMode::None), from_key_value(0), to_key_value(0) {
    this->setFocusPolicy(Qt::StrongFocus);
}

KeyInputWidget::~KeyInputWidget() {}

void KeyInputWidget::onCaptureFromKeyButtonClicked(QLabel* label) {
    current_mode = CaptureMode::FromKey;
    qDebug() << "CURRENT MODE: " << current_mode;
    if (label) {
        label->setText("Press a key for 'From Key'");
        from_key_label = label;
    }
}

void KeyInputWidget::onCaptureToKeyButtonClicked(QLabel* label) {
    current_mode = CaptureMode::ToKey; 
    if (label) {
        label->setText("Press a key for 'To Key'");
        to_key_label = label;
    }
}

void KeyInputWidget::setCurrentMode(CaptureMode mode) {
    current_mode = mode;
}

void redrawBindings() {

}
void KeyInputWidget::keyPressEvent(QKeyEvent* event) {
    qDebug() << "keypress from keyinput running";
    quint32 virtual_key = event->nativeVirtualKey();
    qDebug() << virtual_key << current_mode;
    if (current_mode == CaptureMode::None) return;

    else if (current_mode == CaptureMode::FromKey) {
        from_key_value = virtual_key;
        // label shouldnt be a nullptr at this point (i think?)
        qDebug() << "SETTING FROM KEY LABEL TO BE: " << KeyScan::KeyNameFromVirtualKeyCode(virtual_key);
        from_key_label->setText("Map Key: " + KeyScan::KeyNameFromVirtualKeyCode(virtual_key));
    }

    else if (current_mode == CaptureMode::ToKey) {
        to_key_value = virtual_key;
        to_key_label->setText("TO KEY: " + KeyScan::KeyNameFromVirtualKeyCode(virtual_key));
    }

    setCurrentMode(CaptureMode::None);
}

KeyInputWidget::CaptureMode KeyInputWidget::getCurrentMode() const {
    return current_mode;
}
