#include "mlv_menu_screen.h"
#include <MLV/MLV_all.h>
#include <stdlib.h>
#include <math.h>

/* ============================
        CONSTANTS
   ============================ */

#define BTN_W  300
#define BTN_H   80
#define BTN_R   18

#define BASE_R 255
#define BASE_G 247
#define BASE_B 229

#define BTN_HOVER      MLV_rgba(70,130,255,255)
#define TEXT_COLOR     MLV_COLOR_BLUE
#define BG_COLOR       MLV_rgba(44,170,201,255)

#define draw_spiral_stars draw_intro_background

#define ARROW_W 60
#define ARROW_H 60

/* default background at launch */
#define BACKGROUND_MODE 2
/* 1 = drifting stars
   2 = twinkling stars
   3 = nebula dust
   4 = spiral galaxy */

#define STAR_COUNT 80

/* ============================
       BACKGROUND STATE
   ============================ */

static int stars_init = 0;

/* stars (modes 1 & 2) */
static int star_x[STAR_COUNT];
static int star_y[STAR_COUNT];
static int star_speed[STAR_COUNT];
static int star_alpha[STAR_COUNT];
static int star_alpha_dir[STAR_COUNT];

/* dust (mode 3) */
static int dust_x[STAR_COUNT];
static int dust_y[STAR_COUNT];
static int dust_dx[STAR_COUNT];

/* spiral (mode 4) */
static double spiral_angle = 0.0;

/* runtime background mode (starts from macro default) */
static int background_mode = BACKGROUND_MODE;


/* ============================
       STAR / BG INIT
   ============================ */

static void init_stars(void){
    int i;
    if (stars_init) return;

    for (i = 0; i < STAR_COUNT; i++) {
        /* stars (modes 1 & 2) */
        star_x[i] = rand() % GAME_WINDOW_WIDTCH;
        star_y[i] = rand() % GAME_WINDOW_HEIGHT;
        star_speed[i] = 1 + rand() % 2;
        star_alpha[i] = 80 + rand() % 140;
        star_alpha_dir[i] = (rand() % 2) ? 1 : -1;

        /* dust (mode 3) */
        dust_x[i] = rand() % GAME_WINDOW_WIDTCH;
        dust_y[i] = rand() % GAME_WINDOW_HEIGHT;
        dust_dx[i] = (rand() % 3) - 1;  /* -1, 0, 1 */
    }

    stars_init = 1;
}

static void draw_background_stars(void){
    int i;

    init_stars();

    /* dark blue base */
    MLV_clear_window(MLV_rgba(12,16,35,255));

    /* -------- modes 1 & 2: stars -------- */
    if (background_mode == 1 || background_mode == 2) {

        for (i = 0; i < STAR_COUNT; i++) {

            MLV_draw_filled_circle(
                star_x[i],
                star_y[i],
                1,
                MLV_rgba(200,220,255,star_alpha[i])
            );

            if (background_mode == 1) {
                /* drifting stars */
                star_y[i] += star_speed[i];
                if (star_y[i] >= GAME_WINDOW_HEIGHT)
                    star_y[i] = 0;
            }
            else {
                /* twinkling stars */
                star_alpha[i] += star_alpha_dir[i] * 3;
                if (star_alpha[i] > 220) star_alpha_dir[i] = -1;
                if (star_alpha[i] < 60)  star_alpha_dir[i] = 1;
            }
        }
        return;
    }

    /* -------- mode 3: nebula dust -------- */
    if (background_mode == 3) {

        for (i = 0; i < STAR_COUNT; i++) {

            MLV_draw_filled_circle(
                dust_x[i],
                dust_y[i],
                2,
                MLV_rgba(180,200,255,60)
            );

            dust_y[i] += 1;
            dust_x[i] += dust_dx[i];

            if (dust_y[i] >= GAME_WINDOW_HEIGHT) {
                dust_y[i] = 0;
                dust_x[i] = rand() % GAME_WINDOW_WIDTCH;
            }
        }
        return;
    }
/* -------- mode 4: calm parallax glow -------- */
    if (background_mode == 4) {

        static int init = 0;
        static int px[60];
        static int py[60];
        static int layer[60];
        static int phase[60];

        int i;

        if (!init) {
            for (i = 0; i < 60; i++) {
                px[i] = rand() % GAME_WINDOW_WIDTCH;
                py[i] = rand() % GAME_WINDOW_HEIGHT;
                layer[i] = 1 + rand() % 3;   /* depth layers */
                phase[i] = rand() % 360;
            }
            init = 1;
        }

        /* deep night background */
        MLV_clear_window(MLV_rgba(14,18,40,255));

        for (i = 0; i < 60; i++) {

            int speed = layer[i];          /* farther = slower */
            int radius = layer[i];         /* nearer = bigger */

            px[i] += speed;
            if (px[i] > GAME_WINDOW_WIDTCH)
                px[i] = 0;

            phase[i] += 2;
            if (phase[i] > 360) phase[i] = 0;

            MLV_draw_filled_circle(
                px[i],
                py[i],
                radius,
                MLV_rgba(
                    160 + layer[i] * 20,
                    180 + layer[i] * 15,
                    255,
                    80 + (int)(20 * sin(phase[i] * 0.017))
                    )
                );
        }
    }
}

/* ============================
       TUTORIAL PAGES (FR)
   ============================ */

static const char* tutorial_pages[] = {
    "Bienvenue dans Number Match !\n\n"
    "Le but du jeu est simple : effacer tous les chiffres du plateau.\n\n"
    "Pour cela, trouvez :\n"
    "• des paires de nombres identiques (ex. : 1 et 1),\n"
    "• ou des paires dont la somme fait 10 (ex. : 6 et 4).\n\n"
    "Sélectionnez les chiffres un par un pour les rayer,\n"
    "et progressez jusqu’à effacer tout le plateau !\n",

    "Règles essentielles :\n\n"
    "• Chaque paire trouvée vous fait gagner des points.\n"
    "• Lorsque toutes les cases d’une ligne disparaissent,\n"
    "  la ligne est supprimée et le plateau se resserre.\n\n"
    "• Vérifiez toutes les directions possibles :\n"
    "  - horizontalement,\n"
    "  - verticalement,\n"
    "  - diagonalement.\n\n"
    "Les diagonales opposées peuvent aussi former des paires !\n",

    "Astuces pour progresser :\n\n"
    "• Pensez à regarder les chiffres séparés par des cases :\n"
    "  ils peuvent tout de même former une paire.\n\n"
    "• Analysez le plateau ligne par ligne :\n"
    "  la fin d’une ligne et le début de la suivante\n"
    "  peuvent cacher une paire compatible.\n\n"
    "• Prenez votre temps et observez bien les motifs répétés.\n",

    "Bonne chance et amusez-vous bien !\n\n"
    "Number Match est un jeu de logique et d’observation.\n"
    "Plus vous jouerez, plus vous repérerez les paires rapidement.\n"
    "Amusez-vous à optimiser vos choix et à battre vos scores !\n"
};


/* ============================
       HELPER FUNCTIONS
   ============================ */

static int hit_button(int mx, int my, int x, int y){
    return mx >= x && mx <= x + BTN_W && my >= y && my <= y + BTN_H;
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
    int tw, th;
    int x;

    x = (GAME_WINDOW_WIDTCH - BTN_W)/2;
    draw_round_button(x, y, BTN_W, BTN_H, BTN_R, hover ? BTN_HOVER : BG_COLOR);
    MLV_get_size_of_text(label, &tw, &th);
    MLV_draw_text(x + BTN_W/2 - tw/2,
                  y + BTN_H/2 - th/2,
                  label,
                  TEXT_COLOR);
}

/* ============================
    BG TOGGLE BUTTON (TOP-RIGHT)
   ============================ */
static int bg_mouse_before = 0;

static int hit_bg_button(int mx, int my){
    int x = GAME_WINDOW_WIDTCH - 60;
    int y = 20;
    int w = 40;
    int h = 40;
    return hit_small(mx, my, x, y, w, h);
}

static void draw_bg_button(int mx, int my){
    int x = GAME_WINDOW_WIDTCH - 60;
    int y = 20;
    int w = 40;
    int h = 40;
    int hover = hit_bg_button(mx, my);

    draw_round_button(
        x, y, w, h, 8,
        hover ? BTN_HOVER : BG_COLOR
    );

    MLV_draw_text(x + 10, y + 10, "BG", TEXT_COLOR);
}


/* ============================
        ARROW DRAWING
   ============================ */

static void draw_left_arrow(int x, int y, int hover) {
    MLV_Color c;
    int xs[3];
    int ys[3];

    c = hover ? MLV_COLOR_BLUE : MLV_COLOR_BLACK;

    xs[0] = x + ARROW_W;
    xs[1] = x + ARROW_W;
    xs[2] = x;

    ys[0] = y;
    ys[1] = y + ARROW_H;
    ys[2] = y + ARROW_H/2;

    MLV_draw_filled_polygon(xs, ys, 3, c);
}

static void draw_right_arrow(int x, int y, int hover) {
    MLV_Color c;
    int xs[3];
    int ys[3];

    c = hover ? MLV_COLOR_BLUE : MLV_COLOR_BLACK;

    xs[0] = x;
    xs[1] = x;
    xs[2] = x + ARROW_W;

    ys[0] = y;
    ys[1] = y + ARROW_H;
    ys[2] = y + ARROW_H/2;

    MLV_draw_filled_polygon(xs, ys, 3, c);
}


/* ============================
       SLIDE TRANSITION
   ============================ */

static void animate_slide(const char* old_page, const char* new_page, int direction) {
    int offset;

    offset = 0;
    while (offset < GAME_WINDOW_WIDTCH) {

        MLV_clear_window(MLV_rgba(12,16,35,255));

        MLV_draw_text_box(
            40 - offset * direction,
            40,
            GAME_WINDOW_WIDTCH - 80,
            GAME_WINDOW_HEIGHT - 120,
            old_page,
            9,
            MLV_COLOR_BLACK,
            MLV_COLOR_BLACK,
            MLV_COLOR_WHITE,
            MLV_TEXT_LEFT,
            MLV_HORIZONTAL_CENTER,
            MLV_VERTICAL_TOP
        );

        MLV_draw_text_box(
            40 + (GAME_WINDOW_WIDTCH - offset) * direction,
            40,
            GAME_WINDOW_WIDTCH - 80,
            GAME_WINDOW_HEIGHT - 120,
            new_page,
            9,
            MLV_COLOR_BLACK,
            MLV_COLOR_BLACK,
            MLV_COLOR_WHITE,
            MLV_TEXT_LEFT,
            MLV_HORIZONTAL_CENTER,
            MLV_VERTICAL_TOP
        );

        MLV_actualise_window();
        MLV_delay_according_to_frame_rate();

        offset += 40;
    }
}


/* ============================
        TUTORIAL SCREEN
   ============================ */

void show_tutorial_screen() {

    int page = 0;
    int running = 1;
    int mx = 0, my = 0;

    int arrow_left_x;
    int arrow_left_y;
    int arrow_right_x;
    int arrow_right_y;

    int nb_pages;

    int left_before  = 0;
    int right_before = 0;
    int enter_before = 0;
    int esc_before   = 0;
    int mouse_before = 0;

    nb_pages = (int)(sizeof(tutorial_pages) / sizeof(tutorial_pages[0]));

    arrow_left_y  = GAME_WINDOW_HEIGHT - 150;
    arrow_right_y = GAME_WINDOW_HEIGHT - 150;

    arrow_left_x  = 60;
    arrow_right_x = GAME_WINDOW_WIDTCH - ARROW_W - 60;

    while (running) {

        MLV_Event ev;
        char buf[64];
        int hover_left;
        int hover_right;

        int left_now;
        int right_now;
        int enter_now;
        int esc_now;
        int mouse_now;

        MLV_get_mouse_position(&mx, &my);
        hover_left  = hit_small(mx,my,arrow_left_x,arrow_left_y,ARROW_W,ARROW_H);
        hover_right = hit_small(mx,my,arrow_right_x,arrow_right_y,ARROW_W,ARROW_H);

        MLV_clear_window(MLV_rgba(12,16,35,255));

        MLV_draw_text_box(
            40, 40,
            GAME_WINDOW_WIDTCH - 80,
            GAME_WINDOW_HEIGHT - 200,
            tutorial_pages[page],
            9,
            MLV_COLOR_BLACK,
            MLV_COLOR_BLACK,
            MLV_COLOR_WHITE,
            MLV_TEXT_LEFT,
            MLV_HORIZONTAL_CENTER,
            MLV_VERTICAL_TOP
        );

        sprintf(buf, "Page %d / %d", page+1, nb_pages);
        MLV_draw_text(GAME_WINDOW_WIDTCH/2 - 40,
                      GAME_WINDOW_HEIGHT - 120,
                      buf,
                      MLV_COLOR_BLUE);

        draw_left_arrow (arrow_left_x,  arrow_left_y,  hover_left);
        draw_right_arrow(arrow_right_x, arrow_right_y, hover_right);

        MLV_draw_text(
            40, GAME_WINDOW_HEIGHT - 40,
            "Flèches GAUCHE/DROITE ou clic — Entrée/Echap pour quitter",
            MLV_COLOR_BLACK
        );

        MLV_actualise_window();

        ev = MLV_get_event(NULL,NULL,NULL,NULL,NULL,&mx,&my,NULL,NULL);

        /* ---------- EDGE-DETECTION KEYBOARD ---------- */

        left_now  = (MLV_get_keyboard_state(MLV_KEYBOARD_LEFT)   == MLV_PRESSED);
        right_now = (MLV_get_keyboard_state(MLV_KEYBOARD_RIGHT)  == MLV_PRESSED);
        enter_now = (MLV_get_keyboard_state(MLV_KEYBOARD_RETURN) == MLV_PRESSED);
        esc_now   = (MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED);

        if (left_now && !left_before && page > 0) {
            animate_slide(tutorial_pages[page], tutorial_pages[page-1], -1);
            page--;
        }

        if (right_now && !right_before && page < nb_pages - 1) {
            animate_slide(tutorial_pages[page], tutorial_pages[page+1], 1);
            page++;
        }

        if ((enter_now && !enter_before) ||
            (esc_now   && !esc_before)) {
            running = 0;
        }

        left_before  = left_now;
        right_before = right_now;
        enter_before = enter_now;
        esc_before   = esc_now;

        /* ---------- EDGE-DETECTION MOUSE ---------- */

        mouse_now = (MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED);

        if (mouse_now && !mouse_before) {

            if (hover_left && page > 0) {
                animate_slide(tutorial_pages[page], tutorial_pages[page-1], -1);
                page--;
            }

            if (hover_right && page < nb_pages - 1) {
                animate_slide(tutorial_pages[page], tutorial_pages[page+1], 1);
                page++;
            }
        }

        mouse_before = mouse_now;
    }
}

/* ============================
     CONFIRM NEW GAME
   ============================ */

static int confirm_new_game(void){
    int mx = 0, my = 0;
    int bx, by;
    int yes_x, no_x;
    int btn_w = 120;
    int btn_h = 60;

    bx = (GAME_WINDOW_WIDTCH - 420) / 2;
    by = (GAME_WINDOW_HEIGHT - 200) / 2;

    yes_x = bx + 20;
    no_x  = bx + 280;

    while (1) {

        MLV_Event ev;
        int hover_yes, hover_no;

        MLV_get_mouse_position(&mx,&my);

        hover_yes = hit_small(mx,my,yes_x,by+120,btn_w,btn_h);
        hover_no  = hit_small(mx,my,no_x ,by+120,btn_w,btn_h);

        MLV_draw_filled_rectangle(
            0,0,
            GAME_WINDOW_WIDTCH,
            GAME_WINDOW_HEIGHT,
            MLV_rgba(0,0,0,120)
        );

        MLV_draw_filled_rectangle(bx,by,420,200,MLV_COLOR_WHITE);
        MLV_draw_rectangle(bx,by,420,200,MLV_COLOR_BLACK);

        MLV_draw_text(
            bx + 40, by + 40,
            "Supprimer la derniere sauvegarde ?",
            MLV_COLOR_BLACK
        );

        draw_round_button(
            yes_x, by+120,
            btn_w, btn_h, BTN_R,
            hover_yes ? BTN_HOVER : BG_COLOR
        );

        draw_round_button(
            no_x, by+120,
            btn_w, btn_h, BTN_R,
            hover_no ? BTN_HOVER : BG_COLOR
        );

        MLV_draw_text(yes_x + 45, by+140, "OUI", MLV_COLOR_BLUE);
        MLV_draw_text(no_x  + 45, by+140, "NON", MLV_COLOR_BLUE);

        MLV_actualise_window();

        ev = MLV_get_event(NULL,NULL,NULL,NULL,NULL,&mx,&my,NULL,NULL);

        if (ev == MLV_MOUSE_BUTTON) {
            if (hover_yes) return 1;
            if (hover_no)  return 0;
        }
    }
}
/* ============================
     INTRO BACKGROUND
   ============================ */

static void draw_intro_background(double t) {
    int i;
    int cx = GAME_WINDOW_WIDTCH / 2;
    int cy = GAME_WINDOW_HEIGHT / 2;

    /* deep space base */
    MLV_clear_window(MLV_rgba(12,16,35,255));

    /* drifting nebula dust */
    for (i = 0; i < 140; i++) {

        double dx = sin(t * 0.4 + i * 0.7) * 120.0;
        double dy = cos(t * 0.3 + i * 0.9) * 80.0;

        int x = cx + (int)dx + (i * 13) % GAME_WINDOW_WIDTCH;
        int y = cy + (int)dy + (i * 17) % GAME_WINDOW_HEIGHT;

        x %= GAME_WINDOW_WIDTCH;
        y %= GAME_WINDOW_HEIGHT;
        if (x < 0) x += GAME_WINDOW_WIDTCH;
        if (y < 0) y += GAME_WINDOW_HEIGHT;

        MLV_draw_filled_circle(
            x, y,
            2,
            MLV_rgba(90, 120, 200, 35)
        );
    }

    /* subtle stars (very light, very few) */
    for (i = 0; i < 40; i++) {

        int x = (i * 97) % GAME_WINDOW_WIDTCH;
        int y = (i * 53) % GAME_WINDOW_HEIGHT;

        MLV_draw_filled_circle(
            x, y,
            1,
            MLV_rgba(200, 220, 255, 80)
        );
    }
}

/* ============================
     ANIMATED TITLE
   ============================ */

static void draw_animated_title(const char* text, float phase){
    int tw, th, cw;
    int x, y, i;
    char buf[2];

    MLV_get_size_of_text(text,&tw,&th);
    MLV_get_size_of_text("N",&cw,&th);

    x = GAME_WINDOW_WIDTCH/2 - tw/2;
    y = GAME_WINDOW_HEIGHT/6;

    buf[1] = '\0';

    for (i = 0; text[i]; i++) {

        int p = (i + (int)(phase * 20.0f)) % 4;
        MLV_Color c;

        if      (p == 0) c = MLV_rgba(255,215,0,255);
        else if (p == 1) c = MLV_rgba(0,255,255,255);
        else if (p == 2) c = MLV_rgba(255,105,180,255);
        else             c = MLV_rgba(173,216,230,255);

        buf[0] = text[i];

        MLV_draw_text(x + i*cw - 1, y, buf, c);
        MLV_draw_text(x + i*cw + 1, y, buf, c);
        MLV_draw_text(x + i*cw,     y, buf, MLV_COLOR_WHITE);
    }
}
/* ============================
   ANIMATED TITLE (CUSTOM Y)
   ============================ */

static void draw_animated_title_at_y(const char* text, float phase, int y){
    int tw, th, cw;
    int x, i;
    char buf[2];

    MLV_get_size_of_text(text,&tw,&th);
    MLV_get_size_of_text("N",&cw,&th);

    x = GAME_WINDOW_WIDTCH/2 - tw/2;

    buf[1] = '\0';

    for (i = 0; text[i]; i++) {

        int p = (i + (int)(phase * 20.0f)) % 4;
        MLV_Color c;

        if      (p == 0) c = MLV_rgba(255,215,0,255);
        else if (p == 1) c = MLV_rgba(0,255,255,255);
        else if (p == 2) c = MLV_rgba(255,105,180,255);
        else             c = MLV_rgba(173,216,230,255);

        buf[0] = text[i];

        MLV_draw_text(x + i*cw - 1, y, buf, c);
        MLV_draw_text(x + i*cw + 1, y, buf, c);
        MLV_draw_text(x + i*cw,     y, buf, MLV_COLOR_WHITE);
    }
}

/* ============================
   SPIRAL OF NUMBERS (→ 10)
   ============================ */

static void draw_number_spiral(double angle, double radius) {
    int cx = GAME_WINDOW_WIDTCH / 2;
    int cy = GAME_WINDOW_HEIGHT / 2;
    int i;

    /* number pairs summing to 10 */
    const char* left_nums[]  = { "1", "2", "3", "4", "5" };
    const char* right_nums[] = { "9", "8", "7", "6", "5" };
    const int count = 5;

    for (i = 0; i < count; i++) {

        double a = angle + i * 0.9;
        double r = radius - i * 35;

        int xL = cx + (int)(cos(a) * r);
        int yL = cy + (int)(sin(a) * r);

        int xR = cx + (int)(cos(a + 0.4) * r);
        int yR = cy + (int)(sin(a + 0.4) * r);

        if (r > 0) {
            MLV_draw_text(xL, yL, left_nums[i],  MLV_COLOR_WHITE);
            MLV_draw_text(xR, yR, right_nums[i], MLV_COLOR_WHITE);
        }
    }
}

/* ============================
   INTRO ANIMATION
   ============================ */

static void play_intro_animation(void) {
    double spiral_angle = 0.0;
    int frame, wait;
    int cx = GAME_WINDOW_WIDTCH / 2 - 2;
    int cy = GAME_WINDOW_HEIGHT / 2;
    int xL, xR;

    const int target_left  = GAME_WINDOW_WIDTCH / 2;
    const int target_right = GAME_WINDOW_WIDTCH / 2;
    const int speed = 2;

    /* ========== 5 + 5 = 10 ========== */

    xL = -80;
    xR = GAME_WINDOW_WIDTCH + 80;

    /* slide IN together */
    while (xL < target_left || xR > target_right) {

        if (xL < target_left)  xL += speed;
        if (xR > target_right) xR -= speed;

        draw_spiral_stars(spiral_angle);
        spiral_angle += 0.05;

        MLV_draw_text(xL, cy, "5", MLV_COLOR_WHITE);
        MLV_draw_text(xR, cy, "5", MLV_COLOR_WHITE);

        MLV_actualise_window();
    }

    /* hold numbers */
    for (wait = 0; wait < 30; wait++) {
        draw_spiral_stars(spiral_angle);
        spiral_angle += 0.05;
        MLV_draw_text(xL, cy, "5", MLV_COLOR_WHITE);
        MLV_draw_text(xR, cy, "5", MLV_COLOR_WHITE);
    }

    /* show result */
    for (wait = 0; wait < 40; wait++) {
        draw_spiral_stars(spiral_angle);
        spiral_angle += 0.05;
        MLV_draw_text(cx, cy, "10", MLV_COLOR_YELLOW);
        MLV_actualise_window();
    }

    /* ========== 6 + 4 = 10 (DIAGONAL) ========== */

    {
        int xTL, yTL;
        int xBR, yBR;

        xTL = -80;
        yTL = -80;

        xBR = GAME_WINDOW_WIDTCH + 40;
        yBR = GAME_WINDOW_HEIGHT + 40;

        /* slide diagonally */
        while (xTL < cx || yTL < cy || xBR > cx || yBR > cy) {

            if (xTL < cx) xTL += speed;
            if (yTL < cy) yTL += speed;

            if (xBR > cx) xBR -= speed;
            if (yBR > cy) yBR -= speed;

            draw_spiral_stars(spiral_angle);
            spiral_angle += 0.05;

            MLV_draw_text(xTL, yTL, "6", MLV_COLOR_WHITE);
            MLV_draw_text(xBR, yBR, "4", MLV_COLOR_WHITE);

            MLV_actualise_window();
        }

        /* hold numbers */
        for (wait = 0; wait < 30; wait++) {
            draw_spiral_stars(spiral_angle);
            spiral_angle += 0.05;
            MLV_draw_text(cx, cy, "6", MLV_COLOR_WHITE);
            MLV_draw_text(cx, cy, "4", MLV_COLOR_WHITE);
        }

        /* show result */
        for (wait = 0; wait < 40; wait++) {
            draw_spiral_stars(spiral_angle);
            spiral_angle += 0.05;
            MLV_draw_text(cx, cy, "10", MLV_COLOR_YELLOW);
            MLV_actualise_window();
        }
    }
/* ========== 8 + 2 = 10 (DIAGONAL – OPPOSITE) ========== */

    {
        int xTR, yTR;
        int xBL, yBL;

        xTR = GAME_WINDOW_WIDTCH + 80;
        yTR = -80;

        xBL = -80;
        yBL = GAME_WINDOW_HEIGHT + 80;

        /* slide diagonally (opposite corners) */
        while (xTR > cx || yTR < cy || xBL < cx || yBL > cy) {

            if (xTR > cx) xTR -= speed;
            if (yTR < cy) yTR += speed;

            if (xBL < cx) xBL += speed;
            if (yBL > cy) yBL -= speed;

            draw_spiral_stars(spiral_angle);
            spiral_angle += 0.05;

            MLV_draw_text(xTR, yTR, "8", MLV_COLOR_WHITE);
            MLV_draw_text(xBL, yBL, "2", MLV_COLOR_WHITE);

            MLV_actualise_window();
        }

        /* hold numbers */
        for (wait = 0; wait < 30; wait++) {
            draw_spiral_stars(spiral_angle);
            spiral_angle += 0.05;
            MLV_draw_text(cx, cy, "8", MLV_COLOR_WHITE);
            MLV_draw_text(cx, cy, "2", MLV_COLOR_WHITE);
        }

        /* show result */
        for (wait = 0; wait < 40; wait++) {
            draw_spiral_stars(spiral_angle);
            spiral_angle += 0.05;
            MLV_draw_text(cx, cy, "10", MLV_COLOR_YELLOW);
            MLV_actualise_window();
        }
    }

/* ========== 3 + 7 = 10 (TOP / BOTTOM) ========== */

    int yTop, yBot;

    xL = target_left;
    xR = target_right;

    yTop = -80;
    yBot = GAME_WINDOW_HEIGHT + 80;

/* slide vertically */
    while (yTop < cy || yBot > cy) {

        if (yTop < cy) yTop += speed;
        if (yBot > cy) yBot -= speed;

        draw_spiral_stars(spiral_angle);
        spiral_angle += 0.05;

        MLV_draw_text(xL, yTop, "3", MLV_COLOR_WHITE);
        MLV_draw_text(xR, yBot, "7", MLV_COLOR_WHITE);

        MLV_actualise_window();
    }

/* hold numbers */
    for (wait = 0; wait < 30; wait++) {
        draw_spiral_stars(spiral_angle);
        spiral_angle += 0.05;
        MLV_draw_text(xL, cy, "3", MLV_COLOR_WHITE);
        MLV_draw_text(xR, cy, "7", MLV_COLOR_WHITE);
    }
    
    for (wait = 0; wait < 40 ; wait++) {
        draw_spiral_stars(spiral_angle);
        spiral_angle += 0.05;
        MLV_draw_text(cx, cy, "10", MLV_COLOR_YELLOW);
        MLV_actualise_window();
    }
    
/* ========== SPIRAL → 10 ========== */

    {
        double angle = 0.0;
        double radius = 260.0;
        int wait;

        while (radius > 40.0) {

            draw_spiral_stars(spiral_angle);

            draw_number_spiral(angle, radius);

            angle  += 0.05;   /* slower rotation */
            radius -= 0.9;    /* slower collapse */

            MLV_actualise_window();
            MLV_delay_according_to_frame_rate();
        }


        /* final result → slide into menu title position (animated colors) */
        {
            int y_start = GAME_WINDOW_HEIGHT / 2;
            int y_end   = GAME_WINDOW_HEIGHT / 6;
            int y = y_start;
            float phase = 0.0f;

            while (y > y_end) {

                draw_spiral_stars(spiral_angle);
                spiral_angle += 0.05;
                MLV_delay_according_to_frame_rate();

                draw_animated_title_at_y("Number Match", phase, y);

                phase += 0.02f;
                y -= 2;

                MLV_actualise_window();
            }
        }
    }
}
/* ============================
           MAIN MENU
   ============================ */

void mlv_show_menu(struct game_config *config){

    int mx = 0;
    int my = 0;
    int running = 1;
    int tw, th;

    float fade = 0.001f;
    int direction = 1;

    int play_y = 230;
    int load_y = 330;
    int tut_y  = 430;
    int quit_y = 530;

    int bx;

    MLV_create_window("NumberMatch Menu","NumberMatch",
                      GAME_WINDOW_WIDTCH,GAME_WINDOW_HEIGHT);

    MLV_change_frame_rate(FRAME_RATE);

    play_intro_animation();
    
    MLV_ctext_animations_start();

    while(running){

        MLV_Event ev;
        int r, g, b;

        MLV_get_mouse_position(&mx,&my);
        int bg_mouse_now =
            (MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED);

        fade += 0.001f * direction;
        if (fade >= 1.0f) direction = -1;
        if (fade <= 0.0f) direction = 1;

        r = BASE_R - (int)(fade*35);
        g = BASE_G - (int)(fade*35);
        b = BASE_B - (int)(fade*10);

        (void)r; (void)g; (void)b; /* kept to avoid warnings */

        /* background (modes 1–4) */
        draw_background_stars();

        /* Animated title */
        (void)tw; (void)th;
        draw_animated_title("NUMBER MATCH", fade);

        bx = (GAME_WINDOW_WIDTCH - BTN_W)/2;

        /* BG toggle button (top-right) */
        draw_bg_button(mx, my);

        /* main buttons */
        draw_button("NOUVELLE PARTIE",  play_y, hit_button(mx,my,bx,play_y));
        draw_button("CHARGER PARTIE",   load_y, hit_button(mx,my,bx,load_y));
        draw_button("TUTORIEL",         tut_y,  hit_button(mx,my,bx,tut_y));
        draw_button("QUITTER",          quit_y, hit_button(mx,my,bx,quit_y));

        MLV_actualise_window();
/* BG button: edge-detected, NOT event-based */
        if (hit_bg_button(mx,my) && bg_mouse_now && !bg_mouse_before) {
            background_mode++;
            if (background_mode > 4)
                background_mode = 1;
        }

        ev = MLV_get_event(NULL,NULL,NULL,NULL,NULL,&mx,&my,NULL,NULL);

        if (ev == MLV_MOUSE_BUTTON) {

            /* main menu buttons unchanged */
            if (hit_button(mx,my,bx, play_y)) {
                if (confirm_new_game()) {
                    start_game(config);
                }
            }

            if (hit_button(mx,my,bx, load_y)) {
                load_game(config);
            }

            if (hit_button(mx,my,bx, tut_y)) {
                show_tutorial_screen();
            }

            if (hit_button(mx,my,bx, quit_y)) {
                running = 0;
            }
        }
        bg_mouse_before = bg_mouse_now;
    }

    MLV_ctext_animations_end();

    MLV_free_window();
}
