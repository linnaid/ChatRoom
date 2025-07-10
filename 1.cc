#include <sw/redis++/redis++.h>
#include <iostream>
using namespace sw::redis;


int main() {
    try {
        // 连接参数：Redis服务器的IP和端口
        Redis redis("tcp://127.0.0.1:6379");

        // 测试连接，设置一个键值对
        redis.set("key", "value");
        redis.del("key");
        auto val = redis.get("key");
        
        if (val) {
            std::cout << "key: " << *val << std::endl;
        } else {
            std::cout << "key not found" << std::endl;
        }

    } catch (const Error &e) {
        std::cerr << "Redis error: " << e.what() << std::endl;
    }

    return 0;
}
