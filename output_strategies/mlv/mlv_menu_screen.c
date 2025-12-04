#include "mlv_menu_screen.h"
#include <MLV/MLV_all.h>

#define BTN_W  300
#define BTN_H   80
#define BTN_R   18
#define BASE_R 255
#define BASE_G 247
#define BASE_B 229
#define BTN_COLOR      MLV_rgba(0,0,128,255)
#define BTN_HOVER      MLV_rgba(70,130,255,255)
#define TEXT_COLOR     MLV_COLOR_BLUE
#define BG_COLOR       MLV_rgba(44,170,201,255)

#define ARROW_W 60
#define ARROW_H 60

#define MAX_TUTORIAL_PAGES 5


/* ============================================
              TUTORIAL PAGES
   ============================================ */

static const char* tutorial_pages[MAX_TUTORIAL_PAGES] = {

    "PAGE 1 — Basics\n"
    "• Match two numbers that add up to 10.\n"
    "• Adjacent cells only.\n"
    "• Lines wrap around.\n",

    "PAGE 2 — Board mechanics\n"
    "• When no moves remain, new lines are added.\n"
    "• Undo costs points.\n",

    "PAGE 3 — Scoring\n"
    "• Removing numbers adds points.\n"
    "• Combos give bonus points.\n",

    "PAGE 4 — Strategy\n"
    "• Clear long lines first.\n"
    "• Avoid stacking identical numbers.\n",

    "PAGE 5 — Controls\n"
    "• Mouse selects numbers.\n"
    "• LEFT/RIGHT = navigate.\n"
    "• ENTER = exit tutorial.\n"
};


/* ============================================
              BUTTON UTILITIES
   ============================================ */

static int hit(int mx,int my,int x,int y){
    return mx>=x && mx<=x+BTN_W && my>=y && my<=y+BTN_H;
}

static int hit_small(int mx,int my,int x,int y,int w,int h){
    return mx>=x && mx<=x+w && my>=y && my<=y+h;
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


/* ============================================
              ARROW DRAWING
   ============================================ */

void draw_left_arrow(int x, int y, int hover) {
    MLV_Color c = hover ? MLV_COLOR_BLUE : MLV_COLOR_BLACK;

    int xs[3] = {x+ARROW_W, x+ARROW_W, x};
    int ys[3] = {y, y+ARROW_H, y+ARROW_H/2};

    MLV_draw_filled_polygon(xs, ys, 3, c);
}

void draw_right_arrow(int x, int y, int hover) {
    MLV_Color c = hover ? MLV_COLOR_BLUE : MLV_COLOR_BLACK;

    int xs[3] = {x, x, x+ARROW_W};
    int ys[3] = {y, y+ARROW_H, y+ARROW_H/2};

    MLV_draw_filled_polygon(xs, ys, 3, c);
}


/* ============================================
         PAGE SLIDE ANIMATION
   ============================================ */

void animate_slide(const char* old_page, const char* new_page, int direction) {
    /* direction = -1 (left), +1 (right) */

    for (int offset = 0; offset < GAME_WINDOW_WIDTCH; offset += 40) {

        MLV_clear_window(MLV_COLOR_BEIGE);

        /* OLD page sliding OUT */
        MLV_draw_text_box(
            40 - offset * direction,
            40,
            GAME_WINDOW_WIDTCH - 80,
            GAME_WINDOW_HEIGHT - 120,
            old_page,
            9, MLV_COLOR_BLACK, MLV_COLOR_BLACK,
            MLV_COLOR_WHITE, MLV_TEXT_LEFT,
            MLV_HORIZONTAL_CENTER, MLV_VERTICAL_TOP
        );

        /* NEW page sliding IN */
        MLV_draw_text_box(
            40 + (GAME_WINDOW_WIDTCH - offset) * direction,
            40,
            GAME_WINDOW_WIDTCH - 80,
            GAME_WINDOW_HEIGHT - 120,
            new_page,
            9, MLV_COLOR_BLACK, MLV_COLOR_BLACK,
            MLV_COLOR_WHITE, MLV_TEXT_LEFT,
            MLV_HORIZONTAL_CENTER, MLV_VERTICAL_TOP
        );

        MLV_actualise_window();
        MLV_delay_according_to_frame_rate();
    }
}


/* ============================================
            TUTORIAL SCREEN
   ============================================ */

void show_tutorial_screen() {

    int page = 0;
    int running = 1;
    int mx, my;
    MLV_Event ev;
    MLV_Keyboard_button key;

    int arrow_left_x  = 20;
    int arrow_left_y  = GAME_WINDOW_HEIGHT/2 - ARROW_H/2;
    int arrow_right_x = GAME_WINDOW_WIDTCH - ARROW_W - 20;
    int arrow_right_y = GAME_WINDOW_HEIGHT/2 - ARROW_H/2;

    while (running) {

        MLV_clear_window(MLV_COLOR_BEIGE);

        /* Show current page */
        MLV_draw_text_box(
            40, 40,
            GAME_WINDOW_WIDTCH - 80,
            GAME_WINDOW_HEIGHT - 120,
            tutorial_pages[page],
            9, MLV_COLOR_BLACK, MLV_COLOR_BLACK,
            MLV_COLOR_WHITE, MLV_TEXT_LEFT,
            MLV_HORIZONTAL_CENTER, MLV_VERTICAL_TOP
        );

        /* Page number */
        char buf[64];
        sprintf(buf, "Page %d / %d", page+1, MAX_TUTORIAL_PAGES);
        MLV_draw_text(GAME_WINDOW_WIDTCH/2 - 40,
                      GAME_WINDOW_HEIGHT - 60,
                      buf, MLV_COLOR_BLUE);

        /* Arrows */
        draw_left_arrow(
            arrow_left_x, arrow_left_y,
            hit_small(mx,my,arrow_left_x,arrow_left_y,ARROW_W,ARROW_H)
        );

        draw_right_arrow(
            arrow_right_x, arrow_right_y,
            hit_small(mx,my,arrow_right_x,arrow_right_y,ARROW_W,ARROW_H)
        );

        MLV_draw_text(40, GAME_WINDOW_HEIGHT - 30,
            "LEFT/RIGHT or click arrows — ENTER to exit",
            MLV_COLOR_BLACK
        );

        MLV_actualise_window();

        /* Get event */
        ev = MLV_get_event(&key,NULL,NULL,NULL,NULL,&mx,&my,NULL,NULL);


        /* KEYBOARD navigation */
        if (ev == MLV_KEY_PRESS || ev == MLV_KEYBOARD) {

            if (key == MLV_KEYBOARD_LEFT && page > 0) {
                animate_slide(tutorial_pages[page], tutorial_pages[page-1], -1);
                page--;
            }

            if (key == MLV_KEYBOARD_RIGHT && page < MAX_TUTORIAL_PAGES-1) {
                animate_slide(tutorial_pages[page], tutorial_pages[page+1], +1);
                page++;
            }

            if (key == MLV_KEYBOARD_RETURN || key == MLV_KEYBOARD_ESCAPE) {
                running = 0;
            }
        }

        /* MOUSE navigation */
        if (ev == MLV_MOUSE_BUTTON) {

            if (hit_small(mx,my,arrow_left_x,arrow_left_y,ARROW_W,ARROW_H) && page > 0) {
                animate_slide(tutorial_pages[page], tutorial_pages[page-1], -1);
                page--;
            }

            if (hit_small(mx,my,arrow_right_x,arrow_right_y,ARROW_W,ARROW_H) &&
                page < MAX_TUTORIAL_PAGES - 1) {
                animate_slide(tutorial_pages[page], tutorial_pages[page+1], +1);
                page++;
            }
        }
    }
}


/* ============================================
                 MAIN MENU
   ============================================ */

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

        fade += 0.001f * direction;
        if(fade >= 1.0f) direction = -1;
        if(fade <= 0.0f) direction = 1;

        int r = BASE_R - (int)(fade*35);
        int g = BASE_G - (int)(fade*35);
        int b = BASE_B - (int)(fade*10);

        MLV_clear_window( MLV_rgba(r,g,b,255) );

        MLV_get_size_of_text("NUMBER MATCH",&tw,&th);
        MLV_draw_text(GAME_WINDOW_WIDTCH/2-tw/2,
                      GAME_WINDOW_HEIGHT/6,
                      "NUMBER MATCH",
                      TEXT_COLOR);

        draw_button("NEW GAME",  play_y, hit(mx,my,(GAME_WINDOW_WIDTCH-BTN_W)/2,play_y));
        draw_button("LOAD GAME", load_y, hit(mx,my,(GAME_WINDOW_WIDTCH-BTN_W)/2,load_y));
        draw_button("TUTORIAL",  tut_y,  hit(mx,my,(GAME_WINDOW_WIDTCH-BTN_W)/2,tut_y));
        draw_button("EXIT",      quit_y, hit(mx,my,(GAME_WINDOW_WIDTCH-BTN_W)/2,quit_y));

        MLV_actualise_window();


        /* EVENT HANDLING */
        MLV_Event ev = MLV_get_event(
            NULL,NULL,NULL,
            NULL,NULL,
            &mx,&my,
            NULL,NULL
        );

        int bx = (GAME_WINDOW_WIDTCH - BTN_W) / 2;

        if (ev == MLV_MOUSE_BUTTON) {

            if (hit(mx,my,bx, play_y)) {
                config->load_save = 0;
                running = 0;
            }

            if (hit(mx,my,bx, load_y)) {
                config->load_save = 1;
                running = 0;
            }

            if (hit(mx,my,bx, tut_y)) {
                show_tutorial_screen();
            }

            if (hit(mx,my,bx, quit_y)) {
                running = 0;
            }
        }
    }

    MLV_free_window();
}