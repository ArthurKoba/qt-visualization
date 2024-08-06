#include "serial.h"

#include <iostream>


Serial::~Serial() {
    close();
}

void Serial::set_data_handler(serial_data_handler_t data_handler, void *data_handler_context_ptr) {
    _data_handler = data_handler;
    _data_handler_context_ptr = data_handler_context_ptr;
}

DWORD Serial::_task(void *context) {
    Serial &self = *reinterpret_cast<Serial*>(context);
    self.read_task();
    return 0;
}

bool Serial::connect(const std::string &com_port, uint32_t baud_rate) {

    if (_serial_handle) {
        std::cerr << "The port is already open!" << std::endl;
        return true;
    }

    std:: string port = R"(\\.\)" + com_port;
    _serial_handle = CreateFileA(
            port.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
//    FILE_FLAG_OVERLAPPED,
            nullptr
    );

    if (_serial_handle == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            std::cerr << "Port: " << com_port << " not available!" << std::endl;
        }
        close();
        return false;
    }

    if (not GetCommState(_serial_handle, &_dcb)) {
        std::cerr << "Error getting parameters for port." << std::endl;
        close();
        return false;
    }

    _dcb.BaudRate = baud_rate;
    _dcb.ByteSize = 8;
    _dcb.StopBits = ONESTOPBIT;
    _dcb.Parity = NOPARITY;
    _dcb.fDtrControl = DTR_CONTROL_ENABLE;

    GetCommTimeouts(_serial_handle, &_timeouts);
    _timeouts.ReadIntervalTimeout=10;
    _timeouts.ReadTotalTimeoutMultiplier=_timeouts.ReadTotalTimeoutConstant=0;
    _timeouts.WriteTotalTimeoutMultiplier=_timeouts.WriteTotalTimeoutConstant=0;

    if (not SetCommState(_serial_handle, &_dcb) or not SetCommTimeouts(_serial_handle, &_timeouts)) {
        std::cerr << "Error setting parameters for port." << std::endl;
        close();
        return false;
    }
//    _overlapped.hEvent = CreateEvent(nullptr,false,FALSE,nullptr);
    _thread_handle = CreateThread(
            nullptr,
            0U,
            Serial::_task,
            this,
            0,
            &_thread_id
    );
    return true;
}

[[noreturn]] void Serial::read_task() {
//    DWORD mask = EV_RXCHAR;
//    SetCommMask(_io_handler,EV_RXCHAR);
    DWORD count_read_bytes = 0;
    while (true) {
//        if (WaitCommEvent(_io_handler, &mask, &_overlapped)) {}
        ClearCommError(_serial_handle, &_errors, &_status);
        if (not _status.cbInQue) continue;
        auto *buffer = reinterpret_cast<uint8_t*>(malloc(_status.cbInQue));
        ReadFile(_serial_handle, buffer, _status.cbInQue, &count_read_bytes, nullptr);
        if (count_read_bytes and _data_handler) {
            _data_handler(buffer, count_read_bytes, _data_handler_context_ptr);
        }
        free(buffer);
    }
}


bool Serial::write(uint8_t *data_ptr, size_t size) {
    DWORD bytes_sent;
    if (not WriteFile(_serial_handle, data_ptr, size, &bytes_sent, nullptr)) {
        std::cerr << "There are problems sending data to the port." << std::endl;
        ClearCommError(_serial_handle, &_errors, &_status);
    }
    return bytes_sent;
}

void Serial::close() {
    if (_serial_handle) {
        CloseHandle(_serial_handle);
        _serial_handle = nullptr;
    }
    if (_thread_handle) {
        CloseHandle(_thread_handle);
        _thread_handle = nullptr;
    }
}

bool Serial::check_connection() const {
    return bool(_serial_handle);
}