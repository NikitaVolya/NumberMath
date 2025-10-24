#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include"console_game_strategy.h"


int main() {
    srand(time(NULL));

    show_console_game_menu();
    exit(EXIT_SUCCESS);
}