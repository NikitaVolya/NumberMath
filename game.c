#include<termios.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

#include"game.h"

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

    while (field->height > 0) remove_game_field_row(field, 0);
    
    add_values_game_field(field, values, NUMBERS);
}

void user_game_select(vector2i *cursor,
                      vector2i *selected_pos,
                      game_field *field) {
    MATCH_TYPE match_res;
    
    if (selected_pos->x != -1) {
        set_selection_game_field_cell(field, *selected_pos, 0);

        match_res = check_match(field, *selected_pos, *cursor);
        if (cursor->x == selected_pos->x && cursor->y == selected_pos->y) {
            selected_pos->x = -1;
        } else if (match_res) {
            set_available_game_field_cell(field, *selected_pos, 0);
            set_available_game_field_cell(field, *cursor, 0);

            if (check_game_row_is_clear(field, cursor->y)) {
                remove_game_field_row(field, cursor->y);
                field->score += CLEAR_LINE_MATCH;
                
                if (selected_pos->y >= cursor->y)
                    selected_pos->y--;
                
                cursor->y--;
            }
            
            if (check_game_row_is_clear(field, selected_pos->y)) {
                remove_game_field_row(field, selected_pos->y);
                field->score += CLEAR_LINE_MATCH;
                cursor->y--;
            }

            if (cursor->y < 0) cursor->y = 0;

            selected_pos->x = -1;

            field->score += match_res;
            serialize_game_field(field, "save.bin");
        } else {
            *selected_pos = *cursor;
            set_selection_game_field_cell(field, *selected_pos, 1);         
        }
    } else {
        *selected_pos = *cursor;
        set_selection_game_field_cell(field, *selected_pos, 1);
    }
}

void user_game_move(GAME_KEY key,
                    vector2i *cursor,
                    game_field *field) {
    int row_size = row_size = get_game_field_row_size(field, cursor->y);
    
    switch (key) {
    case RIGHT:
        if (cursor->x < row_size - 1)
            cursor->x++;
        else
            cursor->x = 0;
        break;
    case LEFT:
        if (cursor->x > 0)
            cursor->x--;
        else
            cursor->x = row_size - 1;
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
    default:
        break;
    }
}

void user_game_input(vector2i *cursor,
                     vector2i *selected_pos,
                     game_field *field) {
    GAME_KEY key;
    vector2i pos1, pos2;
    
    key = get_game_key();

    if (key & ARROW_KEY) {
        user_game_move(key, cursor, field);
    } else {
        switch (key) {
        case ENTER:
            user_game_select(cursor, selected_pos, field);
            break;
        case ADD_LINE:
            if (field->additions_available > 0) {
                expand_game_field(field);
                field->additions_available--;
            } else {
                print_over("No addditions available", create_vector2i(0, 9));
                get_key();
            }
            serialize_game_field(field, "save.bin");
            break;
        case HELP:
            if (field->hints_available <= 0) {
                print_over("No hints available ", create_vector2i(3, 9));
                get_key();
            } else if (find_match(field, &pos1, &pos2)) {
                set_highlight_game_field_cell(field, pos1, 1);
                set_highlight_game_field_cell(field, pos2, 1);
                field->hints_available--;
            } else {
                print_over("No match finded", create_vector2i(5, 9));
                get_key();
            }
            serialize_game_field(field, "save.bin");
            break;
        default:
            break;
        }
    }
}

void display_available_numbers(game_field *field) {
    int i, x, y, numbers[10];
    field_cell cell;
    
    for (i = 1; i < 10; i++)
        numbers[i] = 0;

    for (i = 0; i < field->count; i++) {
        x = i % field->width;
        y = i / field->width;

        cell = field->table[y][x];
        if (cell.is_available)
            numbers[cell.value] = 1;
    }

    for (i = 1; i < 10; i++) {
        if (numbers[i])
            printf(" %d ", i);
        else
            printf(" - ");
    }
    printf("\n");
}

void display_game_screen(game_field *field) {
    int i;

    if (system("clear") != 0)
        printf("Error while console clearing\n");

    printf("Score: %d\n", field->score);
    printf("Stage: %d\n", field->stage);

    printf("Get help ( %c ) | %d / %d\n", HELP + ('A' - 'a'),
           field->hints_available, field->hints_max);
    printf("Additions ( %c ) | %d / %d\n", ADD_LINE + ('A' - 'a'),
           field->additions_available, field->additions_max);
    
    for (i = 0; i < field->width; i++)
        printf(HORISONTAL_LINE_PATTERN);
    printf("\n");
    
    display_available_numbers(field);

    for (i = 0; i < field->width; i++)
        printf(HORISONTAL_LINE_PATTERN);
    printf("\n");
    
    display_game_field(field);

    for (i = field->height; i < MIN_FIELD_DISPLAY_HEIGHT; i++)
        printf("\n");

    for (i = 0; i < field->width; i++)
        printf(HORISONTAL_LINE_PATTERN);
    printf("\n");
}

void game_cycle(game_field* field) {
    vector2i cursor, selected_pos;

    serialize_game_field(field, "save.bin");
    
    selected_pos.x = -1;

    cursor = create_vector2i(0, 0);

    do {
        
        set_cursor_game_field_cell(field, cursor, 1);
        
        display_game_screen(field);

        set_cursor_game_field_cell(field, cursor, 0);
        
        user_game_input(&cursor, &selected_pos, field);

        if (check_game_field_is_clear(field)) {
            field->stage++;
            field->score += CLEAR_FIELD_MATCH;
            
            field->additions_available = field->additions_max;
            field->hints_available = field->hints_max;
            
            init_game(field);

            serialize_game_field(field, "save.bin");
        }

     } while (!check_game_is_over(field));

    display_game_screen(field);

    print_over("GAME OVER !!!", create_vector2i(6, 9));

    printf("Type any key for continue...\n");
    
    get_key();
    
    free(field);
}

void load_game() {
    game_field *field;

    field = deserialize_game_field("save.bin");
    if (field == NULL) {
        print_over("Error while loading game!", create_vector2i(3, 3));
        get_key();
    } else {
        game_cycle(field);
    }

}

void start_game() {
    
    game_field *field;

    field = create_new_game_field(9);

    init_game(field);

    game_cycle(field);
}