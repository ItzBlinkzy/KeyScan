#include "KeyRemapper.h"
#include <QDebug>
#include <QVBoxLayout>
#include "KeyScan.h"
#include "KeyInputWidget.h"

HHOOK KeyRemapper::keyboard_hook = nullptr;
quint32 key_pressed_virtual_code = 0;
KeyRemapper::KeyRemapper(QObject* parent) : QObject(parent) {}
std::unordered_map<quint32, quint32> KeyRemapper::key_map;


KeyRemapper::~KeyRemapper() {
    stopHook();
}   

void KeyRemapper::startHook() {
    if (!keyboard_hook) {
        keyboard_hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
        if (!keyboard_hook) {
            qDebug() << "Failed to install hook";
        }
        else {
            qDebug() << "Hook installed successfully";
        }
    }
}

void KeyRemapper::stopHook() {
    if (isHooked()) {
        UnhookWindowsHookEx(keyboard_hook);
        keyboard_hook = nullptr;
        qDebug() << "Hook removed successfully!";
    }
}
bool KeyRemapper::addKeyMapping(quint32 from_key, quint32 to_key) {
    if (from_key == to_key) {
        qDebug() << "cannot remap to same key";
        return false;
    }

    // cant map current from_key to existing key in the keymap.
    if (key_map.find(from_key) != key_map.end()) {
        qDebug() << "key already exists cannot use again.";
        return false;
    }

    key_map[from_key] = to_key;
    qDebug() << "Key mapping added: " << from_key << "to " << to_key;


    return true;
}

void KeyRemapper::removeKeyMapping(quint32 from_key) {
    key_map.erase(from_key);
    qDebug() << "Key mapping removed for key:" << from_key;
}

void KeyRemapper::clearKeyMappings(QVBoxLayout* bindings_layout) {
    key_map.clear();
    KeyScan::clearLayout(bindings_layout);
    qDebug() << "All key mappings cleared.";
}

std::unordered_map<quint32, quint32> KeyRemapper::getRemappedKeys() {
    return key_map;
}

bool KeyRemapper::isHooked() {
    if (keyboard_hook) return true;
    return false;
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
    return CallNextHookEx(keyboard_hook, nCode, wParam, lParam);
}

bool KeyRemapper::remapKey(quint32 vkCode) {
    auto it = key_map.find(vkCode);
    if (it != key_map.end()) {
        quint32 mappedKey = it->second;
        // simulating the press with the mapped key
        keybd_event(mappedKey, 0, 0, 0); // Key down
        keybd_event(mappedKey, 0, KEYEVENTF_KEYUP, 0); // key up
        return true;
    }
    return false;
}

void KeyRemapper::drawCurrentBinds(QWidget* widget, QLayout* layout) {
    for (auto& [key, value] : getRemappedKeys()) {
        QString to_key = KeyScan::KeyNameFromScanCode(MapVirtualKeyW(key, MAPVK_VK_TO_VSC));
        QString from_key = KeyScan::KeyNameFromScanCode(MapVirtualKeyW(value, MAPVK_VK_TO_VSC));

        QLabel* label = new QLabel(to_key + " -> " + from_key, widget);
        //QPushButton* remove_button = new QPushButton()
        layout->addWidget(label);
    }
}
