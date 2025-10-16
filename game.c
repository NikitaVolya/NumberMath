#include<termios.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

#include"game.h"

#define HORISONTAL_LINE_PATTERN "==="
#define MIN_FIELD_DISPLAY_HEIGHT 15


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
    case ENTER:
        res = ENTER;
        break;
    case HELP:
        res = HELP;
        break;
    case ADD_LINE:
        res = ADD_LINE;
        break;
    case 27:
        key = get_key();
        if (key == 91) {
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

void user_game_select(vector2i *cursor,
                      vector2i *selected_pos,
                      game_field *field) {
    if (selected_pos->x != -1) {
        set_selection_game_field_cell(field, *selected_pos, 0);

        if (cursor->x == selected_pos->x && cursor->y == selected_pos->y) {
            selected_pos->x = -1;
        } else if (check_match(field, *selected_pos, *cursor)) {
            set_available_game_field_cell(field, *selected_pos, 0);
            set_available_game_field_cell(field, *cursor, 0);

            if (check_game_row_is_clear(field, cursor->y)) {
                remove_game_field_row(field, cursor->y);
                cursor->y--;
            }
            
            if (selected_pos->y != cursor->y &&
                check_game_row_is_clear(field, selected_pos->y)) {
                remove_game_field_row(field, selected_pos->y);
                cursor->y--;
            }

            if (cursor->y < 0) cursor->y = 0;

            selected_pos->x = -1;
        } else {
            *selected_pos = *cursor;
            set_selection_game_field_cell(field, *selected_pos, 1);         
        }
    } else {
        *selected_pos = *cursor;
        set_selection_game_field_cell(field, *selected_pos, 1);
    }
}

void user_game_input(vector2i *cursor,
                     vector2i *selected_pos,
                     game_field *field) {
    GAME_KEY key;
    int row_size;
    
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
            user_game_select(cursor, selected_pos, field);
            break;
        case NONE: default:
            break;
    }
}

void display_game_screen(game_field *field, vector2i cursor) {
    int i;

    if (system("clear") != 0)
        printf("Error while console clearing\n");

    printf("Score: %d\n", field->score);
    printf("Stage: %d\n", field->stage);

    printf("Get help: %c | Add line: %c\n", HELP + ('A' - 'a'), ADD_LINE + ('A' - 'a'));

    for (i = 0; i < field->width; i++)
        printf(HORISONTAL_LINE_PATTERN);
    printf("\n");
    
    set_cursor_game_field_cell(field, cursor, 1);
    display_game_field(field);
    set_cursor_game_field_cell(field, cursor, 0);

    for (i = field->height; i < MIN_FIELD_DISPLAY_HEIGHT; i++)
        printf("\n");

    for (i = 0; i < field->width; i++)
        printf(HORISONTAL_LINE_PATTERN);
    printf("\n");

}

void start_game() {
    game_field *field = create_new_game_field(9);
    vector2i cursor, selected_pos;
    
    selected_pos.x = -1;

    init_game(field);

    cursor = create_vector2i(0, 0);

    do {
        
        display_game_screen(field, cursor);
        
        user_game_input(&cursor, &selected_pos, field);

     } while (!check_game_is_over(field));

    display_game_screen(field, cursor);
    printf("GAME OVER!!!\n");

    sleep(4);

    printf("Type any key for continue...\n");
    get_game_key();
    
    free(field);
}