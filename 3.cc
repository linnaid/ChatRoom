#include "2.h"
#include <iostream>

int main() {
    UserServer server;
    std::string code = server.GenerateVerifiCode();
    std::cout << "生成验证码: " << code << std::endl;

    std::string recipient;
    std::cout << "请输入接收验证码的邮箱: ";
    std::cin >> recipient;

    server.SendCodeToEmail(code, recipient);

    return 0;
}
