#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_menu.h"

void show_tutorial() {
    const char *tutorial_texts[] = {
        "Bienvenue dans le jeu!\n\nUtilisez les fleches pour naviguer dans le menu et ENTER pour selectionner.",
        "Pendant le jeu, vous pouvez bouger votre personnage avec les touches directionnelles.",
        "Collectez des objets et evitez les obstacles pour augmenter votre score.",
        "Bonne chance et amusez-vous bien!"
    };
    int n = 4;
    int page = 0;
    GAME_KEY key;
    int done = 0;

    while (!done) {
        if (system("clear") != 0)
            printf("Error while console clearing\n");

        printf("\n==================== TUTORIAL ====================\n\n");
        printf("%s\n", tutorial_texts[page]);
        printf("\n=================================================\n");
        printf("Page %d/%d\n", page + 1, n);
        printf("Utilisez LEFT / RIGHT pour naviguer, ENTER pour quitter.\n");
        printf("=================================================\n");
        fflush(stdout);

        key = get_game_key();
        switch (key) {
        case LEFT:
            page = (page + n - 1) % n;
            break;
        case RIGHT:
            page = (page + 1) % n;
            break;
        case ENTER:
            done = 1;
            break;
        default:
            break;
        }
    }
}

int execute(int position, int *exit) {
    int res = 1;
    switch (position) {
    case 0:
        if (system("clear") != 0)
            printf("Error while console clearing\n");
        start_game();
        break;
    case 1:
        break;
    case 2:
        show_tutorial();
        break;
    case 3:
        *exit = 1;
        break;
    default:
        res = 0;
        break;
    }
    return res;
}

int game_menu() {
    const char *items[] = { "New Game", "Load Game", "Tutorial", "Exit" };
    const int n = 4;
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
