#include "KeyInputWidget.h"
#include "KeyScan.h"
#include <QWidget>



KeyInputWidget::KeyInputWidget(Ui::KeyScanClass* ui, ::QWidget* parent)
    : QWidget(parent), ui(ui), current_mode(CaptureMode::None), from_key_value(0), to_key_value(0) {
    this->setFocusPolicy(Qt::StrongFocus);
}

KeyInputWidget::~KeyInputWidget() {}

void KeyInputWidget::onCaptureFromKeyButtonClicked() {
    current_mode = CaptureMode::FromKey;
    ui->from_key_button->setText("Waiting for Input...");
}

void KeyInputWidget::onCaptureToKeyButtonClicked() {
    current_mode = CaptureMode::ToKey; 
    ui->to_key_button->setText("Waiting for input...");
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
        ui->from_key_label->setText(KeyScan::KeyNameFromVirtualKeyCode(virtual_key));
        ui->from_key_button->setText("Select Key to Remap");
    }

    else if (current_mode == CaptureMode::ToKey) {
        to_key_value = virtual_key;
        ui->to_key_label->setText(KeyScan::KeyNameFromVirtualKeyCode(virtual_key));
        ui->to_key_button->setText("New Key Assignment.");
    }

    setCurrentMode(CaptureMode::None);
}

KeyInputWidget::CaptureMode KeyInputWidget::getCurrentMode() const {
    return current_mode;
}
