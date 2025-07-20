#include "User_auth_s.h"

#define PORT1 8080
#define PORT2 9090

int main(){
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    UserAuth_s ser(PORT1);
    ser.init();
    ser.run();
}