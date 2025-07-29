#include "connection.h"

Connection::Connection(int fd, UserManager& user_msg)
: _fd(fd),
_user_msg(user_msg),
_redis("tcp://127.0.0.1:6379")
{}

const std::string &Connection::get_username() const
{
    return fds[_fd];
}

bool Connection::readMessage()
{
    char buffer[MAX_NUM];
    ssize_t byte_read;

    while ((byte_read = ::read(_fd, buffer, MAX_NUM)) > 0)
    {
        _buffer.append(buffer, byte_read);
        // std::cout << _buffer << "11111";
    }

    if (byte_read == 0)
    {
        std::cout << "close" << std::endl;
        return false;
    }

    if (byte_read < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
    {
        return false;
    }

    while (1)
    {
        chat::Chat chat;
        if(!Protocol::unpack(_buffer, chat)) break;
        std::cout << chat.action() << std::endl;
        switch (chat.action())
        {

        case chat::Actions::LOGIN:
            adduser(chat.log().username());
            break;
        case chat::Actions::ADDFDREQ:
            send_addFriend(chat);
            break;
        case chat::Actions::ADDFDRES:
            addFriend(chat);
            break;
        case chat::Actions::USERNTF:
            send_user_notify(chat);
            break;
        case chat::Actions::FDLIST:
            send_List(chat);
            break;
        case chat::Actions::FRIENDMSG:
            Send_msg(chat);
            break;
        case chat::Actions::UNUSERNTF:
            Delete_notify(chat);
            break;
        case chat::Actions::DELFDREQ:
            Send_delete_friend(chat);
        case chat::Actions::OFFLINEMSG:
            send_offline_msg(chat);
            break;
        case chat::Actions::GROUPMSG:
            break;
        case chat::Actions::GROUPNTF:
            break;
        case chat::Actions::HEART:
            checkHeart(chat.heart());
            break;
        case chat::Actions::BLOCKFRIEND:
            block(chat);
            break;
        case chat::Actions::UNBLOCKFRIEND:
            unblock(chat);
            break;
        case chat::Actions::SENDFILE:
            send_file(chat);
            break;
        case chat::Actions::RECVFILE:
            recv_file(chat);
        case chat::Actions::LOOKFILE:
            send_file_req(chat);
            break;
        case chat::Actions::ACTION_QUIT:
            break;
        default:
            break;
        }
        
        switch (chat.group()) {
        case chat::Group::ADDGROUP:
            add_group(chat);
            break;
        case chat::Group::JOINGROUP:
            join_group(chat);
            break;
        case chat::Group::GROUPLIST:
            group_list(chat);
            break;
        case chat::Group::FINDREQ:
            group_notify(chat);
            break;
        case chat::Group::GROUP_QUIT:
            break;
        default:
            break;
        }
    }
    return true;
}

// 以下是关于群聊


void Connection::group_notify(const chat::Chat& chat) {
    std::unordered_map<std::string, std::string> notify;
    notify = _redis
}

void Connection::group_list(const chat::Chat& chat) {
    std::string username = chat.group_list().username();
    chat::Chat chat_groups;
    chat_groups.set_group(chat::Group::GROUPLIST);
    chat::GroupList* group_list = chat_groups.mutable_group_list();
    std::vector<std::string> groups;
    groups = _redis.getGroupList(username);
    for(const auto& group_name : groups) {
        group_list->add_groups(group_name);
    }
    std::string buf;
    chat_groups.SerializeToString(&buf);
    buf = Protocol::pack(buf);

    MessageCenter::instance().dispatch(_fd, _fd, buf);
}

void Connection::join_group(const chat::Chat& chat) {
    std::string u_name = chat.join_group().u_name();
    std::string g_name = chat.join_group().g_name();
    std::string username = fds[_fd];
    
    if(!_redis.GroupExists(u_name, g_name)) {
        chat::Chat chat_group;
        std::string msg = "\033[1;31m原因: 群聊不存在\033[0m";
        chat_group.set_group(chat::Group::JOINGROUPRESPONSE);
        chat::JoinGroupResponse* group_res = chat_group.mutable_join_res();
        group_res->set_u_name(u_name);
        group_res->set_g_name(g_name);
        group_res->set_username(username);
        group_res->set_decide(false);
        group_res->set_msg(msg);
        std::string buf;
        chat_group.SerializeToString(&buf);
        buf = Protocol::pack(buf);

        MessageCenter::instance().dispatch(_fd, _fd, buf);
        return;
    }
    
    std::string time = Protocol::GetNowTime();
    std::string uuid = _user_msg.getGroup_uuid(u_name, g_name);
    std::unordered_map<std::string, std::string> managers;
    managers = _redis.getGroupManager(uuid);
    for(const auto& [statue, user] : managers) {
        chat::Chat chat_group;
        chat_group.set_group(chat::Group::JOINGROUP);
        chat::JoinGroup* join_group = chat_group.mutable_join_group();
        join_group->set_u_name(u_name);
        join_group->set_g_name(g_name);
        join_group->set_username(username);
        join_group->set_time(time);
        std::string group;
        chat_group.SerializeToString(&group);
        group = Protocol::pack(group);
        std::unordered_map<std::string, std::string> g_info;
        g_info = {
            {username, group}
        };
        _redis.setGroupNotify(user, g_info);

        int fd = users[user];
        if(fd == 0) {
            std::unordered_map<std::string, std::string> msg;
            std::string a = "\033[33m您收到一条加群申请 ,来自 \033[1;32m" + username + "\033[0m";
            msg = {
                {a, time}
            };
            _redis.setResKey("Response", user, msg);
            continue;
        }
        
        MessageCenter::instance().dispatch(_fd, fd, group);
    }
}

void Connection::add_group(const chat::Chat& chat) {
    std::string g_name = chat.add_group().group_name();
    std::string username = chat.add_group().username();
    std::string time = Protocol::GetNowTime();

    chat::Chat chat_group;
    chat_group.set_group(chat::Group::ADDGROUP);
    chat::AddGroup* add_group = chat_group.mutable_add_group();
    add_group->set_time(time);

    if(_redis.GroupExists(username, g_name)) {
        std::string msg = "\033[31m已创建过此群聊❌\033[0m";
        add_group->set_msg(msg);
    } else {
        std::string uuid = Protocol::generate_uuid();
        std::unordered_map<std::string, std::string> g_info;
        g_info = {
            {"username", username},
            {"groupname", g_name},
            {"ttime",  time}
        };
        _redis.addGroup(uuid, username, g_name, g_info);
        _redis.SetGroupMember(uuid, username);
        _redis.UserSetGroups(username, username, g_name);
        std::unordered_map<std::string, std::string> g;
        g = {
            {"群主", username}
        };
        _redis.SetGroupManager(uuid, g);
        std::string msg = "\033[32m群聊创建成功!\033[0m";
        add_group->set_msg(msg);
    }
    
    std::string group;
    chat_group.SerializeToString(&group);
    group = Protocol::pack(group);

    MessageCenter::instance().dispatch(_fd, _fd, group);
}

// 以下是关于好友以及部分登陆
void Connection::recv_file(const chat::Chat& chat) {
    std::string name = chat.recv_file().name();
    std::string username = fds[_fd];
    _redis.delUserFile(username, name);
}

void Connection::send_file_req(const chat::Chat& chat) {
    std::string username = fds[_fd];
    std::unordered_map<std::string, std::vector<std::string>> files;
    files = _redis.getAllFile(username);
    chat::Chat chat_files;
    chat_files.set_action(chat::Actions::LOOKFILE);
    chat::LookFileRequest* look_files = chat_files.mutable_file_req();
    for(const auto& [user_friend, friend_files] : files) {
        auto& file_lists = (*look_files->mutable_result())[user_friend];
        for(const auto& list : friend_files) {
            file_lists.add_files(list);
        }
    }

    std::string msg;
    chat_files.SerializeToString(&msg);
    msg = Protocol::pack(msg);

    MessageCenter::instance().dispatch(_fd,_fd, msg);
}

void Connection::send_file(const chat::Chat& chat) {
    std::string username = fds[_fd];
    std::string name = chat.send_file().name();
    std::string file_name = chat.send_file().file_name();
    std::unordered_map<std::string, std::string> user_file;
    std::string time = Protocol::GetNowTime();
    user_file = {
        {username, file_name}
    };
    _redis.setUserFile(name, username, file_name);
    int fd = users[name];
    if(fd == 0) {
        std::unordered_map<std::string, std::string> msg;
        std::string a = "\033[34m您收到一个文件 " + file_name + ",来自 " + username + "\033[0m";
        msg = {
            {a, time}
        };
        _redis.setResKey("Response", name, msg);
        
        return;
    }
    chat::Chat chat_file;
    chat_file.set_action(chat::Actions::SENDFILE);
    chat::SendFile* send_file = chat_file.mutable_send_file();
    send_file->set_name(username);
    send_file->set_file_name(file_name);
    send_file->set_time(time);
    std::string msg;
    chat_file.SerializeToString(&msg);
    msg = Protocol::pack(msg);

    MessageCenter::instance().dispatch(_fd, fd, msg);

}

void Connection::unblock(const chat::Chat& chat) {
    std::string username = fds[_fd];
    std::string name = chat.block_friend().name();
    int fd = users[name];
    if(fd == 0) {
        std::unordered_map<std::string, std::string> msg;
        std::string a = username + "\033[32m已将您解除屏蔽\033[0m";
        std::string time = Protocol::GetNowTime();
        msg = {
            {a, time}
        };
        _redis.setResKey("Response", name, msg);
        
        return;
    }
    chat::Chat chat_b;
    chat_b.set_action(chat::Actions::UNBLOCKFRIEND);
    chat::BlockFriend* block = chat_b.mutable_block_friend();
    block->set_name(username);
    std::string msg;
    chat_b.SerializeToString(&msg);
    msg = Protocol::pack(msg);

    MessageCenter::instance().dispatch(_fd, fd, msg);
}

void Connection::block(const chat::Chat& chat) {
    std::string username = fds[_fd];
    std::string name = chat.block_friend().name();
    int fd = users[name];
    if(fd == 0) {
        std::unordered_map<std::string, std::string> msg;
        std::string a = username + "\033[31m已将您屏蔽❌\033[0m";
        std::string time = Protocol::GetNowTime();
        msg = {
            {a, time}
        };
        _redis.setResKey("Response", name, msg);
        
        return;
    }
    chat::Chat chat_b;
    chat_b.set_action(chat::Actions::BLOCKFRIEND);
    chat::BlockFriend* block = chat_b.mutable_block_friend();
    block->set_name(username);
    std::string msg;
    chat_b.SerializeToString(&msg);
    msg = Protocol::pack(msg);

    MessageCenter::instance().dispatch(_fd, fd, msg);
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
        std::string pp;
        if(from_name > to_name) {
            pp = from_name + to_name;
        } else {
            pp = to_name + from_name;
        }
        _redis.deleteChatList("Friend", pp);
    
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

void Connection::adduser(const std::string& username) {

    if(users[username] != 0) {
        chat::Chat chat_cancel;
        chat_cancel.set_action(chat::Actions::UNLOGIN);
        std::string cancel;
        chat_cancel.SerializeToString(&cancel);
        cancel = Protocol::pack(cancel);

        MessageCenter::instance().dispatch(_fd, _fd, cancel);
    }

    _user_msg.addUser(_fd, username);

    // 发它的好友消息
    std::vector<std::string> friends;
    friends = _user_msg.getFriend(username);
    chat::Chat chat_list;
    chat_list.set_action(chat::Actions::LOGINLIST);
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
    msg = from_name + ":" + msg;
    std::string user;
    
    if(from_name >to_name) {
        user = from_name +":" + to_name;
        _redis.setChatList("Friend", user, msg);
    } else {
        user = to_name +":" + from_name;
        _redis.setChatList("Friend", user, msg);
    }

    // if(chat.chat_fd().block()) {
    //     return;
    // }

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

        return;
    }

    std::string res = Seriamsg(to_name, msg, time);
    res = Protocol::pack(res);
    
    MessageCenter::instance().dispatch(_fd, to_fd, res);
}

void Connection::Close()
{
    ::close(_fd);
}

std::string Connection::Seriafdreq(const std::string &to_name)
{
    chat::Chat chat;
    chat.set_action(chat::Actions::ADDFDREQ);
    chat::AddFriendRequest *chat_fd = chat.mutable_friend_req();
    auto it = fds.find(_fd);
    std::string from_name;
    if(it != fds.end()) {
        from_name = it->second;
    } else {
        std::cout << "Not find User" << std::endl;
    }
    // 这里可以价格错误包返回;
    chat_fd->set_from_username(from_name);
    chat_fd->set_to_username(to_name);
    std::string time = Protocol::GetNowTime();
    chat_fd->set_time(time);
    return chat.SerializeAsString();
}

std::string Connection::Seriafdres(const std::string &to_name, 
                                   const std::string& msg, 
                                   const std::string& time,
                                   bool decide)
{
    chat::Chat chat;
    chat.set_action(chat::Actions::ADDFDRES);
    chat::AddFriendResponse *chat_fd = chat.mutable_friend_res();
    std::string from_name = fds[_fd];
    chat_fd->set_from_name(from_name);
    chat_fd->set_to_name(to_name);
    chat_fd->set_time(time);
    chat_fd->set_msg(msg);
    chat_fd->set_decide(decide);
    
    return chat.SerializeAsString();
}

std::string Connection::Seriamsg(const std::string& to_name, 
                                 const std::string &msg, 
                                 const std::string& time)
{
    chat::Chat chat;
    chat.set_action(chat::Actions::FRIENDMSG);
    chat::FriendChatMessage *chat_fd = chat.mutable_chat_fd();
    auto it = fds.find(_fd);
    std::string from_name;
    if(it != fds.end()) {
        from_name = it->second;
    } else {
        std::cout << "Not find User" << std::endl;
    }
    chat_fd->set_from_name(from_name);
    chat_fd->set_to_name(to_name);
    chat_fd->set_message(msg);
    chat_fd->set_time(time);
    return chat.SerializeAsString();
}

void Connection::sendMessage(int from_fd, const std::string &msg)
{
    std::lock_guard<std::mutex> lock(_mtx);
    send(_fd, msg.c_str(), msg.size(), 0);
}

bool Connection::checkHeart(const std::string& heart)
{
    std::lock_guard<std::mutex> lock(t_mtx);
    long now_time = getNowTime();
    long heart_time = std::stol(heart);
    if ((now_time - heart_time) > TIME_OUT)
    {
        Close();
        std::cout << "\033[1;31m心跳检测超时！要关闭连接了！\033[0m" << std::endl;
        return false;
    }
    else
    {
        return true;
    }
}

long Connection::getNowTime()
{
    auto now = std::chrono::system_clock::now();

    // duration是"时间段"的意思～
    auto duration = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

    return seconds;
}