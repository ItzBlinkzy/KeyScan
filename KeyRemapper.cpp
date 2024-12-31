#include "KeyRemapper.h"
#include <QDebug>

HHOOK KeyRemapper::keyboardHook = nullptr;

KeyRemapper::KeyRemapper(QObject* parent) : QObject(parent) {}
std::unordered_map<DWORD, DWORD> KeyRemapper::keyMap;


KeyRemapper::~KeyRemapper() {
    stopHook();
}

void KeyRemapper::startHook() {
    if (!keyboardHook) {
        keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
        if (!keyboardHook) {
            qDebug() << "Failed to install hook";
        }
        else {
            qDebug() << "Hook installed successfully";
        }
    }
}

void KeyRemapper::stopHook() {
    if (keyboardHook) {
        UnhookWindowsHookEx(keyboardHook);
        keyboardHook = nullptr;
        qDebug() << "Hook removed successfully!";
    }
}
void KeyRemapper::addKeyMapping(DWORD fromKey, DWORD toKey) {
    keyMap[fromKey] = toKey;
    qDebug() << "Key mapping added:" << fromKey << "to" << toKey;
}

void KeyRemapper::removeKeyMapping(DWORD fromKey) {
    keyMap.erase(fromKey);
    qDebug() << "Key mapping removed for key:" << fromKey;
}

void KeyRemapper::clearKeyMappings() {
    keyMap.clear();
    qDebug() << "All key mappings cleared.";
}

std::unordered_map<DWORD, DWORD> KeyRemapper::getRemappedKeys() {
    return keyMap;
}

// hook on keyboard input to intercept and block keys to remap
LRESULT CALLBACK KeyRemapper::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* kbStruct = (KBDLLHOOKSTRUCT*)lParam;
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            if (remapKey(kbStruct->vkCode)) {
                return 1; // blocking orig key
            }
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

bool KeyRemapper::remapKey(DWORD vkCode) {
    auto it = keyMap.find(vkCode);
    if (it != keyMap.end()) {
        DWORD mappedKey = it->second;
        // simulating the press with the mapped key
        keybd_event(mappedKey, 0, 0, 0); // Key down
        keybd_event(mappedKey, 0, KEYEVENTF_KEYUP, 0); // key up
        return true;
    }
    return false;
}
