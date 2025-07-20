#pragma once

#include "include.h"
#include "user_auth.pb.h"


#define IP "127.0.0.1"
#define PORT 8080

class UserAuth_c{
public:
    UserAuth_c();
    ~UserAuth_c();
    void init();
private:
    void run();
    std::string Register_u();
    std::string LogIn();
    bool register_Ver();
    bool login_Ver();
    void get_verify(auth::Auth& auth_msg, int type);
    void Parse_u(const std::string& buf);
    void R_check(auth::Auth& auth_msg);
    void L_check(auth::Auth& auth_msg);

    
    int _sockfd;
    int _port;
    bool log_i;
    int a;
    std::string _ip;
    std::string _username;
    sockaddr_in _cli;
};