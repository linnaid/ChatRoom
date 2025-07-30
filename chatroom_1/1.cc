#include <ncurses.h>

int main() {
    initscr();            // 初始化屏幕
    cbreak();             // 禁用行缓冲
    noecho();             // 输入不回显
    keypad(stdscr, TRUE); // 启用方向键

    mvprintw(2, 10, "Username: ");
    char username[50];
    getstr(username);

    mvprintw(4, 10, "Password: ");
    char password[50];
    getstr(password);

    mvprintw(6, 10, "Press any key to submit...");
    getch();

    endwin(); // 关闭 ncurses 模式

    // 继续用 username 和 password 做验证
    return 0;
}
