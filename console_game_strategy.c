#include"console_game_strategy.h"

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


void user_console_game_move(GAME_KEY key,
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

void user_console_game_input(vector2i *cursor,
                             vector2i *selected_pos,
                             game_field *field) {
    GAME_KEY key;
    
    key = get_game_key();

    if (key & ARROW_KEY) {
        user_console_game_move(key, cursor, field);
    } else {
        switch (key) {
        case ENTER:
            user_game_select(cursor, selected_pos, field);
            break;
        case ADD_LINE:
            expand_game_field(field);
            break;
        case HELP:
            show_game_hints(field);
            break;
        default:
            break;
        }
    }
}

void display_console_available_numbers(game_field *field) {
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

void display_console_game_screen(game_field *field) {
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
    
    display_console_available_numbers(field);

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

void end_console_game_message(game_field *field) {
    display_console_game_screen(field);

    print_over("GAME OVER !!!", create_vector2i(6, 9));

    printf("Type any key for continue...\n");
    
    get_key();
}

void show_console_game_tutorial() {
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

        printf("\n===============================================================");
        printf("\n████████╗██╗   ██╗████████╗ ██████╗ ██████╗ ██╗ █████╗ ██╗     ");
        printf("\n╚══██╔══╝██║   ██║╚══██╔══╝██╔═══██╗██╔══██╗██║██╔══██╗██║     ");
        printf("\n   ██║   ██║   ██║   ██║   ██║   ██║██████╔╝██║███████║██║     ");
        printf("\n   ██║   ██║   ██║   ██║   ██║   ██║██╔══██╗██║██╔══██║██║     ");
        printf("\n   ██║   ╚██████╔╝   ██║   ╚██████╔╝██║  ██║██║██║  ██║███████╗");
        printf("\n   ╚═╝    ╚═════╝    ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚══════╝");
        printf("\n===============================================================");
        printf("\n%s", tutorial_texts[page]);
        printf("\n===============================================================");
        printf("\nPage %d/%d", page + 1, n);
        printf("\nFlèches GAUCHE et DROIT pour naviguer.\nENTRER pour retourner au menu.");
        printf("\n===============================================================");
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

int execute_cosnole_game_action(int position, int *exit) {
    int res = 1;
    switch (position) {
    case 0:
        if (system("clear") != 0)
            printf("Error while console clearing\n");
        start_game();
        break;
    case 1:
        load_game();
        break;
    case 2:
        show_console_game_tutorial();
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


int show_console_game_menu() {
    const char *items[] = { "New Game", "Load Game", "Tutorial", "Exit" };
    const int n = 4;
    int sel = 0;
    GAME_KEY key;
    int i;
    int exit = 0;

    while (!exit) {
        if (system("clear") != 0)
            printf("Error while console clearing\n");
        printf("\n================================================================");
        printf("\n     ███╗   ██╗██╗   ██╗███╗   ███╗██████╗ ███████╗██████╗ ");
        printf("\n     ████╗  ██║██║   ██║████╗ ████║██╔══██╗██╔════╝██╔══██╗");
        printf("\n     ██╔██╗ ██║██║   ██║██╔████╔██║██████╔╝█████╗  ██████╔╝");
        printf("\n     ██║╚██╗██║██║   ██║██║╚██╔╝██║██╔══██╗██╔══╝  ██╔══██╗");
        printf("\n     ██║ ╚████║╚██████╔╝██║ ╚═╝ ██║██████╔╝███████╗██║  ██║");
        printf("\n     ╚═╝  ╚═══╝ ╚═════╝ ╚═╝     ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝");
        printf("\n          ███╗   ███╗ █████╗ ████████╗ ██████╗██╗  ██╗");
        printf("\n          ████╗ ████║██╔══██╗╚══██╔══╝██╔════╝██║  ██║");
        printf("\n          ██╔████╔██║███████║   ██║   ██║     ███████║");
        printf("\n          ██║╚██╔╝██║██╔══██║   ██║   ██║     ██╔══██║");
        printf("\n          ██║ ╚═╝ ██║██║  ██║   ██║   ╚██████╗██║  ██║");
        printf("\n          ╚═╝     ╚═╝╚═╝  ╚═╝   ╚═╝    ╚═════╝╚═╝  ╚═╝");
        printf("\n================================================================");
        for (i = 0; i < n; i++) {
            if (i == sel) printf("\n >%s<", items[i]);
            else printf("\n  %s", items[i]);
        }
        printf("\n================================================================");
        printf("\nFLECHES HAUT et BAS pour naviguer.\nENTRER pour utiliser.");
        printf("\n================================================================");
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
            execute_cosnole_game_action(sel, &exit);
            break;
        default:
            break;
        }
    }
    printf("\n ██████╗  ██████╗  ██████╗ ██████╗ ██████╗ ██╗   ██╗███████╗██╗");
    printf("\n██╔════╝ ██╔═══██╗██╔═══██╗██╔══██╗██╔══██╗╚██╗ ██╔╝██╔════╝██║");
    printf("\n██║  ███╗██║   ██║██║   ██║██║  ██║██████╔╝ ╚████╔╝ █████╗  ██║");
    printf("\n██║   ██║██║   ██║██║   ██║██║  ██║██╔══██╗  ╚██╔╝  ██╔══╝  ╚═╝");
    printf("\n╚██████╔╝╚██████╔╝╚██████╔╝██████╔╝██████╔╝   ██║   ███████╗██╗");
    printf("\n ╚═════╝  ╚═════╝  ╚═════╝ ╚═════╝ ╚═════╝    ╚═╝   ╚══════╝╚═╝\n");
    return 0;
}


void show_console_game_message(const char *text) {
    int str_len = strlen(text);
    vector2i draw_pos;

    draw_pos.x = 12 - str_len / 2;
    draw_pos.x = draw_pos.x < 0 ? 0 : draw_pos.x;
    draw_pos.y = 9;

    print_over(text, draw_pos);
    get_key();
}