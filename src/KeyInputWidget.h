#pragma once
#include <Windows.h>
#include <QKeyEvent>
#include <QLabel>
#include <QWidget>
#include <ui_KeyScan.h>

class KeyInputWidget : public QWidget {
    Q_OBJECT

public:
    explicit KeyInputWidget(Ui::KeyScanClass* ui, QWidget* parent = nullptr);
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
    Ui::KeyScanClass* ui;
};
