#include "User_chat.h"


void UserChat::recv_add_group(const chat::Chat& chat_msg) {
    std::string msg = chat_msg.add_group().msg();
    std::cout << msg << std::endl;
}

void UserChat::print_join_group(const chat::Chat& chat_msg) {
    std::string u_name = chat_msg.join_res().u_name();
    std::string g_name = chat_msg.join_res().g_name();
    std::string name = g_name + "   ---" + u_name;
    if(chat_msg.join_res().decide()) {
        std::cout << "\033[1;32m您已加入群聊" << name << "\033[0m" << std::endl;
        user_groups.insert(name);
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
        user_groups.insert(group);
    }
}

void UserChat::print_group_notify(const chat::Chat& chat_msg) {
    auto groups = chat_msg.group_notify().response();
    if(groups.empty()) {
        std::cout << "\033[34m还没有任何加群申请...\033[0m" << std::endl;
    }
    chat::Chat chat_group;
    for(const auto& [name, value] : groups) {
        
        std::string msg = value;
        if(!Protocol::unpack(msg, chat_group)) {
            break;
        }
        std::string username = chat_group.join_group().username();
        std::string time = chat_group.join_group().time();
        std::cout << "\033[1;32m" << username 
        << "\033[34m" << "   " << name << "\033[0m" << std::endl;

    }
    std::cout << "\033[34m如果要处理申请,输入 6;\033[0m" << std::endl;
    std::cout << "\033[34m如果不处理申请,输入 9;\033[0m" << std::endl;
}
