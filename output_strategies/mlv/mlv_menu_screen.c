/***********************************************
 *                 HEADERS
 ***********************************************/
#include "mlv_menu_screen.h"
#include <MLV/MLV_all.h>
#include <stdlib.h>
#include <math.h>


/***********************************************
 *                 CONSTANTS
 ***********************************************/

/* --- Button dimensions & styling --- */
#define BTN_W   160
#define BTN_H    60
#define BTN_R    18

/* --- Base color of animated UI fade (Menu) --- */
#define BASE_R 255
#define BASE_G 247
#define BASE_B 229

/* --- UI colors --- */
#define BTN_HOVER   MLV_rgba(70,130,255,255)
#define TEXT_COLOR  MLV_COLOR_BLUE
#define BG_COLOR    MLV_COLOR_BEIGE

/* --- Alias for intro background function (legacy name) --- */
#define draw_spiral_stars draw_intro_background

/* --- Arrow dimensions (tutorial navigation) --- */
#define ARROW_W 60
#define ARROW_H 60

/* --- Default background mode at launch --- */
#define BACKGROUND_MODE 2
/*
    1 = drifting stars
    2 = twinkling stars
    3 = nebula dust
    4 = calm parallax glow
*/

/* Star count used for multiple background modes */
#define STAR_COUNT 80


/***********************************************
 *       GLOBAL BACKGROUND / STAR STATE
 ***********************************************/

/* Initialization guard (avoids re-seeding stars each frame) */
static int stars_init = 0;

/* --- Stars (background modes 1 & 2) --- */
static int star_x[STAR_COUNT];
static int star_y[STAR_COUNT];
static int star_speed[STAR_COUNT];
static int star_alpha[STAR_COUNT];
static int star_alpha_dir[STAR_COUNT];

/* --- Nebula dust (background mode 3) --- */
static int dust_x[STAR_COUNT];
static int dust_y[STAR_COUNT];
static int dust_dx[STAR_COUNT];

/* --- Current active background mode (runtime) --- */
static int background_mode = BACKGROUND_MODE;


/***********************************************
 *             TUTORIAL PAGE TEXT
 ***********************************************/

/*
 * tutorial_pages[]
 * ---------------------------------------------------------
 * Static French tutorial instructions displayed in the
 * multi-page tutorial viewer. Each entry is one page.
 */
static const char* tutorial_pages[] = {

    /* ------------------ Page 1 ------------------ */
    "Bienvenue dans Number Match !\n\n"
    "Le but du jeu est simple : effacer tous les chiffres du plateau.\n\n"
    "Pour cela, trouvez :\n"
    "• des paires de nombres identiques (ex. : 1 et 1),\n"
    "• ou des paires dont la somme fait 10 (ex. : 6 et 4).\n\n"
    "Sélectionnez les chiffres un par un pour les rayer,\n"
    "et progressez jusqu’à effacer tout le plateau !\n",

    /* ------------------ Page 2 ------------------ */
    "Règles essentielles :\n\n"
    "• Chaque paire trouvée vous fait gagner des points.\n"
    "• Lorsque toutes les cases d’une ligne disparaissent,\n"
    "  la ligne est supprimée et le plateau se resserre.\n\n"
    "• Vérifiez toutes les directions possibles :\n"
    "  - horizontalement,\n"
    "  - verticalement,\n"
    "  - diagonalement.\n\n"
    "Les diagonales opposées peuvent aussi former des paires !\n",

    /* ------------------ Page 3 ------------------ */
    "Astuces pour progresser :\n\n"
    "• Pensez à regarder les chiffres séparés par des cases :\n"
    "  ils peuvent tout de même former une paire.\n\n"
    "• Analysez le plateau ligne par ligne :\n"
    "  la fin d’une ligne et le début de la suivante\n"
    "  peuvent cacher une paire compatible.\n\n"
    "• Prenez votre temps et observez bien les motifs répétés.\n",

    /* ------------------ Page 4 ------------------ */
    "Bonne chance et amusez-vous bien !\n\n"
    "Number Match est un jeu de logique et d’observation.\n"
    "Plus vous jouerez, plus vous repérerez les paires rapidement.\n"
    "Amusez-vous à optimiser vos choix et à battre vos scores !\n"
};


/***********************************************
 *         BASIC UI HELPER FUNCTIONS
 ***********************************************/

/*
 * hit_button()
 * ------------------------------------------------------
 * Checks if the mouse (mx,my) intersects a main menu
 * button located at (x,y) with fixed BTN_W × BTN_H size.
 */
static int hit_button(int mx, int my, int x, int y) {
    return mx >= x && mx <= x + BTN_W &&
           my >= y && my <= y + BTN_H;
}


/*
 * hit_small()
 * ------------------------------------------------------
 * Generic hit-detection for rectangles of ANY size.
 */
static int hit_small(int mx, int my, int x, int y, int w, int h) {
    return mx >= x && mx <= x + w &&
           my >= y && my <= y + h;
}


/***********************************************
 *        ROUNDED BUTTON DRAWING HELPERS
 ***********************************************/

/*
 * draw_round_button()
 * ------------------------------------------------------
 * Draws a filled rectangle with rounded corners.
 * Width/height are arbitrary; 'r' is corner radius.
 */
static void draw_round_button(int x, int y, int w, int h, int r, MLV_Color c) {

    /* Center rectangle */
    MLV_draw_filled_rectangle(x + r, y,     w - 2*r, h,       c);
    MLV_draw_filled_rectangle(x,     y + r, w,       h - 2*r, c);

    /* Corners */
    MLV_draw_filled_circle(x + r,     y + r,     r, c);
    MLV_draw_filled_circle(x + w - r, y + r,     r, c);
    MLV_draw_filled_circle(x + r,     y + h - r, r, c);
    MLV_draw_filled_circle(x + w - r, y + h - r, r, c);
}


/*
 * draw_button()
 * ------------------------------------------------------
 * Draws one of the 4 main menu buttons centered horizontally.
 * Highlights when hovered.
 */
static void draw_button(const char *label, int y, int hover) {

    int tw, th;
    int x = (GAME_WINDOW_WIDTCH - BTN_W) / 2;

    draw_round_button(x, y, BTN_W, BTN_H, BTN_R,
                      hover ? BTN_HOVER : BG_COLOR);

    MLV_get_size_of_text(label, &tw, &th);

    MLV_draw_text(
        x + BTN_W/2 - tw/2,
        y + BTN_H/2 - th/2,
        label,
        TEXT_COLOR
    );
}


/***********************************************
 *     TOP-RIGHT BACKGROUND MODE TOGGLE BUTTON
 ***********************************************/

static int bg_mouse_before = 0;

/*
 * hit_bg_button()
 * ------------------------------------------------------
 * Detects click region for "BG" toggle button.
 */
static int hit_bg_button(int mx, int my) {
    int x = GAME_WINDOW_WIDTCH - 60;
    int y = 20;
    return hit_small(mx, my, x, y, 40, 40);
}


/*
 * draw_bg_button()
 * ------------------------------------------------------
 * Displays a small rounded button with the label “BG”.
 */
static void draw_bg_button(int mx, int my) {

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


/***********************************************
 *                 ARROW DRAWING
 ***********************************************/

/*
 * draw_left_arrow()
 * ------------------------------------------------------
 * Draws a simple filled triangle for tutorial navigation.
 */
static void draw_left_arrow(int x, int y, int hover) {

    MLV_Color c = hover ? MLV_COLOR_BLUE : MLV_COLOR_BEIGE;
    int xs[3];
    int ys[3];

    xs[0] = x + ARROW_W;
    xs[1] = x + ARROW_W;
    xs[2] = x;

    ys[0] = y;
    ys[1] = y + ARROW_H;
    ys[2] = y + ARROW_H/2;

    MLV_draw_filled_polygon(xs, ys, 3, c);
}


/*
 * draw_right_arrow()
 * ------------------------------------------------------
 * Mirror version of the left arrow.
 */
static void draw_right_arrow(int x, int y, int hover) {

    MLV_Color c = hover ? MLV_COLOR_BLUE : MLV_COLOR_BEIGE;
    int xs[3];
    int ys[3];

    xs[0] = x;
    xs[1] = x;
    xs[2] = x + ARROW_W;

    ys[0] = y;
    ys[1] = y + ARROW_H;
    ys[2] = y + ARROW_H/2;

    MLV_draw_filled_polygon(xs, ys, 3, c);
}


/***********************************************
 *         SLIDE ANIMATION BETWEEN PAGES
 ***********************************************/

/*
 * animate_slide()
 * ------------------------------------------------------
 * Performs a horizontal sliding transition between
 * two tutorial pages.
 *
 * direction = +1  slide new page from right → left
 * direction = -1  slide new page from left → right
 */
static void animate_slide(const char* old_page,
                          const char* new_page,
                          int direction)
{
    int offset = 0;

    while (offset < GAME_WINDOW_WIDTCH) {

        /* Background clear */
        MLV_clear_window(MLV_rgba(12,16,35,255));

        /* OLD page sliding away */
        MLV_draw_text_box(
            40 - offset * direction,      /* moves off-screen */
            40,
            GAME_WINDOW_WIDTCH - 80,
            GAME_WINDOW_HEIGHT - 120,
            old_page,
            9,
            MLV_COLOR_BLACK,
            MLV_COLOR_BLACK,
            MLV_COLOR_BEIGE,
            MLV_TEXT_LEFT,
            MLV_HORIZONTAL_CENTER,
            MLV_VERTICAL_TOP
        );

        /* NEW page sliding in */
        MLV_draw_text_box(
            40 + (GAME_WINDOW_WIDTCH - offset) * direction,
            40,
            GAME_WINDOW_WIDTCH - 80,
            GAME_WINDOW_HEIGHT - 120,
            new_page,
            9,
            MLV_COLOR_BLACK,
            MLV_COLOR_BLACK,
            MLV_COLOR_BEIGE,
            MLV_TEXT_LEFT,
            MLV_HORIZONTAL_CENTER,
            MLV_VERTICAL_TOP
        );

        MLV_actualise_window();
        MLV_delay_according_to_frame_rate();

        offset += 40;
    }
}


/***********************************************
 *        BACKGROUND INITIALIZATION
 ***********************************************/

/*
 * init_stars()
 * ---------------------------------------------
 * Initializes star & dust arrays ONCE.
 * Used by background modes 1, 2, and 3.
 */
static void init_stars(void) {
    int i;

    /* Prevent reinitialization */
    if (stars_init) return;

    for (i = 0; i < STAR_COUNT; i++) {

        /* --- Stars (Modes 1 & 2) --- */
        star_x[i]         = rand() % GAME_WINDOW_WIDTCH;
        star_y[i]         = rand() % GAME_WINDOW_HEIGHT;
        star_speed[i]     = 1 + rand() % 2;
        star_alpha[i]     = 80 + rand() % 140;
        star_alpha_dir[i] = (rand() % 2) ? 1 : -1;

        /* --- Dust particles (Mode 3) --- */
        dust_x[i] = rand() % GAME_WINDOW_WIDTCH;
        dust_y[i] = rand() % GAME_WINDOW_HEIGHT;
        dust_dx[i] = (rand() % 3) - 1; /* -1, 0, or 1 */
    }

    stars_init = 1;
}


/***********************************************
 *        BACKGROUND DRAWING (ALL MODES)
 ***********************************************/

/*
 * draw_background_stars()
 * ---------------------------------------------
 * Called every frame by the main menu.
 * Draws one of the 4 background modes depending
 * on the global variable background_mode.
 *
 * Mode 1 — drifting stars
 * Mode 2 — twinkling stars
 * Mode 3 — falling nebula dust
 * Mode 4 — calm multi-layer parallax glow
 */
static void draw_background_stars(void) {
    int i;

    /* Ensure star arrays are created */
    init_stars();

    /* Default deep-space background color */
    MLV_clear_window(MLV_rgba(12,16,35,255));
    MLV_delay_according_to_frame_rate();

    /***********************************************
     * MODE 1 & 2 — REGULAR STARS
     ***********************************************/
    if (background_mode == 1 || background_mode == 2) {

        for (i = 0; i < STAR_COUNT; i++) {

            /* Draw star */
            MLV_draw_filled_circle(
                star_x[i],
                star_y[i],
                1,
                MLV_rgba(200,220,255,star_alpha[i])
            );

            /* --- Mode 1: drifting vertically --- */
            if (background_mode == 1) {
                star_y[i] += star_speed[i];
                if (star_y[i] >= GAME_WINDOW_HEIGHT)
                    star_y[i] = 0;
            }

            /* --- Mode 2: twinkling brightness --- */
            else {
                star_alpha[i] += star_alpha_dir[i] * 3;

                if (star_alpha[i] > 220) star_alpha_dir[i] = -1;
                if (star_alpha[i] < 60)  star_alpha_dir[i] =  1;
            }
        }
        return;
    }


    /***********************************************
     * MODE 3 — NEBULA DUST
     ***********************************************/
    if (background_mode == 3) {

        for (i = 0; i < STAR_COUNT; i++) {

            /* Soft glowing dust particle */
            MLV_draw_filled_circle(
                dust_x[i],
                dust_y[i],
                2,
                MLV_rgba(180,200,255,60)
            );

            /* Downward drift + slight horizontal drift */
            dust_y[i] += 1;
            dust_x[i] += dust_dx[i];

            /* Wrap particles back to the top */
            if (dust_y[i] >= GAME_WINDOW_HEIGHT) {
                dust_y[i] = 0;
                dust_x[i] = rand() % GAME_WINDOW_WIDTCH;
            }
        }
        return;
    }


    /***********************************************
     * MODE 4 — PARALLAX GLOW PARTICLES
     ***********************************************/
    if (background_mode == 4) {

        static int init = 0;
        static int px[60];
        static int py[60];
        static int layer[60];
        static int phase[60];

        /* Initialize parallax layers once */
        if (!init) {
            for (i = 0; i < 60; i++) {
                px[i]    = rand() % GAME_WINDOW_WIDTCH;
                py[i]    = rand() % GAME_WINDOW_HEIGHT;
                layer[i] = 1 + rand() % 3;   /* Depth layer: 1, 2, or 3 */
                phase[i] = rand() % 360;
            }
            init = 1;
        }

        /* New dark base for this mode */
        MLV_clear_window(MLV_rgba(14,18,40,255));
        MLV_delay_according_to_frame_rate();

        /* Draw glowing particles with parallax motion */
        for (i = 0; i < 60; i++) {

            int speed  = layer[i];  /* Layer controls speed */
            int radius = layer[i];  /* Layer controls particle size */

            px[i] += speed;

            /* Wrap horizontally */
            if (px[i] > GAME_WINDOW_WIDTCH)
                px[i] = 0;

            phase[i] += 2;
            if (phase[i] > 360) phase[i] = 0;

            /* Glow intensity oscillates with sine wave */
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
/***********************************************
 *               TUTORIAL SCREEN
 ***********************************************
 *
 * show_tutorial_screen()
 * ------------------------------------------------------
 * Displays the 4-page French tutorial.
 *
 * Navigation:
 *   ← / →         : previous / next page
 *   mouse arrows  : click to change page
 *   Enter / Esc   : exit tutorial
 *
 * Uses animate_slide() for page transitions.
 */
void show_tutorial_screen() {

    int page = 0;
    int running = 1;
    int mx = 0, my = 0;

    int arrow_left_x, arrow_left_y;
    int arrow_right_x, arrow_right_y;

    int nb_pages;

    /* Edge-detection states for keyboard & mouse */
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

        int hover_left, hover_right;
        int left_now, right_now, enter_now, esc_now;
        int mouse_now;

        MLV_get_mouse_position(&mx, &my);

        hover_left  = hit_small(mx,my,arrow_left_x, arrow_left_y, ARROW_W, ARROW_H);
        hover_right = hit_small(mx,my,arrow_right_x,arrow_right_y,ARROW_W, ARROW_H);

        /***********************************************
         *               DRAW PAGE
         ***********************************************/
        MLV_clear_window(MLV_rgba(12,16,35,255));

        MLV_draw_text_box(
            40, 40,
            GAME_WINDOW_WIDTCH - 80,
            GAME_WINDOW_HEIGHT - 200,
            tutorial_pages[page],
            9,
            MLV_COLOR_BLACK,
            MLV_COLOR_BLACK,
            MLV_COLOR_BEIGE,
            MLV_TEXT_LEFT,
            MLV_HORIZONTAL_CENTER,
            MLV_VERTICAL_TOP
        );

        {
            char buf[64];
            sprintf(buf, "Page %d / %d", page+1, nb_pages);

            MLV_draw_text(
                GAME_WINDOW_WIDTCH/2 - 40,
                GAME_WINDOW_HEIGHT - 120,
                buf,
                MLV_COLOR_BLUE
            );
        }

        draw_left_arrow (arrow_left_x,  arrow_left_y,  hover_left);
        draw_right_arrow(arrow_right_x, arrow_right_y, hover_right);

        MLV_draw_text(
            40, GAME_WINDOW_HEIGHT - 40,
            "Flèches GAUCHE/DROITE ou clic — Entrée/Echap pour quitter",
            MLV_COLOR_BLACK
        );

        MLV_actualise_window();

        /***********************************************
         *               INPUT HANDLING
         ***********************************************/
        (void)MLV_get_event(NULL,NULL,NULL,NULL,NULL,&mx,&my,NULL,NULL);

        /* ---------- Keyboard ---------- */
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

        if ((enter_now && !enter_before) || (esc_now && !esc_before)) {
            running = 0;
        }

        left_before  = left_now;
        right_before = right_now;
        enter_before = enter_now;
        esc_before   = esc_now;

        /* ---------- Mouse ---------- */
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
/***********************************************
 *          CONFIRM NEW GAME POPUP
 ***********************************************/
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

        int hover_yes, hover_no;

        MLV_get_mouse_position(&mx,&my);

        hover_yes = hit_small(mx,my,yes_x,by+120,btn_w,btn_h);
        hover_no  = hit_small(mx,my,no_x ,by+120,btn_w,btn_h);

        /* Dark overlay */
        MLV_draw_filled_rectangle(
            0,0,
            GAME_WINDOW_WIDTCH,
            GAME_WINDOW_HEIGHT,
            MLV_rgba(0,0,0,120)
        );

        /* White box */
        MLV_draw_filled_rectangle(bx,by,420,200,MLV_COLOR_WHITE);
        MLV_draw_rectangle(bx,by,420,200,MLV_COLOR_BLACK);

        MLV_draw_text(
            bx + 40, by + 40,
            "Supprimer la derniere sauvegarde ?",
            MLV_COLOR_BLACK
        );

        draw_round_button(
            yes_x, by+120, btn_w, btn_h, BTN_R,
            hover_yes ? BTN_HOVER : BG_COLOR
        );

        draw_round_button(
            no_x, by+120, btn_w, btn_h, BTN_R,
            hover_no ? BTN_HOVER : BG_COLOR
        );

        MLV_draw_text(yes_x + 45, by+140, "OUI", MLV_COLOR_BLUE);
        MLV_draw_text(no_x  + 45, by+140, "NON", MLV_COLOR_BLUE);

        MLV_actualise_window();

        if (MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED) {
            if (hover_yes) return 1;
            if (hover_no)  return 0;
        }
    }
}
/***********************************************
 *            INTRO BACKGROUND (NEBULA)
 ***********************************************/
static void draw_intro_background(double t) {

    int i;
    int cx = GAME_WINDOW_WIDTCH / 2;
    int cy = GAME_WINDOW_HEIGHT / 2;

    /* Dark deep-space base */
    MLV_clear_window(MLV_rgba(12,16,35,255));

    /* Nebula drifting particles */
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

    /* Sparse stars */
    for (i = 0; i < 40; i++) {
        int x = (i * 97) % GAME_WINDOW_WIDTCH;
        int y = (i * 53) % GAME_WINDOW_HEIGHT;
        MLV_draw_filled_circle(x, y, 1, MLV_rgba(200,220,255,80));
    }
}
/***********************************************
 *           ANIMATED TITLE RENDERER
 ***********************************************/
static void draw_animated_title(const char* text, float phase){
    int tw, th, cw;
    int x, y, i;
    char buf[2];
    buf[1] = '\0';

    MLV_get_size_of_text(text,&tw,&th);
    MLV_get_size_of_text("N",&cw,&th);

    x = GAME_WINDOW_WIDTCH/2 - tw/2;
    y = GAME_WINDOW_HEIGHT/6;

    for (i = 0; text[i]; i++) {

        int p = (i + (int)(phase * 2.0f)) % 4;
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
static void draw_animated_title_at_y(const char* text, float phase, int y){
    int tw, th, cw;
    int x, i;
    char buf[2];
    buf[1] = '\0';

    MLV_get_size_of_text(text,&tw,&th);
    MLV_get_size_of_text("N",&cw,&th);

    x = GAME_WINDOW_WIDTCH/2 - tw/2;

    for (i = 0; text[i]; i++) {

        int p = (i + (int)(phase * 2.0f)) % 4;
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
static void draw_number_spiral(double angle, double radius) {

    int cx = GAME_WINDOW_WIDTCH / 2;
    int cy = GAME_WINDOW_HEIGHT / 2;
    int i;

    const char* left_nums[]  = { "1","2","3","4","5" };
    const char* right_nums[] = { "9","8","7","6","5" };
    const int count = 5;

    for (i = 0; i < count; i++) {

        double a = angle + i * 0.4;
        double r = radius - i * 10;

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

/***********************************************
 *       INTRO SKIP (ENTER KEY) HELPER
 ***********************************************/
static int intro_skip_requested(void) {
    /* If Enter is pressed at any moment during the intro,
       we immediately leave the intro animation. */
    return (MLV_get_keyboard_state(MLV_KEYBOARD_RETURN) == MLV_PRESSED);
}

static void draw_intro_skip_hint(void) {
    int tw, th;

    MLV_get_size_of_text("(skip = ENTER)", &tw, &th);

    MLV_draw_text(
        GAME_WINDOW_WIDTCH/2 - tw/2,          /* centered horizontally */
        GAME_WINDOW_HEIGHT - th -50,         /* bottom margin */
        "(skip = ENTER)", MLV_COLOR_DARK_GREY          /* soft grey */
    );
}

/***********************************************
 *           FULL INTRO ANIMATION
 ***********************************************/
static void play_intro_animation(void) {
    double spiral_angle = 0.0;
    int wait;
    int cx;
    int cy;
    int xL;
    int xR;
    int target_left;
    int target_right;
    int speed;

    cx = GAME_WINDOW_WIDTCH / 2 - 2;
    cy = GAME_WINDOW_HEIGHT / 2;

    target_left  = GAME_WINDOW_WIDTCH / 2;
    target_right = GAME_WINDOW_WIDTCH / 2;
    speed = 5;

    xL = -80;
    xR = GAME_WINDOW_WIDTCH + 80;

    /* slide in together */
    while (xL < target_left || xR > target_right) {

        if (intro_skip_requested()) return;

        if (xL < target_left)  xL += speed;
        if (xR > target_right) xR -= speed;

        draw_spiral_stars(spiral_angle);
        spiral_angle += 0.05;

        MLV_draw_text(xL, cy, "5", MLV_COLOR_WHITE);
        MLV_draw_text(xR, cy, "5", MLV_COLOR_WHITE);

        draw_intro_skip_hint();
        MLV_actualise_window();
        MLV_delay_according_to_frame_rate();
    }

    /* hold 5 + 5 */
    for (wait = 0; wait < 30; wait++) {

        if (intro_skip_requested()) return;

        draw_spiral_stars(spiral_angle);
        spiral_angle += 0.05;

        MLV_draw_text(xL, cy, "5", MLV_COLOR_WHITE);
        MLV_draw_text(xR, cy, "5", MLV_COLOR_WHITE);

        draw_intro_skip_hint();
        MLV_actualise_window();
    }

    /* show 10 result */
    for (wait = 0; wait < 40; wait++) {

        if (intro_skip_requested()) return;

        draw_spiral_stars(spiral_angle);
        spiral_angle += 0.05;

        MLV_draw_text(cx, cy, "10", MLV_COLOR_YELLOW);

        draw_intro_skip_hint();
        MLV_actualise_window();
        MLV_delay_according_to_frame_rate();
    }

    /* 6 + 4 diagonal */
    {
        int xTL = -80, yTL = -80;
        int xBR = GAME_WINDOW_WIDTCH + 40, yBR = GAME_WINDOW_HEIGHT + 40;

        while (xTL < cx || yTL < cy || xBR > cx || yBR > cy) {

            if (intro_skip_requested()) return;

            if (xTL < cx) xTL += speed;
            if (yTL < cy) yTL += speed;

            if (xBR > cx) xBR -= speed;
            if (yBR > cy) yBR -= speed;

            draw_spiral_stars(spiral_angle);
            spiral_angle += 0.05;

            MLV_draw_text(xTL, yTL, "6", MLV_COLOR_WHITE);
            MLV_draw_text(xBR, yBR, "4", MLV_COLOR_WHITE);

            draw_intro_skip_hint();
            MLV_actualise_window();
            MLV_delay_according_to_frame_rate();
        }

        for (wait = 0; wait < 30; wait++) {

            if (intro_skip_requested()) return;

            draw_spiral_stars(spiral_angle);
            spiral_angle += 0.05;
            MLV_draw_text(cx, cy, "6", MLV_COLOR_WHITE);
            MLV_draw_text(cx, cy, "4", MLV_COLOR_WHITE);

            draw_intro_skip_hint();
            MLV_actualise_window();
        }

        for (wait = 0; wait < 40; wait++) {

            if (intro_skip_requested()) return;

            draw_spiral_stars(spiral_angle);
            spiral_angle += 0.05;
            MLV_draw_text(cx, cy, "10", MLV_COLOR_YELLOW);

            draw_intro_skip_hint();
            MLV_actualise_window();
            MLV_delay_according_to_frame_rate();
        }
    }

    /* 8 + 2 diagonal opposite */
    {
        int xTR = GAME_WINDOW_WIDTCH + 80, yTR = -80;
        int xBL = -80, yBL = GAME_WINDOW_HEIGHT + 80;

        while (xTR > cx || yTR < cy || xBL < cx || yBL > cy) {

            if (intro_skip_requested()) return;

            if (xTR > cx) xTR -= speed;
            if (yTR < cy) yTR += speed;

            if (xBL < cx) xBL += speed;
            if (yBL > cy) yBL -= speed;

            draw_spiral_stars(spiral_angle);
            spiral_angle += 0.05;

            MLV_draw_text(xTR, yTR, "8", MLV_COLOR_WHITE);
            MLV_draw_text(xBL, yBL, "2", MLV_COLOR_WHITE);

            draw_intro_skip_hint();
            MLV_actualise_window();
            MLV_delay_according_to_frame_rate();
        }

        for (wait = 0; wait < 30; wait++) {

            if (intro_skip_requested()) return;

            draw_spiral_stars(spiral_angle);
            spiral_angle += 0.05;

            MLV_draw_text(cx, cy, "8", MLV_COLOR_WHITE);
            MLV_draw_text(cx, cy, "2", MLV_COLOR_WHITE);

            draw_intro_skip_hint();
            MLV_actualise_window();
        }

        for (wait = 0; wait < 40; wait++) {

            if (intro_skip_requested()) return;

            draw_spiral_stars(spiral_angle);
            spiral_angle += 0.05;

            MLV_draw_text(cx, cy, "10", MLV_COLOR_YELLOW);

            draw_intro_skip_hint();
            MLV_actualise_window();
            MLV_delay_according_to_frame_rate();
        }
    }

    /* 3 + 7 vertical */
    {
        int yTop = -80;
        int yBot = GAME_WINDOW_HEIGHT + 80;

        while (yTop < cy || yBot > cy) {

            if (intro_skip_requested()) return;

            if (yTop < cy) yTop += speed;
            if (yBot > cy) yBot -= speed;

            draw_spiral_stars(spiral_angle);
            spiral_angle += 0.05;

            MLV_draw_text(xL, yTop, "3", MLV_COLOR_WHITE);
            MLV_draw_text(xR, yBot, "7", MLV_COLOR_WHITE);

            draw_intro_skip_hint();
            MLV_actualise_window();
            MLV_delay_according_to_frame_rate();
        }

        for (wait = 0; wait < 30; wait++) {

            if (intro_skip_requested()) return;

            draw_spiral_stars(spiral_angle);
            spiral_angle += 0.05;

            MLV_draw_text(xL, cy, "3", MLV_COLOR_WHITE);
            MLV_draw_text(xR, cy, "7", MLV_COLOR_WHITE);

            draw_intro_skip_hint();
            MLV_actualise_window();
        }

        for (wait = 0; wait < 40 ; wait++) {

            if (intro_skip_requested()) return;

            draw_spiral_stars(spiral_angle);
            spiral_angle += 0.05;

            MLV_draw_text(cx, cy, "10", MLV_COLOR_YELLOW);

            draw_intro_skip_hint();
            MLV_actualise_window();
            MLV_delay_according_to_frame_rate();
        }
    }

    /* spiral collapse */
    {
        double angle = 0.0;
        double radius = 260.0;

        while (radius > 40.0) {

            if (intro_skip_requested()) return;

            draw_spiral_stars(spiral_angle);
            draw_number_spiral(angle, radius);

            angle  += 0.05;
            radius -= 0.9;
            spiral_angle += 0.05;

            draw_intro_skip_hint();
            MLV_actualise_window();
            MLV_delay_according_to_frame_rate();
        }

        /* slide title upward */
        {
            int y_start = GAME_WINDOW_HEIGHT / 2;
            int y_end   = GAME_WINDOW_HEIGHT / 6;
            int y = y_start;
            float phase = 0.0f;

            while (y > y_end) {

                if (intro_skip_requested()) return;

                draw_spiral_stars(spiral_angle);
                spiral_angle += 0.05;

                draw_animated_title_at_y("Number Match", phase, y);
                phase += 0.02f;
                y -= 2;

                draw_intro_skip_hint();
                MLV_actualise_window();
                MLV_delay_according_to_frame_rate();
            }
        }
    }
}


/***********************************************
 *                 MAIN MENU
 ***********************************************/
void mlv_show_menu(struct game_config *config){

    int mx;
    int my;
    int running;

    float fade;
    int direction;

    int play_y;
    int load_y;
    int tut_y;
    int quit_y;

    int bx;

    mx = 0;
    my = 0;
    running = 1;

    fade = 0.001f;
    direction = 1;

    play_y = 230;
    load_y = 330;
    tut_y  = 430;
    quit_y = 530;

    MLV_create_window(
        "NumberMatch Menu",
        "NumberMatch",
        GAME_WINDOW_WIDTCH,
        GAME_WINDOW_HEIGHT
    );

    MLV_change_frame_rate(FRAME_RATE);

    play_intro_animation();

    MLV_ctext_animations_start();

    while (running) {

        MLV_Event ev;
        int r;
        int g;
        int b;
        int bg_mouse_now;

        MLV_get_mouse_position(&mx,&my);

        bg_mouse_now =
            (MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED);

        fade += 0.001f * direction;
        if (fade >= 1.0f) direction = -1;
        if (fade <= 0.0f) direction = 1;

        r = BASE_R - (int)(fade*35);
        g = BASE_G - (int)(fade*35);
        b = BASE_B - (int)(fade*10);

        (void)r;
        (void)g;
        (void)b; /* kept to avoid warnings if not used later */

        /* background (modes 1–4) */
        draw_background_stars();

        /* Animated title */
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
