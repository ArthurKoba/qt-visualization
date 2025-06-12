//
// Created by Koba on 26.05.2025.
//

#include "abs_task.h"

#include <iostream>

void AbstractTask::start() {
    if (_thread_handle) return;
    _thread_handle = CreateThread(
            nullptr,
            0U,
            [] (void *ctx) -> DWORD {
                auto this_ = reinterpret_cast<AbstractTask*>(ctx);
                auto result = this_->_before_run_task();
                if (result == 0) {
                    result = this_->_task();
                }
                this_->_after_close_task();
                return result;
            },
            this,
            0,
            &_thread_id
    );
}


void AbstractTask::stop() {
    if (_thread_handle) {
        std::cout << "Close task" << std::endl;
        TerminateThread(_thread_handle, 0);
        _after_close_task();
        _thread_handle = nullptr;
    }
}

uint64_t AbstractTask::_before_run_task() {
    return 0;
}
