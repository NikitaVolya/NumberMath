#include <stdio.h>
#include <string.h>
#include "game.h" 

int game_menu() {
    const char *items[] = { "New Game", "Load Game", "Exit" };
    const int n = 3;
    int sel = 0;
    char line[32];
    GAME_KEY key;

    while (1) {
        printf("\033[2J\033[H");
        printf("=== GAME MENU ===\n\n");
        for (int i = 0; i < n; ++i) {
            if (i == sel) printf(" > %s <\n", items[i]);
            else printf(" %s\n", items[i]);
        }
        printf("\nUtiliser W et S ou les Fléches haut et bas et puis cliquer sur entrée pour naviguer!\n (q pour quitter)\n");
        printf("> ");
        fflush(stdout);

        key = get_game_key();
        if(key == UP) {
            sel = (sel + n - 1) % n;
        }
        else if(key == DOWN) {
            sel = (sel + 1) % n;
        }
        
        /*
        if (!fgets(line, sizeof line, stdin)) break;
        
        size_t len = strlen(line);
        if (len && line[len-1] == '\n') line[len-1] = '\0';
        
        if (line[0] == 'q' || line[0] == 'Q') break;
        if (line[0] == 'w' || line[0] == 'W') {
            sel = (sel + n - 1) % n;
            continue;
        }
        if (line[0] == 's' || line[0] == 'S') {
            sel = (sel + 1) % n;
            continue;
        }
        */
        /*      
        if ((unsigned char)line[0] == 0x1b && line[1] == '[') {
            if (line[2] == 'A') {
                sel = (sel + n - 1) % n;
                continue;
            } else if (line[2] == 'B') { 
                sel = (sel + 1) % n;
                continue;
            }
        }
        */
    }

    printf("\033[2J\033[H");
    printf("Goodbye!\n");
    return 0;
}

