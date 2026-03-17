#include "core/abs_task.h"

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
        TerminateThread(_thread_handle, 0);
        _thread_handle = nullptr;
        _after_close_task();
    }
}

uint64_t AbstractTask::_before_run_task() {
    return 0;
}

void AbstractTask::_sleep(float ms) {
    Sleep(DWORD(ms));
}