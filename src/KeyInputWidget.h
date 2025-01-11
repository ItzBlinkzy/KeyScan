#pragma once
#include <Windows.h>
#include <QKeyEvent>
#include <QLabel>
#include <QWidget>
#include "ui_KeyScan.h"

class KeyInputWidget : public QWidget {
    Q_OBJECT

public:
    KeyInputWidget(QLabel* from_key_label, QLabel* to_key_label, QPushButton* from_button, QPushButton* to_button, QWidget* parent);
    ~KeyInputWidget();

    enum CaptureMode { None, FromKey, ToKey };
    void onCaptureFromKeyButtonClicked();
    void onCaptureToKeyButtonClicked();
    void setCurrentMode(CaptureMode mode);
    CaptureMode getCurrentMode() const;


    quint32 from_key_value;
    quint32 to_key_value;
    void keyPressEvent(QKeyEvent* event);

private:
    CaptureMode current_mode;
    QLabel* m_from_key_label;
    QLabel* m_to_key_label;
    QPushButton* m_from_button;
    QPushButton* m_to_button;
};

