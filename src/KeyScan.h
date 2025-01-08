#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <QString>
#include <array>
#include <QPushButton>
#include <QLayout>
#include <QtWidgets/QMainWindow>
#include "ui_KeyScan.h"
#include <Windows.h>
#include "qlabel.h"
#include "RecentKeyList.h"
#include "KeyRemapper.h"
#include "KeyInputWidget.h"

// Forward declarations
class RecentKeyList;
class KeyRemapper;
class KeyInputWidget;


class KeyScan : public QMainWindow
{
    Q_OBJECT

public:
    KeyScan(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void modifyButtonStyle(QPushButton* button, QString stylesheet);
    void onMenuKeyTestClicked(bool checked);
    void onMenuTypingTestClicked(bool checked);
    void resetKeyboard(Ui::KeyScanClass* ui);
    static void clearLayout(QLayout* layout);
    static QString KeyNameFromScanCode(const unsigned scanCode);
    static QString KeyNameFromVirtualKeyCode(const unsigned virtualKeyCode);
    quint32 handleModifierKeys(QKeyEvent* event, quint32 virtual_key);
    ~KeyScan();

private:
    Ui::KeyScanClass ui;
    QMap<uint8_t, QPushButton*> buttons;
    RecentKeyList recent_keys;
    KeyRemapper* remapper;
    KeyInputWidget* key_input_widget;
};


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
