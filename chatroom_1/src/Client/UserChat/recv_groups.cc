#include "User_chat.h"


void UserChat::recv_add_group(const chat::Chat& chat_msg) {
    std::string msg = chat_msg.add_group().msg();
    std::cout << msg << std::endl;
}

void UserChat::print_join_group(const chat::Chat& chat_msg) {
    std::string u_name = chat_msg.join_res().u_name();
    std::string g_name = chat_msg.join_res().g_name();
    if(chat_msg.join_res().decide()) {
        std::cout << "\033[1;32m您已加入群聊" << g_name << " ---" << u_name << "\033[0m" << std::endl;
    } else {
        std::string msg = chat_msg.join_res().msg();
        std::cout << "\033[1;31m❌加入群聊失败\033[0m" << std::endl;
        std::cout << msg << std::endl;
    }
}

void UserChat::decide_join_group(const chat::Chat& chat_msg) {
    std::cout << "\033[33m您收到一条加群申请,可前往申请界面查看并处理\033[0m" << std::endl;    
}

void UserChat::print_group_list(const chat::Chat& chat_msg) {
    auto groups = chat_msg.group_list().groups();
    if(groups.empty()) {
        std::cout << "\033[35m您还没有加入群聊,快去添加吧!\033[0m" << std::endl;
        return;
    }
    for(const auto& group : groups) {
        std::cout << "\033[1;36m" << group << "\033[0m" << std::endl;
    }
}
