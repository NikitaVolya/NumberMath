#include<stdlib.h>
#include<stdio.h>

#include"game_menu.h"

int main() {
    printf("\e[1;1H\e[2J");
    game_menu();
    exit(EXIT_SUCCESS);
}