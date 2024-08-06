#ifndef WINDOWS_UART_SERIAL_H
#define WINDOWS_UART_SERIAL_H

#include <Windows.h>
#include <cstdint>
#include <string>


typedef void (*serial_data_handler_t)(uint8_t *buffer_ptr, size_t size, void *data_handler_context_ptr);


class Serial {
private:
    serial_data_handler_t _data_handler{};
    void *_data_handler_context_ptr{};
    HANDLE _serial_handle = nullptr;
    DWORD _errors{};
    COMSTAT _status{};
    DCB _dcb = {0};
    COMMTIMEOUTS _timeouts = {0};
//    OVERLAPPED _overlapped = {0};
    DWORD _thread_id = 0;
    HANDLE _thread_handle = nullptr;
    DWORD WINAPI static _task(void *context);
public:
    [[noreturn]] void read_task();
    ~Serial();
    void set_data_handler(serial_data_handler_t data_handler, void *data_handler_context_ptr = nullptr);
    bool connect(const std::string &com_port, uint32_t baud_rate);
    bool write(uint8_t *data_ptr, size_t size);
    void close();
    [[nodiscard]] bool check_connection() const;
};

#endif //WINDOWS_UART_SERIAL_H