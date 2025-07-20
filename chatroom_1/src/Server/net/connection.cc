#include "connection.h"


Connection::Connection(int fd)
: _fd(fd)
{}

const std::string& Connection::get_username() const{
    return fds[_fd];
}

void Connection::readMessage() {
    char buffer[MAX_NUM];
    ssize_t byte_read;
    while(byte_read  = ::read(_fd, buffer, MAX_NUM) > 0){
        _buffer.append(buffer, byte_read);
    }
    while(1){
        std::string msg = Protocol::unpack(_buffer);
        // 这里存放处理的函数...我还没写所以先不放了-_-...
    }
}

void Connection::Close(){
    ::close(_fd);
}

bool Connection::checkHeart() {
    long now_time = getNowTime();
    if((heart_time - now_time) > TIME_OUT) {
        return false;
    } else {
        return true;
    }
}

long Connection::getNowTime() {
    auto now = std::chrono::system_clock::now();

    // duration是"时间段"的意思～
    auto duration = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

    return seconds;
}