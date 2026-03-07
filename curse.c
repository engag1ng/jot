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

char *multiply_character(char character, int multiplicator) {
    if (multiplicator <= 0) {
        return NULL;
    }

    char *string = malloc(multiplicator+1);
    if (string == NULL) {
        return NULL;
    }

    for (int j=0; j< multiplicator; j++) {
        string[j] = character;
    }
    string[multiplicator] = '\0';
    return string;
}

int main(int argc, char* argv[]) {
    char *filename = argv[1];
    int row, col;
    int count;
    char **lines = readFile("test.txt", &count);
    if (lines == NULL) {
        return 1;
    }

    initscr();
    keypad(stdscr, true);
    raw();
    noecho();

    int longest_name = 0;
    int longest_row = 0;

    for (int i = 0; i < count; i++) {
        if (strlen(lines[i]) > longest_row) {
            longest_row = strlen(lines[i]);
        }
        for (int j = 0; j < strlen(lines[i]); j++) {
            char *ch = &lines[i][j];
            if (*ch == ';') {
                if (j > longest_name) {
                    longest_name = j+1;
                }
            break;
            }
        }
    }
    
    void draw_basic() {
        getmaxyx(stdscr, row, col);
        mvprintw(0, 0, "Jot - %s\n", filename);
        char *topLine = malloc(longest_row+6);
        if (!topLine) return;

        topLine[0] = '|';

        for (int i = 1; i < longest_row + 6; i++) {
            topLine[i] = '-';
        }

        topLine[longest_row + 4] = '|';
        topLine[longest_row + 5] = '\0';

        mvprintw(3, 1, "%s", topLine);

        move(2, 1);
        printw("| Name");
        printw(multiply_character(' ', longest_name-3));
        printw("| Date added");
        printw(multiply_character(' ', longest_row - longest_name-11));
        addch('|');

        for (int i = 0; i < count; i++) {
            move(4+i, 0);
            addch(i+'0');
            printw("| ");
            for(int j = 0; j < strlen(lines[i]); j++) {
                char *ch = &lines[i][j];
                if (*ch == ';') {
                    if (j < longest_name) {
                        printw(multiply_character(' ', longest_name-j));
                    }
                    printw(" | ");
                } else {
                    addch(lines[i][j]);
                }                
            }
        }

        mvprintw(row-1, 0, "$ ");

        refresh();
        free(topLine);
        topLine = NULL;
    }

    while (1) {
        draw_basic();
        char cm[128];
        char exit[128] = "exit";

        echo();
        getstr(cm);

        if (strcmp(cm, exit) == 0) {
            endwin();
            break;
        }

        erase();
    }

    return 0;
}