#pragma once
#include <Windows.h>
#include <QKeyEvent>
#include <QLabel>
#include <QWidget>

class KeyInputWidget : public QWidget {
    Q_OBJECT

public:
    explicit KeyInputWidget(QWidget* parent = nullptr);
    ~KeyInputWidget();

    enum CaptureMode { None, FromKey, ToKey };
    void onCaptureFromKeyButtonClicked(QLabel* label);
    void onCaptureToKeyButtonClicked(QLabel* label);
    void setCurrentMode(CaptureMode mode);
    CaptureMode getCurrentMode() const;

    QLabel* from_key_label = nullptr;
    QLabel* to_key_label = nullptr;

    quint32 from_key_value;
    quint32 to_key_value;
    void keyPressEvent(QKeyEvent* event);

private:
    CaptureMode current_mode; 
};
