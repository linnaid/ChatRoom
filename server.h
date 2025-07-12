#pragma once

#include "include.h"

#define NUM 8
#define MAX_EVENT 20

class UserAuth_s{
public:
    UserAuth_s(int port);
    ~UserAuth_s();
    void init();
    void run();
private:
    int make_nonblocking(int sockfd);
    void print(int cli);
    void Try_Send(char* buf);
    void Stdin();
    void accept_cli();

    int _port;
    std::string _ip;
    std::queue<int> _cli;
    int listen_sockfd;
    int _epoll;
};