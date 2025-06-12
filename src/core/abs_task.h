
#ifndef QT_VISUALIZATION_ABS_TASK_H
#define QT_VISUALIZATION_ABS_TASK_H

#include <Windows.h>
#include <cstdint>

class ITask {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
};


class AbstractTask: public ITask {
private:
    DWORD _thread_id = 0;
    HANDLE _thread_handle = nullptr;
protected:
    virtual uint64_t _before_run_task();
    virtual uint64_t _task() = 0;
    virtual void _after_close_task() {};
public:
    void stop() override;
    void start() override;
};


#endif //QT_VISUALIZATION_ABS_TASK_H
