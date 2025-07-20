
#include <RedisClient.h>
#include <RedisKeys.h>
#include <iostream>

RedisClient::RedisClient(const std::string& redis_uri) : _redis(redis_uri) {};

bool RedisClient::saveUser(const std::string& username,
                           const std::unordered_map<std::string, std::string>& user_info){
    try {
        std::string key = RedisKey::UserKey(username);
        auto ret = _redis.hset(key, user_info.begin(), user_info.end());
        return ret;
    }
    catch(const sw::redis::Error &e){
        std::cerr << "Redis Error: " << e.what() << std::endl;
        return false;
    }
}

bool RedisClient::setKeyExpire(const std::string& username, const std::string& value, int expire_time){
    try {
        std::string key = RedisKey::UserKey(username);
        _redis.set(key,value, std::chrono::seconds(expire_time));
        return true;
    }
    catch(const sw::redis::Error &e) {
        std::cerr << "Redis Error:" << e.what() << std::endl;
        return false;
    }
}

std::optional<std::string> RedisClient::userFieldExists(const std::string& username){
    try {
        std::string key = RedisKey::UserKey(username);
        auto buf =  _redis.get(key);
        if(buf) return *buf;
        else return nullptr;
    } catch (const sw::redis::Error &e) {
        std::cerr << "Redis Error: " << e.what() << std::endl;
        return nullptr;
    }
}

bool RedisClient::userFieldHexists(const std::string& username, const std::string& field){
    try {
        std::string key = RedisKey::UserKey(username);
        return _redis.hexists(key, field);
    } catch (const sw::redis::Error &e) {
        std::cerr << "Redis Error: " << e.what() << std::endl;
        return false;
    }
}

std::optional<std::string> RedisClient::getUserField(const std::string& username, const std::string& field){
    try {
        std::string key = RedisKey::UserKey(username);
        auto val = _redis.hget(key, field);
        if(val) return *val;
        else return nullptr;
    } catch (const sw::redis::Error &e) {
        std::cerr << "Redis Error: " << e.what() << std::endl;
        return nullptr; 
    }
}

bool RedisClient::setUserGroups(const std::string& username, const std::string& set_name, const std::string& element){
    try {
        std::string key = RedisKey::UserKey(username) + ":" + set_name;
        auto ret = _redis.sadd(key, element);
        return ret > 0;
    } catch(const sw::redis::Error &e) {
        std::cerr << "Redis Error: " << e.what() << std::endl;
        return false; 
    }
}

bool RedisClient::setGroupMembers(const std::string& groupname, const std::string& set_name, const std::string& username){
    try {
        std::string key = RedisKey::GroupKey(groupname) + ":" + set_name;
        auto ret = _redis.sadd(key, username);
        return ret > 0;
    } catch (const sw::redis::Error &e) {
        std::cerr << "Redis Error:" << e.what() << std::endl;
        return false;
    }
}

bool RedisClient::addUserToOnlineLists(const std::string& username){
    try {
        auto ret = _redis.sadd("online_users", username);
        return ret > 0;
    } catch (const sw::redis::Error &e) {
        std::cerr << "Redis Error:" << e.what() << std::endl;
        return false;
    }
}

bool RedisClient::removeUserToOnlineLists(const std::string& username){
    try {
        auto ret = _redis.srem("online_users", username);
        return ret > 0;
    } catch (const sw::redis::Error &e) {
        std::cerr << "Redis Error:" << e.what() << std::endl;
        return false;
    }
}

