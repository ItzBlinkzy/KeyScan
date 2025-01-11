#include "KeyInputWidget.h"
#include "KeyScan.h"
#include <QWidget>




KeyInputWidget::KeyInputWidget(QLabel* from_key_label, QLabel* to_key_label,
    QPushButton* from_button, QPushButton* to_button,
    QWidget* parent)
    : QWidget(parent)
    , m_from_key_label(from_key_label)
    , m_to_key_label(to_key_label)
    , m_from_button(from_button)
    , m_to_button(to_button)
{

}


KeyInputWidget::~KeyInputWidget() {}

void KeyInputWidget::onCaptureFromKeyButtonClicked() {
    current_mode = CaptureMode::FromKey;
    m_from_key_label->setText("Waiting for Input...");
}

void KeyInputWidget::onCaptureToKeyButtonClicked() {
    current_mode = CaptureMode::ToKey;
    m_to_key_label->setText("Waiting for input...");
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
        m_from_key_label->setText(KeyScan::KeyNameFromVirtualKeyCode(virtual_key));
        m_from_button->setText("Select Key to Remap");
    }

    else if (current_mode == CaptureMode::ToKey) {
        to_key_value = virtual_key;
        m_to_key_label->setText(KeyScan::KeyNameFromVirtualKeyCode(virtual_key));
        m_to_button->setText("New Key Assignment.");
    }

    setCurrentMode(CaptureMode::None);
}

KeyInputWidget::CaptureMode KeyInputWidget::getCurrentMode() const {
    return current_mode;
}
