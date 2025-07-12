#include "User_auth_s.h"

int main(){
    UserAuth_s ser(8080);
    ser.init();
    ser.run();
}