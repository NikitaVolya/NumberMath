#include "mlv_menu_screen.h"
#include "mlv_game_setup.h"

#include <string.h>

static int mouse_over(int mx, int my, int x, int y, int w, int h) {
    return (mx >= x && mx <= x + w && my <= y + h);
}

void mlv_show_menu(struct game_config *config) {
    MLV_create_window(
        "NumberMatch | Menu",
        "NumberMatch",
        GAME_WINDOW_WIDTCH,
        GAME_WINDOW_HEIGHT
        );

    MLV_Font *title_font = MLV_load_font(GAME_FONT_BOLD, 42);
    MLV_Font *btn_font = MLV_load_font(GAME_FONT_BOLD, 28);

    int bw = GAME_WINDOW_WIDTCH * 0.5;
    int bh = 60;
    int bx = (GAME_WINDOW_WIDTCH - bw) / 2;

    int play_y = 220;
    int score_y = play_y + 100;
    int tutorial_y = score_y + 100;
    int quit_y = tutorial_y + 100;

    while(1) {

        MLV_clear_window(MLV_COLOR_BLACK);
        MLV_draw_text_with_font(GAME_PADDING, 80, "NUMBER MATCH", title_font, MLV_COLOR_WHITE);
        int mx, my;

        MLV_get_mouse_position(&mx, &my);

        MLV_Color play_color = mouse_over(mx, my, bx, play_y, bw, bh)
            ? MLV_COLOR_RED : MLV_COLOR_WHITE;
        MLV_draw_filled_rectangle(bx, play_y, bw, bh, MLV_COLOR_GREY);

        MLV_draw_text_box_with_font(bx, play_y, bw, bh, "PLAY", btn_font, 0, play_color, MLV_COLOR_BLACK, play_color, MLV_TEXT_CENTER,
                                    MLV_HORIZONTAL_CENTER,
                                    MLV_VERTICAL_CENTER
            );
        
        MLV_Color score_color = mouse_over(mx, my, bx, score_y, bw, bh)
            ? MLV_COLOR_RED : MLV_COLOR_WHITE;
        MLV_draw_filled_rectangle(bx, score_y, bw, bh, MLV_COLOR_GREY);
        
        MLV_draw_text_box_with_font(bx, score_y, bw, bh, "SCOREBOARD", btn_font, 0, score_color, MLV_COLOR_BLACK, score_color, MLV_TEXT_CENTER,
                                    MLV_HORIZONTAL_CENTER,
                                    MLV_VERTICAL_CENTER
            );
        
        MLV_Color tutorial_color = mouse_over(mx, my, bx, tutorial_y, bw, bh)
            ? MLV_COLOR_RED : MLV_COLOR_WHITE;
        MLV_draw_filled_rectangle(bx, tutorial_y, bw, bh, MLV_COLOR_GREY);
        
        MLV_draw_text_box_with_font(bx, tutorial_y, bw, bh, "TUTORIAL", btn_font, 0, tutorial_color, MLV_COLOR_BLACK, tutorial_color, MLV_TEXT_CENTER,
                                    MLV_HORIZONTAL_CENTER,
                                    MLV_VERTICAL_CENTER
            );
        
        MLV_Color quit_color = mouse_over(mx, my, bx, quit_y, bw, bh)
            ? MLV_COLOR_RED : MLV_COLOR_WHITE;
        MLV_draw_filled_rectangle(bx, quit_y, bw, bh, MLV_COLOR_GREY);
        
        MLV_draw_text_box_with_font(bx, quit_y, bw, bh, "QUIT", btn_font, 0, quit_color, MLV_COLOR_BLACK, quit_color, MLV_TEXT_CENTER,
                                    MLV_HORIZONTAL_CENTER,
                                    MLV_VERTICAL_CENTER
            );
        MLV_actualise_window();

        MLV_Keyboard_button kb;
        MLV_Mouse_button mb;
        MLV_Input_box *input;

        int click =
            MLV_get_event(
                &kb, NULL, NULL, NULL, NULL, &mx, &my, &mb, NULL
                );
        if (click == MLV_MOUSE_BUTTON) {
            if (mouse_over(mx, my, bx, play_y, bw, bh)) {
                MLV_free_font(title_font);
                MLV_free_font(btn_font);
                MLV_free_window();
                return

                    MENU_PLAY;
            }
        }
        if (click == MLV_MOUSE_BUTTON) {
            if (mouse_over(mx, my, bx, score_y, bw, bh)) {
                MLV_free_font(title_font);
                MLV_free_font(btn_font);
                MLV_free_window();
                return

                    MENU_SCOREBOARD;
            }
        }
            if (click == MLV_MOUSE_BUTTON) {
                if (mouse_over(mx, my, bx, tutorial_y, bw, bh)) {
                    MLV_free_font(title_font);
                    MLV_free_font(btn_font);
                    MLV_free_window();
                    return

                        MENU_TUTORIAL;
                }
            }
            if (click == MLV_MOUSE_BUTTON) {
                if (mouse_over(mx, my, bx, quit_y, bw, bh)) {
                    MLV_free_font(title_font);
                    MLV_free_font(btn_font);
                    MLV_free_window();
                    return

                        MENU_QUIT;
                }
            }
    }
return MENU_QUIT;
}
        
