#include"mlv_game_screen.h"

static vector2i mouse_p;

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
        if (cell->is_highlited) {
            *font_color = MLV_COLOR_YELLOW;
        }
    }
}

void display_game_grid(game_field *field, int shift) {
    char text[2] = " ";
    int i, j, field_row;
    vector2i field_cell_p;
    MLV_Font *font = MLV_load_font(GAME_FONT_BOLD, 22);
    MLV_Color background_color, font_color;

    field_cell_p = create_vector2i(GAME_PADDING, GRID_VERTICAL_POS - shift);
    
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

    MLV_draw_filled_rectangle(GAME_PADDING, 0,
                              GAME_PADDING + GRID_WIDTCH, GRID_VERTICAL_POS, MLV_COLOR_WHITE);

    MLV_draw_filled_rectangle(GAME_PADDING, GRID_VERTICAL_POS + GRID_HEIGHT,
                              GAME_PADDING + GRID_WIDTCH, GAME_WINDOW_HEIGHT, MLV_COLOR_WHITE);
    
    MLV_draw_rectangle(GAME_PADDING, GRID_VERTICAL_POS,
                       GRID_WIDTCH, GRID_HEIGHT, MLV_COLOR_BLACK);
}

void display_expand_button(struct game_config *config) {
    MLV_Color fill_color;
    vector2i btn_p;
    float distance;
    char text[2];

    btn_p = EXPAND_BTN_POS;

    distance = vector2i_get_distance(mouse_p, btn_p);

    if (!config->field->additions_available || distance <= EXPAND_BTN_RADIUS) {
        fill_color = MLV_COLOR_GRAY;
    } else {
        fill_color = MLV_COLOR_DEEPSKYBLUE;
    }

    text[0] = '0' + config->field->additions_available;
    text[1] = '\0';


    MLV_draw_filled_circle(EXPAND_BTN_HORISONTAL_POS,
                           EXPAND_BTN_VERTICAL_POS,
                           EXPAND_BTN_RADIUS,
                           fill_color);
    
    MLV_draw_circle(EXPAND_BTN_HORISONTAL_POS,
                    EXPAND_BTN_VERTICAL_POS,
                    EXPAND_BTN_RADIUS,
                    MLV_COLOR_BLACK);

    MLV_draw_filled_rectangle(
        EXPAND_BTN_HORISONTAL_POS - EXPAND_BTN_RADIUS + EXPAND_BTN_PADDING,
        EXPAND_BTN_VERTICAL_POS - EXPAND_BTN_PLUS_WIDTH / 2,
        (EXPAND_BTN_RADIUS - EXPAND_BTN_PADDING) * 2,
        EXPAND_BTN_PLUS_WIDTH * 2,
        MLV_COLOR_BLACK
        );

    MLV_draw_filled_rectangle(
        EXPAND_BTN_HORISONTAL_POS - EXPAND_BTN_PLUS_WIDTH / 2,
        EXPAND_BTN_VERTICAL_POS - EXPAND_BTN_RADIUS + EXPAND_BTN_PADDING,
        EXPAND_BTN_PLUS_WIDTH * 2,
        (EXPAND_BTN_RADIUS - EXPAND_BTN_PADDING) * 2,
        MLV_COLOR_BLACK
        );

    MLV_draw_filled_circle(EXPAND_BTN_HORISONTAL_POS + EXPAND_BTN_RADIUS * 3 / 4,
                           EXPAND_BTN_VERTICAL_POS - EXPAND_BTN_RADIUS * 3 / 4,
                           EXPAND_BTN_RADIUS / 3,
                           MLV_COLOR_WHITE);
    
    MLV_draw_circle(EXPAND_BTN_HORISONTAL_POS + EXPAND_BTN_RADIUS * 3 / 4,
                     EXPAND_BTN_VERTICAL_POS - EXPAND_BTN_RADIUS * 3 / 4,
                     EXPAND_BTN_RADIUS / 3,
                     MLV_COLOR_BLACK);

    MLV_draw_text(EXPAND_BTN_HORISONTAL_POS + EXPAND_BTN_RADIUS * 3 / 5,
                  EXPAND_BTN_VERTICAL_POS - EXPAND_BTN_RADIUS + 2,
                  text,
                  MLV_COLOR_BLACK);
}

void display_help_button(struct game_config *config) {
    MLV_Color fill_color;
    vector2i btn_p;
    float distance;
    char text[2];

    btn_p = HELP_BTN_POS;

    distance = vector2i_get_distance(mouse_p, btn_p);

    if (!config->field->hints_available || distance <= HELP_BTN_RADIUS) {
        fill_color = MLV_COLOR_GRAY;
    } else {
        fill_color = MLV_COLOR_DEEPSKYBLUE;
    }

    text[0] = '0' + config->field->hints_available;
    text[1] = '\0';


    MLV_draw_filled_circle(HELP_BTN_HORISONTAL_POS,
                           HELP_BTN_VERTICAL_POS,
                           HELP_BTN_RADIUS,
                           fill_color);
    
    MLV_draw_circle(HELP_BTN_HORISONTAL_POS,
                    HELP_BTN_VERTICAL_POS,
                    HELP_BTN_RADIUS,
                    MLV_COLOR_BLACK);


    MLV_draw_filled_circle(HELP_BTN_HORISONTAL_POS + HELP_BTN_RADIUS * 3 / 4,
                           HELP_BTN_VERTICAL_POS - HELP_BTN_RADIUS * 3 / 4,
                           HELP_BTN_RADIUS / 3,
                           MLV_COLOR_WHITE);
    
    MLV_draw_circle(HELP_BTN_HORISONTAL_POS + HELP_BTN_RADIUS * 3 / 4,
                     HELP_BTN_VERTICAL_POS - HELP_BTN_RADIUS * 3 / 4,
                     HELP_BTN_RADIUS / 3,
                     MLV_COLOR_BLACK);

    MLV_draw_text(HELP_BTN_HORISONTAL_POS + HELP_BTN_RADIUS * 3 / 5,
                  HELP_BTN_VERTICAL_POS - HELP_BTN_RADIUS + 2,
                  text,
                  MLV_COLOR_BLACK);
}

void display_stage(struct game_config *config) {
    MLV_Font *font;
    char text[11];

    font = MLV_load_font(GAME_FONT_BOLD, 16);

    strcpy(text, "stage: ");
    itos(text + 7, config->field->stage, 4);
    
    MLV_draw_text_with_font(GAME_PADDING, GAME_PADDING, text, font, MLV_COLOR_BLACK);
    
    MLV_free_font(font);
}


void display_mlv_game_screen(struct game_config *config) {
        
    MLV_clear_window(MLV_COLOR_WHITE);

    display_game_grid(config->field, config->shift);
    
    display_game_score(config->field->score);
    
    display_expand_button(config);
    display_help_button(config);
    
    display_stage(config);

    MLV_draw_ctext_animations();
        
    MLV_actualise_window();

}

void show_score_message(int x, int y, int score) {
    vector2i start, end;
    char text[5];

    start = create_vector2i(x, y);
    end = create_vector2i(x, y - CELL_SIZE);

    itos(text, score, 5);

    MLV_create_ctext_animation(text, 20, 500, start, end, CTEXT_ANIMATION_EASEOUT);
}

void user_mlv_game_input(struct game_config* config) {
    MLV_Event event;
    
    MLV_Button_state curr_state;
    static MLV_Button_state prev_state = MLV_RELEASED;
    static int last_mouse_y = 0;
    
    int x = -1, y = -1, mouse_on_grid;
    float dist_to_expand_btn, dist_to_help_btn;
    vector2i gridPos;
    game_field *field;

    MATCH_TYPE user_match;

    
    field = config->field;

    event = MLV_get_event(NULL, NULL, NULL,
                          NULL, NULL,
                          &x, &y, NULL,
                          NULL);

    if (event == MLV_MOUSE_MOTION) {
        mouse_p.x = x;
        mouse_p.y = y;
    }

    gridPos = create_vector2i(
        (x - GAME_PADDING) / CELL_SIZE,
        (y - GRID_VERTICAL_POS + config->shift) / CELL_SIZE
        );

    if (GRID_VERTICAL_POS <= mouse_p.y && mouse_p.y < GRID_VERTICAL_POS + GRID_HEIGHT &&
        gridPos.y >= 0 && gridPos.y < field->height &&
        gridPos.x >= 0 && gridPos.x < get_game_field_row_size(field, gridPos.y)) {
        mouse_on_grid = 1;
    } else {
        mouse_on_grid = 0;
    }
    
    curr_state = MLV_get_mouse_button_state(MLV_BUTTON_LEFT);
    if (curr_state == MLV_PRESSED && prev_state == MLV_RELEASED) {

        if (mouse_on_grid) {
            user_match = user_game_select(config);

            if (user_match > 0) {
                show_score_message(mouse_p.x, mouse_p.y, user_match);
            }
        }

        dist_to_expand_btn = vector2i_get_distance(mouse_p, EXPAND_BTN_POS);
        if (dist_to_expand_btn <= (float) EXPAND_BTN_RADIUS) {
            expand_game_field(config);
        }

        dist_to_help_btn = vector2i_get_distance(mouse_p, HELP_BTN_POS);
        if (dist_to_help_btn <= (float) HELP_BTN_RADIUS) {
            show_game_hints(config);
        }
        
    } else if (curr_state == MLV_PRESSED && prev_state == MLV_PRESSED &&
               GRID_VERTICAL_POS <= mouse_p.y && mouse_p.y <= GRID_VERTICAL_POS + GRID_HEIGHT) {

        config->shift += last_mouse_y - mouse_p.y;
        
        if (config->shift < 0)
            config->shift = 0;

        if (config->field->height * CELL_SIZE - GRID_HEIGHT <= config->shift)
            config->shift = config->field->height * CELL_SIZE - GRID_HEIGHT;
    }

    if (config->field->height * CELL_SIZE < GRID_HEIGHT)
        config->shift = 0;

    
    prev_state = curr_state;
    last_mouse_y = mouse_p.y;
    

    if (event == MLV_MOUSE_MOTION) {
        
        if (mouse_on_grid)
            config->cursor_p = gridPos;
    }
    

    MLV_delay_according_to_frame_rate();
}