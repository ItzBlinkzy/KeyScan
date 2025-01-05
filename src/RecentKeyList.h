#pragma once
#ifndef RECENTKEYLIST_H
#define RECENTKEYLIST_H

#include <QString>
#include <QHBoxLayout>
#include <QPushButton>

class RecentKeyList {
public:
    static constexpr size_t MAX_SIZE = 5;
    using ValueType = QString;

    // Declare the member functions
    void add(ValueType value, QHBoxLayout* layout);
    void resetRecentKeyList(QHBoxLayout* recent_key_layout);
};

#endif // RECENTKEYLIST_H