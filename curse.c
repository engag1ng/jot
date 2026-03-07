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

int print_line(int start, char *line) {
    for(int j = start; line[j] != '\0'; j++) {
        if (line[j] == ';') {
            return j;
        } else {
            addch(line[j]);
        }                
    }
}

int main(int argc, char* argv[]) {
    char *filename = argv[1];
    char *ext = strrchr(filename, '.');
    if (!ext) {
        return 1;
    } else if (strcmp(ext + 1, "jot") != 0) {
        return 1;
    }
    int row, col;
    int count;
    char **lines = readFile(filename, &count);
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
        for (int j = 0; lines[i][j] != '\0'; j++) {
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
        char *topLine = malloc(longest_row+5);
        if (!topLine) return;

        // Divider line
        topLine[0] = '|';

        for (int i = 1; i < longest_row + 5; i++) {
            topLine[i] = '-';
        }

        topLine[longest_row + 3] = '|';
        topLine[longest_row + 4] = '\0';

        mvprintw(3, 1, "%s", topLine);
        free(topLine);
        topLine = NULL;
        // ---- //

        // Variables
        int first_divider = longest_name+3;
        int start_printing_date = longest_name+5;
        int second_divider = longest_name+16;
        // ---- //

        // Column names
        mvprintw(2, 1, "| Name");
        mvprintw(2, first_divider, "| Date added |");
        // ---- //

        // Content
        for (int i = 0; i < count; i++) {
            move(4+i, 0);
            printw("%d| ", i);
            int last_printed = 0;
            last_printed = print_line(last_printed, lines[i]);
            mvprintw(4+i, first_divider, "| ");
            last_printed = print_line(last_printed+1, lines[i]);
            mvprintw(4+i, second_divider, "| ");
        }
        // ---- //

        mvprintw(row-1, 0, "$ ");

        refresh();
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