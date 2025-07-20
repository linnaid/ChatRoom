#pragma once

#include <unordered_map>
#include <sw/redis++/redis++.h>
#include <string>

class RedisClient{
public:
    explicit RedisClient(const std::string& redis_uri);

    bool saveUser(const std::string& username,
                  const std::unordered_map<std::string, std::string>& user_info);
    
    bool setKeyExpire(const std::string& key, const std::string& value, int expire_time);

    bool userFieldHexists(const std::string& username, const std::string& field);

    std::optional<std::string> userFieldExists(const std::string& username);

    std::optional<std::string> getUserField(const std::string& username, const std::string& field);

    bool setUserGroups(const std::string& username, const std::string& set_name, const std::string& element);

    bool setGroupMembers(const std::string& groupname, const std::string& set_name, const std::string& username);
    
    bool addUserToOnlineLists(const std::string& username);
    bool removeUserToOnlineLists(const std::string& username);


private:
    sw::redis::Redis _redis;
};

