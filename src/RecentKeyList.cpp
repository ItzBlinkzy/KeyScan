#include "RecentKeyList.h"
#include "KeyScan.h"

void RecentKeyList::add(ValueType value, QHBoxLayout* layout) {
    while (layout->count() >= MAX_SIZE) {
        // remove and delete the oldest widget if reached max_size
        QLayoutItem* item = layout->takeAt(0);
        if (item) {
            delete item->widget();
            delete item;
        }
    }
    // add most recent key pressed
    QPushButton* button = new QPushButton(value);
    //button->setFocusPolicy(Qt::NoFocus);
    layout->addWidget(button);
}

void RecentKeyList::resetRecentKeyList(QHBoxLayout* recent_key_layout) {
    KeyScan::clearLayout(recent_key_layout);
}