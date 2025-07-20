#pragma once

#include "include.h"
#include "user_chat.pb.h"
#include "RedisClient.h"
#include "WorkerServer.h"


class Master{
public:
    Master();
    ~Master();
    void init();
    void run();
    void distribute_worker(int& cli);

private:
    int make_nonblocking(int sockfd);
    void accept_client();

    RedisClient _redis;
    int _port;
    int cli_fd;
    int _worker;
    int epoll_master;
    std::vector<std::unique_ptr<Worker>> workers;
};