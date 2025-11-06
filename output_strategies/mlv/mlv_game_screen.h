
#ifndef _MLV_GAME_SCREEN_H
#define _MLV_GAME_SCREEN_H

#include <MLV/MLV_all.h>
#include"../output_config.h"
#include"mlv_custom_text_animation.h"

void display_game_score(int score);

void select_cell_style(field_cell *cell, MLV_Color *font_color, MLV_Color *background_color);

void display_game_grid(game_field *field);

void display_mlv_game_screen(struct game_config *config);

void user_mlv_game_input(struct game_config* config);

#endif /* _MLV_GAME_SCREEN_H */