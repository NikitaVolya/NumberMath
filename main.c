#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include"game_menu.h"


int main() {
    srand(time(NULL));
    
    game_menu();
    exit(EXIT_SUCCESS);
}