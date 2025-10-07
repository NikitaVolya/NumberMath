#include <stdio.h>
#include <stdlib.h>

void game_menu ()
{
    int i = 0;
    int cursor = 0;
    char liste[][5] = {"Start", "Load", "Exit"};
    int count = 3;
    do {
        for (i = 0; i < count; i++) {
            if (cursor == i) {
                printf(" [%s]\n", liste[i]);
            }
            else {
                printf("%s\n", liste[i]);
            }
        }
        printf("%d \n", (int)getchar());
    }
    while (1);
}