#pragma once

#include "include.h"
#include "protocol.h"
#include "User_manager.h"

#define TIME_OUT 40

class Connection {
public:
    Connection(int fd);

    void readMessage();

    void Close();

    const std::string& get_username() const;

    bool checkHeart();

private:
    long getNowTime();

    int _fd;
    std::string _username;
    std::string _buffer;
    long heart_time;
};