
#include<termios.h>
#include<unistd.h>
#include"game_field.h"

#define NUMBERS 51

enum GAME_KEY {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,
    ENTER = 4,
    NONE
};
typedef enum GAME_KEY GAME_KEY;

int get_key() {
    struct termios oldt, newt;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    return ch;
}

GAME_KEY get_game_key() {
    GAME_KEY res;
    int key;

    key = get_key();

    switch(key) {
    case 10:
        res = ENTER;
        break;
    case 27:
        if (get_key() == 91) {
            switch (get_key()) {
            case 'A':
                res = UP;
                break;
            case 'B':
                res = DOWN;
                break;
            case 'C':
                res = RIGHT;
                break;
            case 'D':
                res = LEFT;
                break;
            default:
                res = NONE;
                break;
            }
        } else {
            res = NONE;
        }
        break;
    default:
        res = NONE;
    }
    
    
    return res;
    
}


short randshort(short start, short end) {
    return start + rand() % (end - start + 1);
}

void init_game(game_field *field) {
    short values[NUMBERS];
    int i;

    for(i = 0; i < NUMBERS; i++) {
        values[i] = randshort(1, 9);
    }
    
    add_values_game_field(field, values, NUMBERS);
}

void user_input(vector2i *cursor, vector2i *selected_pos, game_field *field) {
    GAME_KEY key;
    int row_size, select_block;

    select_block = 0;
    
    key = get_game_key();
    switch (key) {
        case RIGHT:
            row_size = get_game_field_row_size(field, cursor->y);
            if (cursor->x < row_size - 1)
                cursor->x++;
            break;
        case LEFT:
            if (cursor->x > 0)
                cursor->x--;
            break;
        case UP:
            if (cursor->y > 0)
                cursor->y--;
            break;
        case DOWN:
            if (cursor->y < field->height - 1 &&
                cursor->x <= get_game_field_row_size(field, cursor->y + 1) - 1)
                cursor->y++;
            break;
        case ENTER:
            if (selected_pos->x != -1) {
                set_selection_game_field_cell(field, *selected_pos, 0);

                if (check_match(field, *selected_pos, *cursor)) {
                    set_available_game_field_cell(field, *selected_pos, 0);
                    set_available_game_field_cell(field, *cursor, 0);

                    if (selected_pos->y == cursor->y && check_game_row_is_clear(field, cursor->y)) {
                        remove_game_field_row(field, cursor->y);
                    } else if (selected_pos->y != cursor->y){
                        if (check_game_row_is_clear(field, cursor->y))
                            remove_game_field_row(field, cursor->y);
                        if (check_game_row_is_clear(field, selected_pos->y))
                            remove_game_field_row(field, selected_pos->y);
                    }

                    selected_pos->x = -1;
                    select_block = 1;
                }
            }
            if (!select_block) {
                *selected_pos = *cursor;
                set_selection_game_field_cell(field, *selected_pos, 1);
            }
            select_block = 0;
        case NONE: default:
            break;
    }
}



void start_game() {
    game_field *field = create_new_game_field(9);
    vector2i cursor, selected_pos;
    
    selected_pos.x = -1;

    printf("Start game\n");

    init_game(field);

    cursor = create_vector2i(0, 0);
    
    display_game_field(field);

    do {
        user_input(&cursor, &selected_pos, field);

        
        if (system("clear") != 0)
           printf("Error while console clearing\n");
        
        set_highlight_game_field_cell(field, cursor, 1);
        display_game_field(field);
        set_highlight_game_field_cell(field, cursor, 0);


     } while (!check_game_is_over(field));
    free(field);
}