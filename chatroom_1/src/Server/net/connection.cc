#include "connection.h"


Connection::Connection(int fd)
: _fd(fd)
{}

const std::string& Connection::get_username() const{
    return fds[_fd];
}

void Connection::readMessage() {
    char buffer[MAX_NUM];
    ssize_t byte_read;
    while(byte_read  = ::read(_fd, buffer, MAX_NUM) > 0){
        _buffer.append(buffer, byte_read);
    }
    while(1){
        std::string msg = Protocol::unpack(_buffer);
        // 这里存放处理的函数...我还没写所以先不放了-_-...
    }
}

void Connection::send_offline_msg(const chat::Chat& chat) {
    std::string name = chat.off_msg().name();
    std::string username = chat.off_msg().username();
    std::string user;
    if(name > username) {
        user = name +":" + username;
    } else {
        user = username +":" + name;
    }
    std::vector<std::string> off_lines;
    off_lines = _redis.getChatList("Friend", user);
    chat::Chat chat_off;
    chat_off.set_action(chat::Actions::OFFLINEMSG);
    chat::OfflineMSG* off_msg = chat_off.mutable_off_msg();
    for(const auto& it : off_lines) {
        off_msg->add_msg(it);
    }
    std::string str;
    chat_off.SerializeToString(&str);
    str = Protocol::pack(str);
    
    MessageCenter::instance().dispatch(_fd, _fd, str);
}

void Connection::Send_delete_friend(const chat::Chat& chat) {
    std::string from_name = chat.friend_del_req().from_name();
    std::string to_name = chat.friend_del_req().to_name();
    std::string time = Protocol::GetNowTime();

    chat::Chat chat_res;
    chat_res.set_action(chat::Actions::DELFDRES);
    chat::DeleteFriendResponse* del_res = chat_res.mutable_friend_del_res();
    
    if(_user_msg.deleteFriend(from_name, to_name)) {
        del_res->set_result(true);
        
        std::string r = from_name + "\033[31m 已将您删除好友！\033[0m";
        int to_fd = users[to_name];
        if(to_fd == 0) {
            std::unordered_map<std::string, std::string> message;
            message = {
                {r, time}
            };
            _redis.setResKey("Response", to_name, message);
        } else {
            chat::Chat chat_req;
            chat_req.set_action(chat::Actions::DELFDREQ);
            chat::DeleteFriendRequest* del_req = chat_req.mutable_friend_del_req();
            del_req->set_from_name(from_name);
            del_req->set_to_name(to_name);
            del_req->set_time(time);

            std::string d;
            chat_req.SerializeToString(&d);
            d = Protocol::pack(d);
            MessageCenter::instance().dispatch(_fd, to_fd, d);
        }
    } else {
        del_res->set_result(false);
    }
    std::string del_fd;
    if(!chat_res.SerializeToString(&del_fd)) {
        std::cerr << "\033[34mSeria error!\033[0m" << std::endl;
    }
    del_fd = Protocol::pack(del_fd);
    MessageCenter::instance().dispatch(_fd, _fd, del_fd);
    
}

void Connection::send_List(const chat::Chat& chat) {
    std::string username = chat.friends().name();
    std::vector<std::string> friends;
    friends = _user_msg.getFriend(username);
    chat::Chat chat_list;
    chat_list.set_action(chat::Actions::FDLIST);
    chat::FriendLists* lists = chat_list.mutable_friends();
    for(const auto& it : friends) {
        std::cout << it << std::endl;
        int user = users[it];
        if(user != 0) {
            (*lists->mutable_friends())[it] = true;
        } else {
            (*lists->mutable_friends())[it] = false;
        }
    }
    std::string list;
    if(!chat_list.SerializeToString(&list)) {
        std::cerr << "\033[34mSeria error!\033[0m" << std::endl;
    }
    list = Protocol::pack(list);
    MessageCenter::instance().dispatch(_fd, _fd, list);
}

void Connection::Delete_notify(const chat::Chat& chat) {
    std::string from_name = fds[_fd];
    std::string to_name = chat.req().username();
    _redis.deleteHashMember("Friend", from_name, to_name);
}

void Connection::send_user_notify(const chat::Chat& chat) {
    // std::cout << "进入通知" << std::endl;
    std::string username = chat.req().username();
    std::unordered_map<std::string, std::string> result;
    result = _redis.getHash("Friend", username);
    chat::Chat chat_res;
    chat_res.set_action(chat::Actions::USERNTF);
    chat::UserRequest* res = chat_res.mutable_req();
    for(const auto& [key, value] : result) {
        (*res->mutable_result())[key] = value;
    }
    std::string u_res;
    if(!chat_res.SerializeToString(&u_res)) {
        std::cerr << "\033[34mSeria error!\033[0m" << std::endl;
    }
    u_res = Protocol::pack(u_res);
    MessageCenter::instance().dispatch(_fd, _fd, u_res);
}

// void Connection::Send(const std::string& buf, int fd) {
//     std::string msg = Protocol::pack(buf);
//     ssize_t a = send(fd, msg.c_str(), msg.size(), 0);
//     if(a < 0) {
//         std::cerr << "Send Error in User_chat:" << std::strerror(errno) << std::endl;
//     }
// }

void Connection::adduser(const std::string& username) {
    _user_msg.addUser(_fd, username);
    /////// 发一个包让它打印收到的消息
    std::unordered_map<std::string, std::string> result;
    result = _redis.getHash("Response", username);
    chat::Chat chat_res;
    chat_res.set_action(chat::Actions::RESPONSE);
    chat::LoginResponse* response = chat_res.mutable_response();
    for(const auto& [key, value] : result) {
        (*response->mutable_result())[key] = value;
    }
    std::string res;
    if(!chat_res.SerializeToString(&res)) {
        std::cerr << "\033[34mSeria error!\033[0m" << std::endl;
    }
    res = Protocol::pack(res);
    MessageCenter::instance().dispatch(_fd, _fd, res);
    if(!_redis.deleteHash("Response", username)) {
        std::cerr << "\033[34mSet 删除 error!\033[0m" << std::endl;
    }
}

void Connection::addFriend(const chat::Chat &chat)
{
    std::string from_user = chat.friend_res().from_name();
    std::string to_user = chat.friend_res().to_name();
    std::string time = Protocol::GetNowTime();
    std::string msg;
    bool decide;
    if(chat.friend_res().decide()) {
        _user_msg.addFriend(from_user, to_user);
        msg = from_user + "\033[36m" + " 同意了您的好友申请！\033[0m";
        decide = true;
    } else {
        msg =  from_user + "\033[31m 拒绝了您的好友申请...\033[0m";
        decide  = false;
    }

    int to_fd = users[to_user];
    if(to_fd == 0) {
        /////// push进去
        std::unordered_map<std::string, std::string> message;
            message = {
                {msg, time}
            };
        _redis.setResKey("Response", to_user, message);
    }

    std::string str;
    str = Seriafdres(to_user, msg, time, decide);
    str = Protocol::pack(str);
    MessageCenter::instance().dispatch(_fd, to_fd, str);
    // std::string res = Seriafdres(_fd, decide, to_user);
}

void Connection::send_addFriend(const chat::Chat& chat) {
    std::unordered_map<std::string, std::string> message;
    std::string from_name = chat.friend_req().from_username();
    std::string to_name = chat.friend_req().to_username();
    std::string time = chat.friend_req().time();
    message = {
        {from_name, time}
    };
    int to_fd = users[to_name];
    if(to_fd == 0) {
        std::unordered_map<std::string, std::string> res;   
        std::string a = "\033[1;32m" + from_name + "\033[0m\033[33m请求添加你为好友\033[0m";
        res = {
            {a, time}
        };
        _redis.setResKey("Response", to_name, res);
        _redis.setResKey("Friend", to_name, message);
        return;
    }

    chat::Chat chat_req;
    chat::UserRequest* req = chat_req.mutable_req();
    _redis.setResKey("Friend", to_name, message);
    
    std::string msg = Seriafdreq(to_name);
    msg = Protocol::pack(msg);
    MessageCenter::instance().dispatch(_fd, to_fd, msg);
}

void Connection::Send_msg(const chat::Chat& chat) {
    std::lock_guard<std::mutex> lock(_mtx);
    std::string to_name = chat.chat_fd().to_name();
    std::string from_name = chat.chat_fd().from_name();
    std::string time = chat.chat_fd().time();
    std::string msg = chat.chat_fd().message();
    int to_fd = users[to_name];
    if(to_fd == 0) {
        std::string title = "\033[1;32m您有新的未读消息, 来自:" + from_name + "\033[0m";
        std::unordered_map<std::string, std::string> message;
        std::unordered_map<std::string, std::string> messages;
        messages = {
            {msg, time}
        };
        message = {
            {title, time}
        };
        if(!_redis.userHashHexists("Response", "message", from_name))
        _redis.setResKey("Response", to_name, message);
        std::string user;

        if(from_name >to_name) {
            user = from_name +":" + to_name;
            _redis.setChatList("Friend", user, msg);
        } else {
            user = to_name +":" + from_name;
            _redis.setChatList("Friend", user, msg);
        }
        return;
    }
    std::string user;
    
    if(from_name >to_name) {
        user = from_name +":" + to_name;
        _redis.setChatList("Friend", user, msg);
    } else {
        user = to_name +":" + from_name;
        _redis.setChatList("Friend", user, msg);
    }

    std::string res = Seriamsg(to_name, msg, time);
    res = Protocol::pack(res);
    
    MessageCenter::instance().dispatch(_fd, to_fd, res);
}

void Connection::Close()
{
    ::close(_fd);
}

bool Connection::checkHeart() {
    long now_time = getNowTime();
    if((heart_time - now_time) > TIME_OUT) {
        return false;
    } else {
        return true;
    }
}

long Connection::getNowTime() {
    auto now = std::chrono::system_clock::now();

    // duration是"时间段"的意思～
    auto duration = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

    return seconds;
}