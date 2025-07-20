#include "User_manager.h"

UserManager::UserManager()
: _redis("tcp://127.0.0.1:6379")
{}

// 登入用户
void UserManager::addUser(int fd, const std::string& username) {
    _redis.addUserToOnlineLists(username);
    users[username] = fd;
    fds[fd] = username;
}

// 登出用户
void UserManager::removeUser(int cli) {
    std::string username = fds[cli];
    _redis.removeUserToOnlineLists(username);
    users.erase(username);
    fds.erase(cli);
}

int UserManager::get_fd(const std::string& username){
    return users[username];
}

std::string UserManager::get_name(int fd) {
    return fds[fd];
}