#pragma once

#include "include.h"
#include "RedisClient.h"

extern std::unordered_map<std::string, int> users;
extern std::unordered_map<int, std::string> fds;

class UserManager {
public:
    UserManager();
    
    void addUser(int fd, const std::string& username);

    void removeUser(int cli);

    int get_fd(const std::string& username);

    std::string get_name(int fd);

private:

    RedisClient _redis;
};