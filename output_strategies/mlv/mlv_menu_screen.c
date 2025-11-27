#include "mlv_menu_screen.h"
#include <MLV/MLV_all.h>

#define BTN_W  300
#define BTN_H   80
#define BTN_COLOR      MLV_rgba(70,130,255,255)
#define BTN_HOVER      MLV_rgba(255, 223, 15, 200)
#define TEXT_COLOR     MLV_COLOR_WHITE
#define BG_COLOR       MLV_rgba(44, 170, 201, 255)

static int hit(int mx,int my,int x,int y){
    return mx>=x && mx<=x+BTN_W && my>=y && my<=y+BTN_H;
}

static void draw_button(const char *label,int y,int hover){
    int tw,th;
    int x = (GAME_WINDOW_WIDTCH - BTN_W)/2;

    MLV_draw_filled_rectangle(x,y,BTN_W,BTN_H, hover?BTN_HOVER:BTN_COLOR);
    MLV_draw_rectangle(x,y,BTN_W,BTN_H,MLV_COLOR_BLUE);

    MLV_get_size_of_text(label,&tw,&th);
    MLV_draw_text(x+BTN_W/2-tw/2, y+BTN_H/2-th/2, label, TEXT_COLOR);
}

void mlv_show_menu(struct game_config *config){

    int mx,my;
    int running = 1;
    int tw,th;

    int play_y=230, load_y=330, tut_y=430, quit_y=530;

    MLV_create_window("NumberMatch Menu","NumberMatch",
                      GAME_WINDOW_WIDTCH,GAME_WINDOW_HEIGHT);

    while(running){

        MLV_get_mouse_position(&mx,&my);
        MLV_clear_window(BG_COLOR);

        {
            MLV_get_size_of_text("NUMBER MATCH",&tw,&th);
            MLV_draw_text(GAME_WINDOW_WIDTCH/2-tw/2,120,"NUMBER MATCH",MLV_COLOR_WHITE);
        }

        /* DRAW ONLY — NO ACTIONS */
        draw_button("NEW GAME",  play_y, hit(mx,my,(GAME_WINDOW_WIDTCH-BTN_W)/2,play_y));
        draw_button("LOAD GAME", load_y, hit(mx,my,(GAME_WINDOW_WIDTCH-BTN_W)/2,load_y));
        draw_button("TUTORIAL",  tut_y,  hit(mx,my,(GAME_WINDOW_WIDTCH-BTN_W)/2,tut_y));
        draw_button("EXIT",      quit_y, hit(mx,my,(GAME_WINDOW_WIDTCH-BTN_W)/2,quit_y));

        MLV_actualise_window();

    }

    MLV_free_window();
}
