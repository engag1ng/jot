#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

char **readFile(const char *filename, int *count) {
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            perror("fopen");
            return NULL;
        }

        char buffer[1024];
        char **lines = NULL;
        *count = 0;

        while (fgets(buffer, sizeof(buffer), file)) {
            char **temp = realloc(lines, (*count + 1) * sizeof(char *));
            if (temp == NULL) {
                for (int i = 0; i < *count; i++) free(lines[i]);
                free(lines);
                fclose(file);
                return NULL;
            }
            lines = temp;
            lines[*count] = strdup(buffer);
            if (lines[*count] == NULL) {
                for (int i = 0; i < *count; i++) free(lines[i]);
                free(lines);
                fclose(file);
                return NULL;
            }
            (*count)++;
        }

        fclose(file);
        return lines;
    }

    int writeFile(const char *filename, const char *data) {
        FILE *file = fopen(filename, "w");
        if (file == NULL) {
            perror("fopen");
            return 1;
        }

        fputs(data, file);
        fclose(file);

        return 0;
    }

int main(int argc, char* argv[]) {
    char *filename = argv[1];
    printf(filename);
    int row, col;

    initscr();
    getmaxyx(stdscr, row, col);
    raw();
    noecho();
    printw("Jot - %s\n", filename);
    refresh();
    mvprintw(row-1, 0, "$ ");
    refresh();

    while (1) {
        char cm[128];
        char exit[128] = "exit";

        echo();
        getstr(cm);

        if (strcmp(cm, exit) == 0) {
            endwin();
            break;
        }

    }

    return 0;
}