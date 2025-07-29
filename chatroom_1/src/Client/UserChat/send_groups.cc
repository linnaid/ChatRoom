#include "User_chat.h"


void UserChat::send_add_group() {
    std::string g_name;
    std::cout << "\033[34m👋请输入群聊的名字吧:\033[0m" << std::endl;
    std::getline(std::cin, g_name);
    
    chat::Chat add_f; 
    add_f.set_group(chat::Group::ADDGROUP);
    chat::AddGroup* req = add_f.mutable_add_group();
    req->set_username(_username);
    req->set_group_name(g_name);
    std::string time = Protocol::GetNowTime();
    req->set_time(time);
    std::string msg; 
    add_f.SerializeToString(&msg);
    if(!Send(msg)) {
        std::cout << "\033[31m群聊创建请求发送失败! \033[0m" << std::endl;
    }
}

void UserChat::join_group() {
    std::cout << "\033[34m请输入群聊的名字吧👉:\033[0m" << std::endl;
    std::string g_name;
    getline(std::cin, g_name);
    std::cout << "\033[34m请输入群主的名字吧👉:\033[0m" << std::endl;
    std::string username;
    getline(std::cin, username);
    
    chat::Chat chat_group;
    chat_group.set_group(chat::Group::JOINGROUP);
    chat::JoinGroup* join_group = chat_group.mutable_join_group();
    join_group->set_u_name(username);
    join_group->set_g_name(g_name);
    join_group->set_username(_username);
    std::string msg;
    chat_group.SerializeToString(&msg);
    Send(msg);
}

void UserChat::group_list() {
    chat::Chat chat_group;
    chat_group.set_group(chat::Group::GROUPLIST);
    chat::GroupList* group_list = chat_group.mutable_group_list();
    group_list->set_username(_username);
    std::string group;
    chat_group.SerializeToString(&group);
    
    Send(group);
}
