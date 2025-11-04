#include"mlv_game_screen.h"

void display_game_score(int score) {
    char text[MAX_SCORE_TEXT_SIZE];
    int posX, posY, sizeX, sizeY, i, j;
    char tmp;
    MLV_Font *font = MLV_load_font(GAME_FONT_BOLD, 26);

    if (score == 0) {
        text[0] = '0';
        text[1] = '\0';
    } else {
        for (i = 0; i < MAX_SCORE_TEXT_SIZE - 1 && score > 0; i++) {
            text[i] = '0' + score % 10;
            score /= 10;
        }
        text[i] = '\0';
        
        j = 0;
        i -= 1;
        while (j < i) {
            tmp = text[i];
            text[i] = text[j];
            text[j] = tmp;

            i--;
            j++;
        }
    }

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
        }

        if (cell->is_cursor || cell->is_selected) {
            *background_color = MLV_rgba(146, 225, 255, 255);
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
                       CELL_SIZE * 9, GRID_HEIGHT, MLV_COLOR_BLACK);
    
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
    
        
    MLV_actualise_window();

}