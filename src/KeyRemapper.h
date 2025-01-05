#pragma once
#ifndef KEYREMAPPER_H
#define KEYREMAPPER_H

#include <Windows.h>
#include <QKeyEvent>
#include <QObject>
#include <QVBoxLayout>
class KeyRemapper : public QObject {
    Q_OBJECT

public:
    explicit KeyRemapper(QObject* parent = nullptr);
    ~KeyRemapper();

    void startHook();
    void stopHook();
    void drawCurrentBinds(QWidget* widget, QLayout* layout);
    bool addKeyMapping(quint32 from_key, quint32 to_key);

    void removeKeyMapping(quint32 from_key);

    void clearKeyMappings(QVBoxLayout* layout);

    bool isHooked();
    std::unordered_map<quint32, quint32> getRemappedKeys();

private:
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HHOOK keyboard_hook;
    static quint32 key_pressed_virtual_code;
    static std::unordered_map<quint32, quint32> key_map;
    static bool remapKey(quint32 vkCode);
    bool is_capturing_key = false;
};

#endif // KEYREMAPPER_H
