#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>

#define MAX_EVENTS 10

// 设置文件描述符为非阻塞
int set_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        return -1;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl");
        return -1;
    }
    return 0;
}

int main() {
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        return -1;
    }

    struct epoll_event ev;
    struct epoll_event events[MAX_EVENTS];

    // 监听标准输入（stdin）
    int stdin_fd = fileno(stdin);
    if (set_non_blocking(stdin_fd) == -1) {
        close(epoll_fd);
        return -1;
    }

    ev.events = EPOLLIN;  // 监听输入事件
    ev.data.fd = stdin_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, stdin_fd, &ev) == -1) {
        perror("epoll_ctl");
        close(epoll_fd);
        return -1;
    }

    // std::cout << "Enter some text (Ctrl+D to exit):" << std::endl;

    while (true) {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1); // 阻塞等待事件
        if (n == -1) {
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < n; i++) {
            if (events[i].events & EPOLLIN) {
                char buffer[512];
                ssize_t bytes_read = read(events[i].data.fd, buffer, sizeof(buffer) - 1);
                if (bytes_read == -1) {
                    if (errno != EAGAIN) {
                        perror("read");
                        break;
                    }
                } else if (bytes_read == 0) {
                    // EOF reached
                    // std::cout << "End of input (Ctrl+D detected)" << std::endl;
                    close(epoll_fd);
                    return 0;
                } else {
                    buffer[bytes_read] = '\0';
                    parse(buffer);
                }
            }
        }
    }

    close(epoll_fd);
    return 0;
}

void parse(std::string input) {
    // tcp 粘包处理 。。。
    parse111(result);
}

void parse111(std::string) {

}