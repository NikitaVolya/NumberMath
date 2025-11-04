#include"mlv_game_screen.h"

void display_game_score(int score) {
    char text[MAX_SCORE_TEXT_SIZE];
    int posX, posY, sizeX, sizeY, i, j;
    char tmp;

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

    MLV_draw_text_box(posX, posY, sizeX, sizeY,
                      text, 0, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                      MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
}

void display_game_cell(field_cell *cell, vector2i pos) {
    char text[2] = " ";

    text[0] = '0' + cell->value;
    MLV_draw_text_box(pos.x, pos.y, CELL_SIZE, CELL_SIZE,
                      text, 1, MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                      MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
}


void display_mlv_game_screen(struct game_config *config) {
    game_field *field;
    int i, j, field_row;
    vector2i field_cell_p;

    field = config->field;

    MLV_create_window("NumberMatch | Game",
                      "NumberMatch",
                      GAME_WINDOW_WIDTCH,
                      GAME_WINDOW_HEIGHT);
        
    MLV_clear_window(MLV_COLOR_WHITE);

    display_game_score(field->score);

    field_cell_p = create_vector2i(GAME_PADDING, GRID_VERTICAL_POS);
    
    MLV_draw_rectangle(field_cell_p.x, field_cell_p.y,
                       CELL_SIZE * 9, GRID_HEIGHT, MLV_COLOR_BLACK);
    for (j = 0; j < field->height; j++) {

        field_row = get_game_field_row_size(field, j);
        for (i = 0; i < field_row; i++) {
            display_game_cell(
                field->table[0] + i,
                field_cell_p
                );
            field_cell_p.x += CELL_SIZE;
        }

        field_cell_p.x = GAME_PADDING;
        field_cell_p.y += CELL_SIZE;
    }
        
    MLV_actualise_window();
        
    MLV_wait_seconds(10);
        
    MLV_free_window();

}