#include "mlv_menu_screen.h"
#include <MLV/MLV_all.h>

#define BTN_W  300
#define BTN_H   80
#define BTN_R   18
#define BASE_R 255
#define BASE_G 247
#define BASE_B 229
#define BTN_COLOR      MLV_rgba(0,0,128, 255)
#define BTN_HOVER      MLV_rgba(70,130,255,255)
#define TEXT_COLOR     MLV_COLOR_BLUE
#define BG_COLOR       MLV_rgba(44, 170, 201, 255)

static int hit(int mx,int my,int x,int y){
    return mx>=x && mx<=x+BTN_W && my>=y && my<=y+BTN_H;
}

static void draw_round_button(int x, int y, int w, int h, int r, MLV_Color c) {
    MLV_draw_filled_rectangle(x+r, y, w-2*r, h, c);
    MLV_draw_filled_rectangle(x, y+r, w, h-2*r, c);

    MLV_draw_filled_circle(x+r, y+r, r, c);
    MLV_draw_filled_circle(x+w-r, y+r, r, c);
    MLV_draw_filled_circle(x+r, y+h-r, r, c);
    MLV_draw_filled_circle(x+w-r, y+h-r, r, c);
}

static void draw_button(const char *label,int y,int hover){
    int tw,th;
    int x = (GAME_WINDOW_WIDTCH - BTN_W)/2;

    draw_round_button(x, y, BTN_W, BTN_H, BTN_R, hover?BTN_HOVER:BG_COLOR);
    MLV_get_size_of_text(label, &tw, &th);
    MLV_draw_text(x+BTN_W/2-tw/2, y+BTN_H/2-th/2, label, TEXT_COLOR);
}

void mlv_show_menu(struct game_config *config){

    int mx,my;
    int running = 1;
    int tw,th;

    float fade = 0.001f;
    int direction = 1;
    
    int play_y=230, load_y=330, tut_y=430, quit_y=530;

    MLV_create_window("NumberMatch Menu","NumberMatch",
                      GAME_WINDOW_WIDTCH,GAME_WINDOW_HEIGHT);

    while(running){

        MLV_get_mouse_position(&mx,&my);

        fade += 0.003f * direction;
        if(fade >= 1.0f)
            direction = -1;
        if(fade <= 0.0f)
            direction = 1;
        int r = BASE_R - (int)        (fade*30);
        int g = BASE_G - (int)        (fade*5);
        int b = BASE_B - (int)        (fade*30);

        MLV_clear_window( MLV_rgba(r,g,b,255) );
        {
            MLV_get_size_of_text("NUMBER MATCH",&tw,&th);
            MLV_draw_text(GAME_WINDOW_WIDTCH/2-tw/2,120,"NUMBER MATCH",TEXT_COLOR);
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
