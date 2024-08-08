//
// Created by Koba on 08.08.2024.
//

#include "abs_view.h"

ABSView::ABSView() {
    timer = new QTimer(this);
    auto test = [=]() {
        this->refreshGuiData();
    };
    connect(timer, &QTimer::timeout, this, test);
}
