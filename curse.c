#include <ncurses.h>
#include <string.h>

int main() {
    initscr();
    raw();
    noecho();
    printw("Todo\n");
    refresh();

    while (1) {
        char cm[16];
        char exit[16] = "exit";

        echo();
        getstr(cm);

        if (strcmp(cm, exit) == 0) {
            endwin();
            break;
        }

    }

    return 0;
}