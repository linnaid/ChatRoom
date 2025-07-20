#include <iostream>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>  // 包含 sockaddr_in 和一些常用的网络函数
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <string.h>
#include <queue>
#include <chrono>
#include <ctime>
#include <cstdint>
#include <sstream>
#include <random>
#include <thread>
#include <mutex>
#include <memory>
#include <unordered_map>




#define NUM 8
#define MAX_EVENT 20
#define PORT 9090
#define MAX_NUM 1024
