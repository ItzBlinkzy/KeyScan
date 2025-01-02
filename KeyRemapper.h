#pragma once
#ifndef KEYREMAPPER_H
#define KEYREMAPPER_H

#include <Windows.h>
#include <QObject>
#include <QVBoxLayout>

class KeyRemapper : public QObject {
    Q_OBJECT

public:
    explicit KeyRemapper(QObject* parent = nullptr);
    ~KeyRemapper();

    void startHook();
    void stopHook();

    void addKeyMapping(DWORD fromKey, DWORD toKey);

    void removeKeyMapping(DWORD fromKey);

    void clearKeyMappings(QVBoxLayout* layout);

    bool isHooked();
    std::unordered_map<DWORD, DWORD> getRemappedKeys();

private:
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HHOOK keyboardHook;
    static std::unordered_map<DWORD, DWORD> keyMap;
    static bool remapKey(DWORD vkCode);
};

#endif // KEYREMAPPER_H
