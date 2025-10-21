#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_menu.h"

void show_tutorial() {
    const char *tutorial_texts[] = {
        "Bienvenue dans Number Match!",
        "Le but est d'effacer tous les chiffres du plateau.  \n\nTrouvez des paires de nombres égaux (1 et 1, 7 et 7).  \n\nOu des paires dont la somme est égale à 10 (6 et 4).  \n\nSélectionnez les nombres un par un pour les rayer.  \n\nChaque paire trouvée vous fait gagner des points.  \n\nEffacez tout le plateau pour remporter la partie !  ",
        "S’il ne reste plus de chiffres sur une ligne,  \n\nelle sera supprimée et le reste fusionné.  \n\nVérifiez toutes les directions possibles :  \n\nles paires peuvent être horizontales,  \n\nverticales ou même diagonales.  \n\nCherchez les chiffres séparés par des cases :  \n\nles diagonales opposées peuvent former paires.  \n\nVérifiez ligne par ligne : la fin d’une ligne  \n\net le début de la suivante peuvent cacher paires.  ",
        "Bonne chance et amusez-vous bien!"
    };
    int n = 4;
    int page = 0;
    GAME_KEY key;
    int done = 0;

    while (!done) {
        if (system("clear") != 0)
            printf("Error while console clearing\n");

        printf("\n================================================================\n████████╗██╗   ██╗████████╗ ██████╗ ██████╗ ██╗ █████╗ ██╗     \n╚══██╔══╝██║   ██║╚══██╔══╝██╔═══██╗██╔══██╗██║██╔══██╗██║     \n   ██║   ██║   ██║   ██║   ██║   ██║██████╔╝██║███████║██║     \n   ██║   ██║   ██║   ██║   ██║   ██║██╔══██╗██║██╔══██║██║     \n   ██║   ╚██████╔╝   ██║   ╚██████╔╝██║  ██║██║██║  ██║███████╗\n   ╚═╝    ╚═════╝    ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚══════╝\n================================================================\n");
        printf("%s\n", tutorial_texts[page]);
        printf("\n================================================================\n");
        printf("Page %d/%d\n", page + 1, n);
        printf("          ███████          ███████        \n      ████████                ████████\n   █████████████████   ██████████████████ Pour naviguer\n      ████████                ████████    \n          ███████          ███████\n\n ENTRER pour retourner au menu");
        printf("\n================================================================\n");
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
        printf("\n================================================================\n     ███╗   ██╗██╗   ██╗███╗   ███╗██████╗ ███████╗██████╗ \n     ████╗  ██║██║   ██║████╗ ████║██╔══██╗██╔════╝██╔══██╗\n     ██╔██╗ ██║██║   ██║██╔████╔██║██████╔╝█████╗  ██████╔╝\n     ██║╚██╗██║██║   ██║██║╚██╔╝██║██╔══██╗██╔══╝  ██╔══██╗\n     ██║ ╚████║╚██████╔╝██║ ╚═╝ ██║██████╔╝███████╗██║  ██║\n     ╚═╝  ╚═══╝ ╚═════╝ ╚═╝     ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝\n          ███╗   ███╗ █████╗ ████████╗ ██████╗██╗  ██╗\n          ████╗ ████║██╔══██╗╚══██╔══╝██╔════╝██║  ██║\n          ██╔████╔██║███████║   ██║   ██║     ███████║\n          ██║╚██╔╝██║██╔══██║   ██║   ██║     ██╔══██║\n          ██║ ╚═╝ ██║██║  ██║   ██║   ╚██████╗██║  ██║\n          ╚═╝     ╚═╝╚═╝  ╚═╝   ╚═╝    ╚═════╝╚═╝  ╚═╝\n================================================================\n");
        for (i = 0; i < n; i++) {
            if (i == sel) printf(" >%s<\n", items[i]);
            else printf("  %s\n", items[i]);
        }
        printf("\n================================================================\n              ████▒▒              ▒▒▒▒\n             ██████▒▒            ████▒▒\n            ████████▒▒           ████▒▒\nUtiliser   ██████████▒▒     ████████████▒▒  pour naviguer!\n          ████████████▒▒     ██████████▒▒\n              ████▒▒          ████████▒▒\n              ████▒▒           ██████▒▒\n               ▒▒▒▒             ████▒▒\n================================================================\n");
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
    printf(" ██████╗  ██████╗  ██████╗ ██████╗ ██████╗ ██╗   ██╗███████╗██╗\n██╔════╝ ██╔═══██╗██╔═══██╗██╔══██╗██╔══██╗╚██╗ ██╔╝██╔════╝██║\n██║  ███╗██║   ██║██║   ██║██║  ██║██████╔╝ ╚████╔╝ █████╗  ██║\n██║   ██║██║   ██║██║   ██║██║  ██║██╔══██╗  ╚██╔╝  ██╔══╝  ╚═╝\n╚██████╔╝╚██████╔╝╚██████╔╝██████╔╝██████╔╝   ██║   ███████╗██╗\n ╚═════╝  ╚═════╝  ╚═════╝ ╚═════╝ ╚═════╝    ╚═╝   ╚══════╝╚═╝\n");
    return 0;
}
