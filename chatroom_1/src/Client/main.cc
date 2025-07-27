#include "User_auth_c.h"
#include "User_chat.h"

int main(int argc, char** argv){
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
    UserAuth_c cli_auth(argc, argv);
    cli_auth.init();
    UserChat cli_chat;
    cli_chat.init();
}