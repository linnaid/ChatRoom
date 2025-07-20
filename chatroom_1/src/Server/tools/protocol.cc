#include "protocol.h"

std::string Protocol::pack(const std::string& payload) {
    uint32_t len = htonl(payload.size());
    
    std::string msg;
    msg.resize(4);
    memcpy(&msg[0], &len, 4);

    msg += payload;
    return msg;
}

std::string Protocol::unpack(std::string& buffer){
    while(buffer.size() >= 4){
        uint32_t len = 0;
        memcpy(&len, buffer.data(), 4);
        len = ntohl(len);

        if(buffer.size() < len + 4) break;

        std::string msg = buffer.substr(4, len);
        buffer.erase(0, 4 + len);
        return msg;
    }
    return nullptr;
}

// 心跳检测(ping~ pong~)
bool Protocol::is_heartbeat(const std::string& msg){
    return msg == "ping";
}