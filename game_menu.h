#include <stdio.h>
#include <string.h>
#include "game.h" 

int game_menu() {
    const char *items[] = { "New Game", "Load Game", "Exit" };
    const int n = 3;
    int sel = 0;
    GAME_KEY key;
    int i;

    while (1) {
        if (system("clear") != 0)
            printf("Error while console clearing\n");
        printf("=== GAME MENU ===\n\n");
        for (i = 0; i < n; i++) {
            if (i == sel) printf(" > %s <\n", items[i]);
            else printf(" %s\n", items[i]);
        }
        printf("Utiliser les Fléches haut et bas pour naviguer!\n");
        printf("> ");
        fflush(stdout);

        key = get_game_key();
        if(key == UP) {
            sel = (sel + n - 1) % n;
        }
        else if(key == DOWN) {
            sel = (sel + 1) % n;
        }
    }
    printf("Goodbye!\n");
    return 0;
}

