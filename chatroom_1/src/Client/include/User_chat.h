#pragma once

#include "include.h"
#include "user_chat.pb.h"
#include "User_auth_c.h"
#include "protocol.h"
#include "tools.h"
#include "user_page.h"
#include "Clear.hpp"

enum Actions {
    ADDFRIEND = 1,
    CHATFD = 2,
    FDLIST = 3,
    DELETEFRIEND = 4,
    FINDREQ = 5,
    GROUP = 6,
    ADDFRIENDRES = 7,
    MSG = 8,
    QUITNTY = 9,
    QUIT = 0
};

class friend_Page;
class Clear;

class UserChat {
public:
    UserChat();
    ~UserChat();
    void init();
    

private:
    void runSend();

    void runRecive();

    void main_run();

    void send_login();

    bool Send(const std::string& buf);

    void send_add_friend();

    void recv_add_friend(const chat::Chat& chat_msg);

    void print_add_friend(const chat::Chat& chat_msg);

    void send_delete_friend();

    void send_offline();

    void decide_add_friend();

    void print_friends(const chat::Chat& chat_msg);

    void print_delete(const chat::Chat& chat_msg);

    void printf_delete_req(const chat::Chat& chat_msg);

    void print_response(const chat::Chat& chat_msg);

    void print_msg(const chat::Chat& chat_msg);

    void chat_friend();

    void send_msg(const std::string& to_name);

    void send_List();

    void recive_msg();

    void print_notify(const chat::Chat& chat_msg);

    void joinThread(std::thread& t);

    void handleReq();

    // 这个函数时对处理结果的发送
    void decide_add_friend(const std::string& to_name, int select);
    void delete_add_friend(const std::string& name);

    int _sockfd;
    bool _running;
    bool _running_recv;
    bool _run;
    bool _main_run;
    std::thread t1;
    std::thread t2;
    std::thread main_t1;
    std::thread main_t2;
    int choice;
    Actions action;
    std::string _ip;
    int _port;
    sockaddr_in _fd;
    std::vector<std::string> friend_res;
    std::unordered_set<std::string> friends;
};