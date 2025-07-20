#pragma once
#include <string>

class UserServer {
public:
    std::string GenerateVerifiCode();
    void SendCodeToEmail(const std::string &code, const std::string &email);
};
