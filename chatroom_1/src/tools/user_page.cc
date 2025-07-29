#include "user_page.h"

// const char* purple= "\033[32m";    
// const char* brightMagenta = "\033[37m"; 
// const char* reset = "\033[0m";

void friend_Page::Friend() {
    std::cout << purple<< "┌──────────────────────────────┐\n";
    std::cout << "│" << brightMagenta << "         🧑 用户系统菜单      " << purple<< "│\n";
    std::cout << "├==============================┤\n";
    std::cout << "│" << "                              " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     1. 好友                  " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     2. 群聊                  " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     0. 退出登陆              " << purple<< "│\n";
    std::cout << "│" << "                              " << purple<< "│\n";
    std::cout << "└──────────────────────────────┘" << reset << "\n";
}

void friend_Page::Friend_detil() {
    std::cout << purple<< "┌──────────────────────────────┐\n";
    std::cout << "│" << brightMagenta << "         🧑 好友详细菜单      " << purple<< "│\n";
    std::cout << "├==============================┤\n";
    std::cout << "│" << "                              " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     1. 加好友                " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     2. 私聊                  " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     3. 查看好友列表          " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     4. 删好友                " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     5. 查看申请消息          " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     6. 屏蔽好友              " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     7. 查看历史消息          " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     10. 解除屏蔽             " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     11. 发送文件             " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     12. 接收文件             " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     0. 退出好友界面          " << purple<< "│\n";
    std::cout << "│" << "                              " << purple<< "│\n";
    std::cout << "└──────────────────────────────┘" << reset << "\n";
}

void friend_Page::Group_detil() {
    std::cout << purple<< "┌──────────────────────────────┐\n";
    std::cout << "│" << brightMagenta << "         💡 群聊详细菜单      " << purple<< "│\n";
    std::cout << "├==============================┤\n";
    std::cout << "│" << "                              " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     1. 加群聊                " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     2. 退出群聊              " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     3. 查看群聊列表          " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     4. 退出群聊              " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     5. 查看申请消息          " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     6. 管理群聊              " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     7. 查看历史消息          " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     8. 发送文件            " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     9. 接收文件            " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "     0. 退出群聊界面          " << purple<< "│\n";
    std::cout << "│" << "                              " << purple<< "│\n";
    std::cout << "└──────────────────────────────┘" << reset << "\n";
} 

void auth_Page::Start() {

    std::cout << purple<< "┌──────────────────────────────┐\n";
    std::cout << "│" << brightMagenta << "         🧑 用户系统菜单      " << purple<< "│\n";
    std::cout << "├==============================┤\n";
    std::cout << "│" << "                              " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "        1. 登录               " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "        2. 注册               " << purple<< "│\n";
    std::cout << "│" << brightMagenta << "        0. 退出               " << purple<< "│\n";
    std::cout << "│" << "                              " << purple<< "│\n";
    std::cout << "└──────────────────────────────┘" << reset << "\n";

    std::cout << brightMagenta << "请选择操作 👉：" << reset;
}