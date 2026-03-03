#include <stdio.h>
#include <stdlib.h>

int main() {
    char todo[3][256];
    int j;

    for (j = 0; j < 3; j++) {
        printf("What's your todo: \n");
        fgets(todo[j], sizeof(todo[j]), stdin);
        
        int i;
        for (i = 0; i < j+1; i++) {
            printf("[ ] %s\n", todo[i]);    
        }
    }
    return 0;
}