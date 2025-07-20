#include "WorkerServer.h"

Worker::Worker(int id)
: epoll_worker(-1),
_id(id)
{}

void Worker::make_queue(int cli){
    if(make_nonblocking(cli) == -1){
        perror("Cli_make_nonblocking error");
        return;
    }
    struct epoll_event ev;
    ev.data.fd = cli;
    ev.events = EPOLLIN;
}

int Worker::make_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL);
    if(flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}