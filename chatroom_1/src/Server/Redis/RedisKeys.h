
#pragma once

#include <string>

namespace RedisKey{
    inline std::string UserKey(const std::string& username){
        return "user:" + username;
    }

    inline std::string GroupKey(const std::string& groupname){
        return "group:" + groupname;
    }
    
}