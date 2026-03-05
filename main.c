#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
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

    int count;
    char **lines = readFile("todo.txt", &count);
    if (lines == NULL) {
        return 1;
    }

    for (int i = 0; i < count; i++) {
        printf("%s", lines[i]);
        free(lines[i]);
    }
    free(lines);
    lines = NULL;

    return 0;
}