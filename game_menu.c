#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_menu.h"

int execute(int position, int *exit) {
    int res = 1;
    switch (position) {
    case 0:
        if (system("clear") != 0)
            printf("Error while console clearing\n");
        start_game();
        break;
    case 1:
        load_game();
        break;
    case 2:
        *exit = 1;
        break;
    default:
        res = 0;
        break;
    }
    return res;
}

int game_menu() {
    const char *items[] = { "New Game", "Load Game", "Exit" };
    const int n = 3;
    int sel = 0;
    GAME_KEY key;
    int i;
    int exit = 0;

    while (!exit) {
        if (system("clear") != 0)
            printf("Error while console clearing\n");
        printf("\n====================================================\n █▄░█ █░█ █▀▄▀█ █▄▄ █▀▀ █▀█   █▀▄▀█ ▄▀█ ▀█▀ █▀▀ █░█     \n █░▀█ █▄█ █░▀░█ █▄█ ██▄ █▀▄   █░▀░█ █▀█ ░█░ █▄▄ █▀█\n====================================================\n");
        for (i = 0; i < n; i++) {
            if (i == sel) printf(" >%s<\n", items[i]);
            else printf("  %s\n", items[i]);
        }
        printf("\n====================================================\n Utiliser les Fléches haut et bas pour naviguer!\n====================================================\n");
        fflush(stdout);

        key = get_game_key();
        switch (key) {
        case UP:
            sel = (sel + n - 1) % n;
            break;
        case DOWN:
            sel = (sel + 1) % n;
            break;
        case ENTER:
            execute(sel, &exit);
            break;
        default:
            break;
        }
    }
    printf("Goodbye!\n");
    return 0;
}
