#include "User_chat.h"

UserChat::UserChat() 
: _ip(IP),
_sockfd(-1),
_port(PORT2),
_running(true),
_main_run(true),
_running_recv(true),
_run(true)
{}

UserChat::~UserChat() {
    joinThread(main_t1);
    _main_run = false;
    _running_recv = false;
    joinThread(main_t2);
    if(_sockfd >= 0) {
        close(_sockfd);
    }
    google::protobuf::ShutdownProtobufLibrary();
}

void UserChat::init() {
    if(_username == "") return;
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(_sockfd == -1) {
        std::cerr << "Socket Error in User_chat:" << std::strerror(errno) << std::endl;
    }
    _fd.sin_family = AF_INET;
    _fd.sin_port = htons(_port);
    ::inet_pton(AF_INET, _ip.c_str(), &_fd.sin_addr.s_addr);
    if(connect(_sockfd, (sockaddr*)&_fd, sizeof(_fd)) == -1) {
        std::cout << "hello" << std::endl;
    }
    send_login();

    main_t1 = std::thread(&UserChat::runSend, this); 
    main_t2 = std::thread(&UserChat::main_run, this);
}

void UserChat::send_login() {
    chat::Chat login_signal;
    login_signal.set_action(chat::Actions::LOGIN);
    chat::Login* log_in = login_signal.mutable_log();
    log_in->set_username(_username);
    std::string buf;
    login_signal.SerializeToString(&buf);
    
    Send(buf);
}

void UserChat::main_run() {
    while(_main_run) {
        if(_running_recv) {
            runRecive();
        }
    }
}

void UserChat::runRecive() {
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    setsockopt(_sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    while(_running_recv) {
        if(!_running_recv) break;
        char buf[MAX_NUM];
        ssize_t len = recv(_sockfd, buf, MAX_NUM, 0);
        if(!_running_recv) break;
        if(len > 0) {
            msg.append(buf, len);
            chat::Chat chat_msg;
            while(!msg.empty()) {
                // std::cout << "1";
                if(!Protocol::unpack(msg, chat_msg)) break;
                switch (chat_msg.action())
                {
                case chat::Actions::LOGINLIST:
                    save_friends(chat_msg);
                    break;
                case chat::Actions::FRIENDMSG:
                    print_msg(chat_msg);
                    break;
                case chat::Actions::ADDFDREQ:
                    recv_add_friend(chat_msg);
                    break;
                case chat::Actions::ADDFDRES:
                    print_add_friend(chat_msg);
                    break;
                case chat::Actions::USERNTF:
                    print_notify(chat_msg);
                    break;
                case chat::Actions::FDLIST:
                    print_friends(chat_msg);
                    break;
                case chat::Actions::DELFDRES:
                    print_delete(chat_msg);
                    break;
                case chat::Actions::DELFDREQ:
                    printf_delete_req(chat_msg);
                    break;
                case chat::Actions::RESPONSE:
                    print_response(chat_msg);
                    break;
                case chat::Actions::OFFLINEMSG:
                    print_offline_msg(chat_msg);
                    break;
                case chat::Actions::UNLOGIN:
                    print_cancel();
                    break;
                case chat::Actions::BLOCKFRIEND:
                    print_block(chat_msg.block_friend().name());
                    break;
                case chat::Actions::UNBLOCKFRIEND:
                    print_unblock(chat_msg.block_friend().name());
                    break;
                case chat::Actions::SENDFILE:
                    print_send_file(chat_msg);
                    break;
                case chat::Actions::LOOKFILE:
                    print_look_file(chat_msg);
                    break;
                case chat::Actions::ACTION_QUIT:
                    break;
                default:
                    break;
                }
                switch(chat_msg.group()) {
                case chat::Group::ADDGROUP:
                    recv_add_group(chat_msg);
                    break;
                case chat::Group::JOINGROUP:
                    decide_join_group(chat_msg);
                    break;
                case chat::Group::JOINGROUPRESPONSE:
                    print_join_group(chat_msg);
                    break;
                case chat::Group::GROUPLIST:
                    print_group_list(chat_msg);
                    break;
                case chat::Group::FINDREQ:
                    print_group_notify(chat_msg);
                    break;
                case chat::Group::GROUP_QUIT:
                    break;
                default:
                    break;
                }
            }

        } else if (len == 0) {
            break;
        } else if(len == -1) {
            if(errno == EAGAIN || errno == EWOULDBLOCK) {
                // std::cout<< _sockfd << std::endl;
                continue;
            } else {
                // std::cout<< _sockfd << std::endl;
                std::cerr << "Recive error:" << std::strerror(errno) << std::endl;
                break;
            }
        }
    }

    timeout.tv_sec = 0;
    setsockopt(_sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
}


void UserChat::runSend() {
    std::string msg;
    while(_run) {
        // 推出登陆；加好友；聊天；进入群界面（加，建，踢人）
        if(!_run) break;
        friend_Page::Friend();
        std::cin >> choice;
        if(std::cin.fail() || choice > 2 || choice < 0) {
            Clear::clearScreen();
            std::cout << "\033[1;31m输入错误,请重新输入:\033[0m" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        select = static_cast<Select>(choice);
        
        switch (select)
        {
        case Select::FRIEND:
            runFriend();
            break;
        case Select::GROUP:
            runGroup();
            break;
        case Select::QUIT:
            _run = false;
            break;    
        }
    }
}

void UserChat::runGroup() {
    bool _return = true;
    while(_return) {
        std::cin >> choice;
        if(std::cin.fail() || choice > 6 || choice < 0) {
            Clear::clearScreen();
            std::cout << "\033[1;31m输入错误,请重新输入:\033[0m" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        group = static_cast<Group>(choice);
        switch(group) {
        case Group::ADDGROUP:
            send_add_group();
            break;
        case Group::JOINGROUP:
            join_group();
            break;
        case Group::MANAGEGROUP:
            runGroups();
            break;
        case Group::GROUPLIST:
            group_list();
            break;
        case Group::FINDREQ:
            group_request();
            break;
        case Group::DECIDEREQ:
            deicde_group_request();
            break;
        case Group::QUIT:
            _return = false;
            break;
        }
    }
}


void UserChat::runGroups() {
    bool _return = true;
    while(_return) {
        if(!_return) break;
        friend_Page::Group_detil();
        std::cin >> choice;
        if(std::cin.fail() || choice > 10 || choice < 0) {
            Clear::clearScreen();
            std::cout << "\033[1;31m输入错误,请重新输入:\033[0m" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        groups = static_cast<ManageGroups>(choice);
        switch(groups) {
        case ManageGroups::DISBANDGROUP:
            break;
        case ManageGroups::LEAVEGROUP:
            break;
        case ManageGroups::GROUPMEMBERS:
            break;
        case ManageGroups::ADDADMINISTRATOR:
            break;
        case ManageGroups::DELADMINISTRATOR:
            break;
        case ManageGroups::DECIDEADDGROUP:
            break;
        case ManageGroups::CHATMSG:
            break;
        }
    }
}


void UserChat::runFriend() {
    bool _return = true;
    while(_return) {
        if(!_return) break;
        friend_Page::Friend_detil();
        std::cin >> choice;
        if(std::cin.fail() || choice > 13 || choice < 0) {
            Clear::clearScreen();
            std::cout << "\033[1;31m输入错误,请重新输入:\033[0m" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        action = static_cast<Actions>(choice);
        switch (action)
        {
        case Actions::ADDFRIEND:
            send_add_friend();
            break;
        case Actions::CHATFD:
            chat_friend();
            break;
        case Actions::FDLIST:
            send_List();
            break;
        case Actions::FINDREQ:
            handleReq();
            break;
        case Actions::ADDFRIENDRES:
            decide_add_friend();
            break;
        case Actions::DELETEFRIEND:
            send_delete_friend();
            break;
        case Actions::MSG:
            send_offline();
            break;
        case Actions::BLOCKFRIEND:
            block_friend();
            break;
        case Actions::UNBLOCKFRIEND:
            unblock_friend();
            break;
        case Actions::SENDFILE:
            send_file();
            break;
        case Actions::RECVFILE:
            recive_file();
            break;
        case Actions::LOOKFILE:
            look_file();
            break;
        case Actions::QUITNTY:
            Clear::clearScreen();
            break;
        case Actions::QUIT:
            _return = false;
            break;    
        }
    }
}


