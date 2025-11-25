#include"mlv_game_screen.h"

static vector2i mouse_p;


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

void draw_circle_button(int x, int y, int radius, MLV_Color fill_color, char *text) {
    
    MLV_draw_filled_circle(x, y, radius, fill_color);
    
    MLV_draw_circle(x, y, radius, MLV_COLOR_BLACK);

    MLV_draw_filled_circle(x + radius * 3 / 4,
                           y - radius * 3 / 4,
                           radius / 3,
                           MLV_COLOR_WHITE);
    
    MLV_draw_circle(x + radius * 3 / 4,
                    y - radius * 3 / 4,
                    radius / 3,
                    MLV_COLOR_BLACK);

    MLV_draw_text(x + radius * 3 / 5,
                  y - radius + 2,
                  text,
                  MLV_COLOR_BLACK);
}

void display_expand_button(struct game_config *config, vector2i pos) {
    MLV_Color fill_color;
    vector2i btn_p;
    float distance;
    char text[2];

    btn_p = EXPAND_BTN_POS;

    distance = vector2i_get_distance(pos, btn_p);

    fill_color = !config->field->additions_available || distance <= EXPAND_BTN_RADIUS ?
                 MLV_COLOR_GRAY : MLV_COLOR_DEEPSKYBLUE;

    text[0] = '0' + config->field->additions_available;
    text[1] = '\0';


    draw_circle_button(EXPAND_BTN_HORISONTAL_POS,
                       EXPAND_BTN_VERTICAL_POS,
                       EXPAND_BTN_RADIUS,
                       fill_color,
                       text);

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
}

void display_help_button(struct game_config *config, vector2i pos) {
    MLV_Color fill_color;
    vector2i btn_p;
    float distance;
    char text[2];

    btn_p = HELP_BTN_POS;

    distance = vector2i_get_distance(pos, btn_p);

    fill_color = !config->field->hints_available || distance <= HELP_BTN_RADIUS ?
        MLV_COLOR_GRAY : MLV_COLOR_DEEPSKYBLUE;

    text[0] = '0' + config->field->hints_available;
    text[1] = '\0';

    draw_circle_button(HELP_BTN_HORISONTAL_POS,
                       HELP_BTN_VERTICAL_POS,
                       HELP_BTN_RADIUS,
                       fill_color,
                       text);
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

void display_game_scroller(struct game_config *config) {
    float fill_procent;
    int max_shift_height, scrolle_size;

    max_shift_height = config->field->height * CELL_SIZE - GRID_HEIGHT;
    if (max_shift_height <= 0)
        max_shift_height = 1;
    scrolle_size = ((float) (GRID_HEIGHT / CELL_SIZE) / config->field->height) * GRID_HEIGHT;
    if (scrolle_size > GRID_HEIGHT)
        scrolle_size = GRID_HEIGHT;

    fill_procent = (float) config->shift / max_shift_height;

    MLV_draw_rectangle(GAME_PADDING + GRID_WIDTCH + 10, GRID_VERTICAL_POS,
                       20, GRID_HEIGHT,
                       MLV_COLOR_BLACK);

    MLV_draw_filled_rectangle(GAME_PADDING + GRID_WIDTCH + 10, GRID_VERTICAL_POS + (GRID_HEIGHT - scrolle_size + 4) * fill_procent,
                              20                             , scrolle_size,
                              MLV_COLOR_GRAY);
}


void display_mlv_game_screen(struct game_config *config) {
        
    MLV_clear_window(MLV_COLOR_WHITE);

    display_game_grid(config->field, config->shift);
    
    display_game_score(config->field->score);
    
    display_expand_button(config, mouse_p);
    
    display_help_button(config, mouse_p);
    
    display_stage(config);

    display_game_scroller(config);

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
    static MLV_Button_state prev_state = MLV_RELEASED;
    static int last_mouse_y = 0;
    
    MLV_Event event;
    MLV_Button_state curr_state;
    
    int x, y, mouse_on_grid;
    float dist_to_expand_btn, dist_to_help_btn;
    vector2i gridPos;
    game_field *field;

    MATCH_TYPE user_match;

    field = config->field;

    event = MLV_get_event(NULL, NULL, NULL,
                          NULL, NULL,
                          &x, &y, NULL,
                          NULL);

    /* update mouse position */
    if (event == MLV_MOUSE_MOTION) {
        mouse_p.x = x;
        mouse_p.y = y;
    }

    /* get grid cursor position */
    gridPos = create_vector2i(
        (x - GAME_PADDING) / CELL_SIZE,
        (y - GRID_VERTICAL_POS + config->shift) / CELL_SIZE
        );

    /* test if mouse is on grid */
    if (GRID_VERTICAL_POS <= mouse_p.y &&
        mouse_p.y < GRID_VERTICAL_POS + GRID_HEIGHT &&
        mouse_p.x > GAME_PADDING  &&
        gridPos.x >= 0 &&
        gridPos.x < get_game_field_row_size(field, gridPos.y)) {
        mouse_on_grid = 1;
    } else {
        mouse_on_grid = 0;
    }
    
    curr_state = MLV_get_mouse_button_state(MLV_BUTTON_LEFT);
    
    /* click event  */
    if (curr_state == MLV_PRESSED && prev_state == MLV_RELEASED) {

        /* check click on game grid  */
        if (mouse_on_grid) {
            user_match = user_game_select(config);

            if (user_match > 0) {
                show_score_message(mouse_p.x, mouse_p.y, user_match);
            }
        }

        /* check expand btn click */
        dist_to_expand_btn = vector2i_get_distance(mouse_p, EXPAND_BTN_POS);
        if (dist_to_expand_btn <= (float) EXPAND_BTN_RADIUS) {
            expand_game_field(config);
        }

        /* check help btn click */
        dist_to_help_btn = vector2i_get_distance(mouse_p, HELP_BTN_POS);
        if (dist_to_help_btn <= (float) HELP_BTN_RADIUS) {
            show_game_hints(config);
        }

    /* mouse hold event */
    } else if (curr_state == MLV_PRESSED && prev_state == MLV_PRESSED &&
               GRID_VERTICAL_POS <= mouse_p.y && mouse_p.y <= GRID_VERTICAL_POS + GRID_HEIGHT) {

        config->shift += last_mouse_y - mouse_p.y;
        
        if (config->shift < 0)
            config->shift = 0;

        if (config->field->height * CELL_SIZE <= config->shift + GRID_HEIGHT)
            config->shift = config->field->height * CELL_SIZE - GRID_HEIGHT;

    /* mouse motion event  */
    } else if (event == MLV_MOUSE_MOTION) {
        if (mouse_on_grid)
            config->cursor_p = gridPos;
    }

    /* reset game shift if game grid is small */
    if (config->field->height * CELL_SIZE < GRID_HEIGHT)
        config->shift = 0;

    prev_state = curr_state;
    last_mouse_y = mouse_p.y;

    MLV_delay_according_to_frame_rate();
}