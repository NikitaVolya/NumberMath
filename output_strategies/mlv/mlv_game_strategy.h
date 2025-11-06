
#ifndef _MLV_GAME_STRATEGY_H
#define _MLV_GAME_STRATEGY_H

#define GAME_WINDOW_WIDTCH 520
#define GAME_WINDOW_HEIGHT 680

#define GAME_PADDING ( GAME_WINDOW_WIDTCH / 10 )

#define CELL_SIZE ( ( GAME_WINDOW_WIDTCH - GAME_PADDING * 2 ) / 9 )

#define GRID_VERTICAL_POS ( GAME_PADDING * 2 )
#define GRID_HEIGHT ( CELL_SIZE * 10 )
#define GRID_WIDTCH ( CELL_SIZE * 9 )

#define GAME_FONT "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
#define GAME_FONT_BOLD "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"

#define MAX_SCORE_TEXT_SIZE 10

#define FRAME_RATE 120


#include"mlv_custom_text_animation.h"
#include"mlv_game_screen.h"


#endif /* _MLV_GAME_STRATEGY_H  */