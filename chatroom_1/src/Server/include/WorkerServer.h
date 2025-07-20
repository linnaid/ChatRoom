#pragma once

#include "include.h"
#include "connection.h"

class Connection;

class Worker {
public:
    Worker(int id);
    ~Worker();
    void start();
    void make_queue(int cli);
    void Notify(int from_fd, const std::string& msg);
    void join();
private:
    int make_nonblocking(int sockfd);
    void event_loop();
    void hand_io(int cli);
    void clean_client(int cli);

    int _id;
    std::vector<int> cli_fds;
    std::thread _thread;
    std::mutex cli_mutex;
    std::unordered_map<int, std::shared_ptr<Connection>> conn;
    int epoll_worker;
    int pipe_fd[2];
    bool _running;
};