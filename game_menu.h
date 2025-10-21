#ifndef _GAME_MENU_H
#define _GAME_MENU_H

#include "game.h"

/*  
    Displays the multi-page tutorial explaining the game rules and navigation.

    input:  
        none  

    output:  
        shows several tutorial pages on screen that the player  
        can navigate using LEFT and RIGHT arrows,  
        and exit using ENTER.  
*/
void show_tutorial();


/*  
    Executes a menu action based on the current menu selection.

    input:  
        position - index of the selected menu item  
        exit - pointer to an integer flag controlling menu termination  

    output:  
        launches the corresponding action:  
            0 - Start new game  
            1 - Load saved game  
            2 - Show tutorial  
            3 - Exit the game  
        returns 1 if execution succeeded,  
        0 if the position is invalid  
*/
int execute(int position, int *exit);


/*  
    Displays and manages the main game menu interface.

    input:  
        none  

    output:  
        continuously renders the main menu with navigation controls,  
        processes user key inputs (UP, DOWN, ENTER),  
        and executes the selected menu action.  
        exits when the user chooses "Exit".  
*/
int game_menu();

#endif /* _GAME_MENU_H */

