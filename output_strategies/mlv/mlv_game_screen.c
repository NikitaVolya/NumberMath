#include"mlv_game_screen.h"


void itos(char *dest, int value, int count) {
    int i, j, tmp;
    
    if (count <= 0) {
        
    } else if (count == 1) {
        dest[0] = '\0';
    } else if (value == 0) {
        dest[0] = '0';
        dest[1] = '\0';
    } else {
        for (i = 0; i < count - 1 && value > 0; i++) {
            dest[i] = '0' + value % 10;
            value /= 10;
        }
        dest[i] = '\0';
        
        j = 0;
        i -= 1;
        while (j < i) {
            tmp = dest[i];
            dest[i] = dest[j];
            dest[j] = tmp;

            i--;
            j++;
        }
    }
}


void display_game_score(int score) {
    char text[MAX_SCORE_TEXT_SIZE];
    int posX, posY, sizeX, sizeY;
    MLV_Font *font = MLV_load_font(GAME_FONT_BOLD, 26);

    itos(text, score, MAX_SCORE_TEXT_SIZE);

    posX = GAME_PADDING + CELL_SIZE * 3;
    posY = GAME_PADDING;

    sizeX = CELL_SIZE * 3;
    sizeY = GAME_PADDING * 2 / 3;

    MLV_draw_text_box_with_font(posX, posY, sizeX, sizeY,
                                text, font,
                                0, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                                MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    MLV_free_font(font);
}

void select_cell_style(field_cell *cell,
                       MLV_Color *font_color,
                       MLV_Color *background_color) {
    
    *font_color = MLV_COLOR_BLACK;
    *background_color = MLV_COLOR_WHITE;

    if (!cell->is_available) {
        *font_color = MLV_COLOR_DARK_GRAY;
        *background_color = MLV_COLOR_LIGHT_GRAY;
    } else {
        if (cell->is_selected) {
            *font_color = MLV_COLOR_DEEPSKYBLUE;
            *background_color = MLV_rgba(146, 225, 255, 255);
        } else if (cell->is_cursor) {
            *background_color = MLV_rgba(200, 225, 255, 255);
        }
    }
}

void display_game_grid(game_field *field) {
    char text[2] = " ";
    int i, j, field_row;
    vector2i field_cell_p;
    MLV_Font *font = MLV_load_font(GAME_FONT_BOLD, 22);
    MLV_Color background_color, font_color;

    field_cell_p = create_vector2i(GAME_PADDING, GRID_VERTICAL_POS);
    
    MLV_draw_rectangle(field_cell_p.x, field_cell_p.y,
                       GRID_WIDTCH, GRID_HEIGHT, MLV_COLOR_BLACK);
    
    for (j = 0; j < field->height; j++) {

        field_row = get_game_field_row_size(field, j);
        for (i = 0; i < field_row; i++) {

            text[0] = '0' + field->table[j][i].value;

            select_cell_style(field->table[j] + i, &font_color, &background_color);
                
            MLV_draw_text_box_with_font(field_cell_p.x, field_cell_p.y, CELL_SIZE, CELL_SIZE,
                                        text, font,
                                        3, MLV_COLOR_BLACK, font_color, background_color,
                                        MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
            
            field_cell_p.x += CELL_SIZE;
        }

        field_cell_p.x = GAME_PADDING;
        field_cell_p.y += CELL_SIZE;
    }

    MLV_free_font(font);
}


void display_mlv_game_screen(struct game_config *config) {
        
    MLV_clear_window(MLV_COLOR_WHITE);

    display_game_grid(config->field);
    display_game_score(config->field->score);

    MLV_draw_ctext_animations();
        
    MLV_actualise_window();

}

void show_score_message(int x, int y, int score) {
    vector2i start, end;
    char text[5];

    x = GAME_PADDING + x * CELL_SIZE + CELL_SIZE / 2 - 5;
    y = GRID_VERTICAL_POS + y * CELL_SIZE + CELL_SIZE / 2 - 5;

    start = create_vector2i(x, y);
    end = create_vector2i(x, y - CELL_SIZE);

    itos(text, score, 5);

    MLV_create_ctext_animation(text, 20, 500, start, end, CTEXT_ANIMATION_EASEOUT);
}

void user_mlv_game_input(struct game_config* config) {
    MLV_Event event;
    
    MLV_Button_state curr_state;
    static MLV_Button_state prev_state = MLV_RELEASED;
    
    int x = -1, y = -1;
    vector2i gridPos;
    game_field *field;

    MATCH_TYPE user_match;

    field = config->field;

    event = MLV_get_event(NULL, NULL, NULL,
                          NULL, NULL,
                          &x, &y, NULL,
                          NULL);

    gridPos = create_vector2i(
            (x - GAME_PADDING) / CELL_SIZE,
            (y - GRID_VERTICAL_POS) / CELL_SIZE
            );

    if (gridPos.y >= 0 && gridPos.y < field->height &&
        gridPos.x >= 0 && gridPos.x < get_game_field_row_size(field, gridPos.y)) {
        
        if (event == MLV_MOUSE_MOTION) {
            config->cursor_p = gridPos;
        }

        curr_state = MLV_get_mouse_button_state(MLV_BUTTON_LEFT);

        if (curr_state == MLV_PRESSED && prev_state == MLV_RELEASED) {
            user_match = user_game_select(config);

            if (user_match > 0) {
                show_score_message(gridPos.x, gridPos.y, user_match);
            }
        }

        prev_state = curr_state;
    }

    MLV_delay_according_to_frame_rate();
}