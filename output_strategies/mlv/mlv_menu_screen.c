/**
 * @file mlv_menu_screen.c
 * @brief Menu, intro, tutorial and animated backgrounds for Number Match.
 */

#include "mlv_menu_screen.h"
#include <MLV/MLV_all.h>
#include <stdlib.h>
#include <math.h>

/* ============================================================================
 *                                 CONSTANTS
 * ==========================================================================*/

/** @name Menu button geometry */
/** @{ */
#define BTN_W 160
#define BTN_H 60
#define BTN_R 18
/** @} */

/** @name Base color components */
/** @{ */
#define BASE_R 255
#define BASE_G 247
#define BASE_B 229
/** @} */

/** @name UI colors */
/** @{ */
#define BTN_COLOR MLV_rgba(0,0,128,255)
#define BTN_HOVER MLV_rgba(70,130,255,255)
#define TEXT_COLOR MLV_COLOR_BLUE
#define BG_COLOR   MLV_COLOR_BEIGE
/** @} */

/** @name Navigation & background constants */
/** @{ */
#define ARROW_W 60
#define ARROW_H 60
#define STAR_COUNT 80
#define TUTORIAL_NB_PAGES 4
/** @} */


/* ============================================================================
 *                                   ENUMS
 * ==========================================================================*/

/**
 * @enum BackgroundMode
 * @brief Available background animation modes.
 */
enum BackgroundMode {
    BG_DRIFTING = 1, /**< Stars drift downward */
    BG_TWINKLING,    /**< Stars twinkle via alpha changes */
    BG_NEBULA,       /**< Dust particles fall + slight horizontal drift */
    BG_PARALLAX      /**< Parallax layered drifting particles */
};

/* ============================================================================
 *                              STATE STRUCTURES
 * ==========================================================================*/

/**
 * @struct StarsState
 * @brief State for drifting/twinkling star background.
 */
struct StarsState {
    int x[STAR_COUNT];        /**< Star X positions */
    int y[STAR_COUNT];        /**< Star Y positions */
    int speed[STAR_COUNT];    /**< Vertical drift speed */
    int alpha[STAR_COUNT];    /**< Current alpha */
    int alpha_dir[STAR_COUNT];/**< Alpha direction (+1 or -1) */
};

/**
 * @struct DustState
 * @brief State for nebula dust background.
 */
struct DustState {
    int x[STAR_COUNT]; /**< Dust X positions */
    int y[STAR_COUNT]; /**< Dust Y positions */
    int dx[STAR_COUNT];/**< Horizontal drift (-1..+1) */
};

/**
 * @struct ParallaxState
 * @brief State for parallax background particles.
 */
struct ParallaxState {
    int init;      /**< 0 until initialized */
    int px[60];    /**< Particle X positions */
    int py[60];    /**< Particle Y positions */
    int layer[60]; /**< Layer (1..3) controls size & speed */
    int phase[60]; /**< Phase for alpha sinusoid */
};

/* ============================================================================
 *                           BASIC UI HELPER FUNCTIONS
 * ==========================================================================*/

/**
 * @brief Test if the mouse position hits a standard menu button.
 * @param mx Mouse X
 * @param my Mouse Y
 * @param x  Button X (top-left)
 * @param y  Button Y (top-left)
 * @return 1 if hit, 0 otherwise
 */
int hit_button(int mx, int my, int x, int y) {
    return mx >= x && mx <= x + BTN_W &&
           my >= y && my <= y + BTN_H;
}

/**
 * @brief Test if the mouse position hits a custom rectangle area.
 * @param mx Mouse X
 * @param my Mouse Y
 * @param x  Rect X (top-left)
 * @param y  Rect Y (top-left)
 * @param w  Rect width
 * @param h  Rect height
 * @return 1 if hit, 0 otherwise
 */
int hit_small(int mx, int my, int x, int y, int w, int h) {
    return mx >= x && mx <= x + w &&
           my >= y && my <= y + h;
}

/* ============================================================================
 *                        ROUNDED BUTTON DRAWING HELPERS
 * ==========================================================================*/

/**
 * @brief Draw a filled rounded rectangle using rectangles + circles.
 * @param x Top-left X
 * @param y Top-left Y
 * @param w Width
 * @param h Height
 * @param r Corner radius
 * @param c Fill color
 */
void draw_round_button(int x, int y, int w, int h, int r, MLV_Color c) {
    MLV_draw_filled_rectangle(x + r, y,     w - 2*r, h,       c);
    MLV_draw_filled_rectangle(x,     y + r, w,       h - 2*r, c);

    MLV_draw_filled_circle(x + r,     y + r,     r, c);
    MLV_draw_filled_circle(x + w - r, y + r,     r, c);
    MLV_draw_filled_circle(x + r,     y + h - r, r, c);
    MLV_draw_filled_circle(x + w - r, y + h - r, r, c);
}

/**
 * @brief Draw a centered menu button (fixed size BTN_W x BTN_H) with label.
 * @param label Text to draw
 * @param y     Button Y (top-left)
 * @param hover Non-zero if hovered
 */
void draw_button(const char *label, int y, int hover) {
    int tw, th;
    int x;

    x = (GAME_WINDOW_WIDTCH - BTN_W) / 2;

    draw_round_button(
        x, y, BTN_W, BTN_H, BTN_R,
        hover ? BTN_HOVER : BG_COLOR
    );

    MLV_get_size_of_text(label, &tw, &th);
    MLV_draw_text(
        x + BTN_W/2 - tw/2,
        y + BTN_H/2 - th/2,
        label,
        TEXT_COLOR
    );
}

/* ============================================================================
 *                   TOP-RIGHT BACKGROUND MODE TOGGLE BUTTON
 * ==========================================================================*/

/**
 * @brief Hit-test for background mode toggle button ("BG") at top-right.
 * @param mx Mouse X
 * @param my Mouse Y
 * @return 1 if hit, 0 otherwise
 */
int hit_bg_button(int mx, int my) {
    int x, y;
    x = GAME_WINDOW_WIDTCH - 60;
    y = 20;
    return hit_small(mx, my, x, y, 40, 40);
}

/**
 * @brief Draw background toggle button ("BG") at top-right.
 * @param mx Mouse X
 * @param my Mouse Y
 */
void draw_bg_button(int mx, int my) {
    int x, y, w, h;
    int hover;

    x = GAME_WINDOW_WIDTCH - 60;
    y = 20;
    w = 40;
    h = 40;

    hover = hit_bg_button(mx, my);

    draw_round_button(
        x, y, w, h, 8,
        hover ? BTN_HOVER : BG_COLOR
    );

    MLV_draw_text(x + 10, y + 10, "BG", TEXT_COLOR);
}

/* ============================================================================
 *                               ARROW DRAWING
 * ==========================================================================*/

/**
 * @brief Draw a left arrow triangle used for tutorial navigation.
 * @param x     Arrow top-left X
 * @param y     Arrow top-left Y
 * @param hover Non-zero if hovered
 */
void draw_left_arrow(int x, int y, int hover) {
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

/**
 * @brief Draw a right arrow triangle used for tutorial navigation.
 * @param x     Arrow top-left X
 * @param y     Arrow top-left Y
 * @param hover Non-zero if hovered
 */
void draw_right_arrow(int x, int y, int hover) {
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

/* ============================================================================
 *                      SLIDE ANIMATION BETWEEN PAGES
 * ==========================================================================*/

/**
 * @brief Animate a sliding transition between tutorial pages.
 * @param old_page  Text of current page
 * @param new_page  Text of next page
 * @param direction -1: slide to previous, +1: slide to next
 */
void animate_slide(const char* old_page, const char* new_page, int direction) {
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
            MLV_COLOR_BEIGE,
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

/* ============================================================================
 *                        BACKGROUND INITIALIZATION
 * ==========================================================================*/

/**
 * @brief Initialize background states (stars, dust, parallax).
 * @param s Stars state (output)
 * @param d Dust state (output)
 * @param p Parallax state (output)
 */
void init_background(struct StarsState* s, struct DustState* d, struct ParallaxState* p) {
    int i;

    for (i = 0; i < STAR_COUNT; i++) {
        s->x[i]         = rand() % GAME_WINDOW_WIDTCH;
        s->y[i]         = rand() % GAME_WINDOW_HEIGHT;
        s->speed[i]     = 1 + rand() % 2;
        s->alpha[i]     = 80 + rand() % 140;
        s->alpha_dir[i] = (rand() % 2) ? 1 : -1;

        d->x[i]  = rand() % GAME_WINDOW_WIDTCH;
        d->y[i]  = rand() % GAME_WINDOW_HEIGHT;
        d->dx[i] = (rand() % 3) - 1;
    }

    p->init = 0;
}

/* ============================================================================
 *                       BACKGROUND DRAWING (ALL MODES)
 * ==========================================================================*/

/**
 * @brief Draw the animated background based on mode.
 * @param mode Background mode
 * @param s    Stars state
 * @param d    Dust state
 * @param p    Parallax state
 */
void draw_background(
    enum BackgroundMode mode,
    struct StarsState* s,
    struct DustState* d,
    struct ParallaxState* p
) {
    int i;

    if (mode == BG_PARALLAX) {
        MLV_clear_window(MLV_rgba(14,18,40,255));
    } else {
        MLV_clear_window(MLV_rgba(12,16,35,255));
    }

    if (mode == BG_DRIFTING || mode == BG_TWINKLING) {

        for (i = 0; i < STAR_COUNT; i++) {

            MLV_draw_filled_circle(
                s->x[i], s->y[i], 1,
                MLV_rgba(200,220,255, s->alpha[i])
            );

            if (mode == BG_DRIFTING) {
                s->y[i] += s->speed[i];
                if (s->y[i] >= GAME_WINDOW_HEIGHT) {
                    s->y[i] = 0;
                }
            } else {
                s->alpha[i] += s->alpha_dir[i] * 3;
                if (s->alpha[i] > 220) s->alpha_dir[i] = -1;
                if (s->alpha[i] < 60)  s->alpha_dir[i] =  1;
            }
        }
    }
    else if (mode == BG_NEBULA) {

        for (i = 0; i < STAR_COUNT; i++) {

            MLV_draw_filled_circle(
                d->x[i], d->y[i], 2,
                MLV_rgba(180,200,255,60)
            );

            d->y[i] += 1;
            d->x[i] += d->dx[i];

            if (d->y[i] >= GAME_WINDOW_HEIGHT) {
                d->y[i] = 0;
                d->x[i] = rand() % GAME_WINDOW_WIDTCH;
            }
        }
    }
    else if (mode == BG_PARALLAX) {

        if (!p->init) {
            for (i = 0; i < 60; i++) {
                p->px[i]    = rand() % GAME_WINDOW_WIDTCH;
                p->py[i]    = rand() % GAME_WINDOW_HEIGHT;
                p->layer[i] = 1 + rand() % 3;
                p->phase[i] = rand() % 360;
            }
            p->init = 1;
        }

        for (i = 0; i < 60; i++) {

            int speed;
            int radius;
            int a;

            speed  = p->layer[i];
            radius = p->layer[i];

            p->px[i] += speed;
            if (p->px[i] > GAME_WINDOW_WIDTCH) p->px[i] = 0;

            p->phase[i] += 2;
            if (p->phase[i] > 360) p->phase[i] = 0;

            a = 80 + (int)(20.0 * sin(p->phase[i] * 0.017453292519943295));

            MLV_draw_filled_circle(
                p->px[i],
                p->py[i],
                radius,
                MLV_rgba(
                    160 + p->layer[i] * 20,
                    180 + p->layer[i] * 15,
                    255,
                    a
                )
            );
        }
    }

    /* frame pacing */
    MLV_delay_according_to_frame_rate();
}

/* ============================================================================
 *                         ANIMATED TITLE RENDERER
 * ==========================================================================*/

/**
 * @brief Draw animated title at a default Y position.
 * @param text  Title string
 * @param phase Animation phase (used to shift colors)
 */
void draw_animated_title(const char* text, float phase){
    int tw, th, cw;
    int x, y, i;
    char buf[2];

    buf[1] = '\0';

    MLV_get_size_of_text(text, &tw, &th);
    MLV_get_size_of_text("N", &cw, &th);

    x = GAME_WINDOW_WIDTCH/2 - tw/2;
    y = GAME_WINDOW_HEIGHT/6;

    for (i = 0; text[i]; i++) {

        int p;
        MLV_Color c;

        p = (i + (int)(phase * 2.0f)) % 4;

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

/**
 * @brief Draw animated title at a specific Y position.
 * @param text  Title string
 * @param phase Animation phase (used to shift colors)
 * @param y     Y position for text
 */
void draw_animated_title_at_y(const char* text, float phase, int y){
    int tw, th, cw;
    int x, i;
    char buf[2];

    buf[1] = '\0';

    MLV_get_size_of_text(text, &tw, &th);
    MLV_get_size_of_text("N", &cw, &th);

    x = GAME_WINDOW_WIDTCH/2 - tw/2;

    for (i = 0; text[i]; i++) {

        int p;
        MLV_Color c;

        p = (i + (int)(phase * 2.0f)) % 4;

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

/* ============================================================================
 *                           NUMBER SPIRAL DRAW
 * ==========================================================================*/

/**
 * @brief Draw a small number spiral (used in intro collapse).
 * @param angle  Current angle
 * @param radius Current radius
 */
void draw_number_spiral(double angle, double radius) {
    int cx, cy;
    int i;
    const char* left_nums[5];
    const char* right_nums[5];

    cx = GAME_WINDOW_WIDTCH / 2;
    cy = GAME_WINDOW_HEIGHT / 2;

    left_nums[0] = "1"; left_nums[1] = "2"; left_nums[2] = "3"; left_nums[3] = "4"; left_nums[4] = "5";
    right_nums[0]= "9"; right_nums[1]= "8"; right_nums[2]= "7"; right_nums[3]= "6"; right_nums[4]= "5";

    for (i = 0; i < 5; i++) {

        double a;
        double r;
        int xL, yL, xR, yR;

        a = angle + i * 0.4;
        r = radius - i * 10.0;

        xL = cx + (int)(cos(a) * r);
        yL = cy + (int)(sin(a) * r);

        xR = cx + (int)(cos(a + 0.4) * r);
        yR = cy + (int)(sin(a + 0.4) * r);

        if (r > 0.0) {
            MLV_draw_text(xL, yL, left_nums[i],  MLV_COLOR_WHITE);
            MLV_draw_text(xR, yR, right_nums[i], MLV_COLOR_WHITE);
        }
    }
}

/* ============================================================================
 *                               INTRO SKIP HINT
 * ==========================================================================*/

/**
 * @brief Check if ENTER is pressed to skip intro.
 * @return 1 if skip requested, 0 otherwise
 */
int intro_skip_requested(void) {
    return (MLV_get_keyboard_state(MLV_KEYBOARD_RETURN) == MLV_PRESSED);
}

/**
 * @brief Draw skip hint at bottom center.
 */
void draw_intro_skip_hint(void) {
    int tw, th;

    MLV_get_size_of_text("(skip = ENTER)", &tw, &th);

    MLV_draw_text(
        GAME_WINDOW_WIDTCH/2 - tw/2,
        GAME_WINDOW_HEIGHT - th - 50,
        "(skip = ENTER)",
        MLV_COLOR_DARK_GREY
    );
}

/* ============================================================================
 *                           INTRO BACKGROUND (NEBULA)
 * ==========================================================================*/

/**
 * @brief Draw intro nebula-like background (animated using time parameter).
 * @param t Time parameter / phase
 */
void draw_intro_background(double t) {
    int i;
    int cx, cy;

    cx = GAME_WINDOW_WIDTCH / 2;
    cy = GAME_WINDOW_HEIGHT / 2;

    MLV_clear_window(MLV_rgba(12,16,35,255));

    for (i = 0; i < 140; i++) {

        double dx, dy;
        int x, y;

        dx = sin(t * 0.4 + i * 0.7) * 120.0;
        dy = cos(t * 0.3 + i * 0.9) * 80.0;

        x = cx + (int)dx + (i * 13) % GAME_WINDOW_WIDTCH;
        y = cy + (int)dy + (i * 17) % GAME_WINDOW_HEIGHT;

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

    for (i = 0; i < 40; i++) {
        int x, y;
        x = (i * 97) % GAME_WINDOW_WIDTCH;
        y = (i * 53) % GAME_WINDOW_HEIGHT;
        MLV_draw_filled_circle(x, y, 1, MLV_rgba(200,220,255,80));
    }
}

/* ============================================================================
 *                           FULL INTRO ANIMATION
 * ==========================================================================*/

/**
 * @brief Play the full intro animation (skippable with ENTER).
 */
void play_intro_animation(void) {
    double spiral_angle;
    int wait;
    int running;

    int cx, cy;
    int xL, xR;
    int target_left, target_right;
    int speed;

    spiral_angle = 0.0;
    running = 1;

    cx = GAME_WINDOW_WIDTCH / 2 - 2;
    cy = GAME_WINDOW_HEIGHT / 2;

    target_left  = GAME_WINDOW_WIDTCH / 2;
    target_right = GAME_WINDOW_WIDTCH / 2;
    speed = 5;

    xL = -80;
    xR = GAME_WINDOW_WIDTCH + 80;

    while (running && (xL < target_left || xR > target_right)) {

        if (intro_skip_requested()) running = 0;

        if (xL < target_left)  xL += speed;
        if (xR > target_right) xR -= speed;

        draw_intro_background(spiral_angle);
        spiral_angle += 0.05;

        MLV_draw_text(xL, cy, "5", MLV_COLOR_WHITE);
        MLV_draw_text(xR, cy, "5", MLV_COLOR_WHITE);

        draw_intro_skip_hint();
        MLV_actualise_window();
        MLV_delay_according_to_frame_rate();
    }

    for (wait = 0; running && wait < 30; wait++) {
        if (intro_skip_requested()) running = 0;

        draw_intro_background(spiral_angle);
        spiral_angle += 0.05;

        MLV_draw_text(xL, cy, "5", MLV_COLOR_WHITE);
        MLV_draw_text(xR, cy, "5", MLV_COLOR_WHITE);

        draw_intro_skip_hint();
        MLV_actualise_window();
        MLV_delay_according_to_frame_rate();
    }

    for (wait = 0; running && wait < 40; wait++) {
        if (intro_skip_requested()) running = 0;

        draw_intro_background(spiral_angle);
        spiral_angle += 0.05;

        MLV_draw_text(cx, cy, "10", MLV_COLOR_YELLOW);

        draw_intro_skip_hint();
        MLV_actualise_window();
        MLV_delay_according_to_frame_rate();
    }

    /* spiral collapse + title slide */
    if (running) {
        double angle;
        double radius;

        angle = 0.0;
        radius = 260.0;

        while (running && radius > 40.0) {

            if (intro_skip_requested()) running = 0;

            draw_intro_background(spiral_angle);
            draw_number_spiral(angle, radius);

            angle += 0.05;
            radius -= 0.9;
            spiral_angle += 0.05;

            draw_intro_skip_hint();
            MLV_actualise_window();
            MLV_delay_according_to_frame_rate();
        }

        if (running) {
            int y_start, y_end, y;
            float phase;

            y_start = GAME_WINDOW_HEIGHT / 2;
            y_end   = GAME_WINDOW_HEIGHT / 6;
            y = y_start;
            phase = 0.0f;

            while (running && y > y_end) {

                if (intro_skip_requested()) running = 0;

                draw_intro_background(spiral_angle);
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

/* ============================================================================
 *                               TUTORIAL SCREEN
 * ==========================================================================*/

/**
 * @brief Show the tutorial screen with multiple pages and slide transitions.
 *
 * Navigation:
 * - Left/Right arrows or LEFT/RIGHT keys
 * - ENTER or ESC exits tutorial
 */
void show_tutorial_screen(void) {

    /* ===== DECLARATIONS (C89: MUST BE FIRST) ===== */
    const char* tutorial_pages[TUTORIAL_NB_PAGES] = {
        "Bienvenue dans Number Match !\n\n"
        "Le but du jeu est simple : effacer tous les chiffres du plateau.\n\n"
        "Pour cela, trouvez :\n"
        "• Deux chiffres identiques\n"
        "• Ou deux chiffres dont la somme fait 10\n\n"
        "Sélectionnez les chiffres un par un pour les rayer,\n"
        "et progressez jusqu’à effacer tout le plateau !\n",

        "Règles essentielles :\n\n"
        "• Chaque paire trouvée vous fait gagner des points.\n"
        "• Lorsque toutes les cases d’une ligne disparaissent,\n"
        "  une nouvelle ligne peut apparaître selon les règles du jeu.\n\n"
        "• Les paires peuvent se faire :\n"
        "  - horizontalement\n"
        "  - verticalement\n"
        "  - diagonalement\n\n"
        "Les diagonales opposées peuvent aussi former des paires !\n",

        "Astuces pour progresser :\n\n"
        "• Pensez à regarder les chiffres séparés par des cases :\n"
        "  ils peuvent tout de même former une paire.\n\n"
        "• Parfois, les chiffres alignés plus loin\n"
        "  peuvent cacher une paire compatible.\n\n"
        "• Prenez votre temps et observez bien les motifs répétés.\n",

        "Bonne chance et amusez-vous bien !\n\n"
        "Number Match est un jeu de logique et d’observation.\n"
        "Plus vous jouerez, plus vous repérerez les paires rapidement.\n"
        "Amusez-vous à optimiser vos choix et à battre vos scores !\n"
    };

    int page;
    int running;
    int mx, my;

    int arrow_left_x, arrow_left_y;
    int arrow_right_x, arrow_right_y;

    int left_before, right_before, enter_before, esc_before;
    int mouse_before;
    int sliding;

    /* ===== INITIALISATION ===== */
    page = 0;
    running = 1;
    mx = 0;
    my = 0;
    sliding = 0;

    arrow_left_y  = GAME_WINDOW_HEIGHT - 150;
    arrow_right_y = GAME_WINDOW_HEIGHT - 150;
    arrow_left_x  = 60;
    arrow_right_x = GAME_WINDOW_WIDTCH - 60 - ARROW_W;

    left_before  = 0;
    right_before = 0;
    enter_before = 0;
    esc_before   = 0;
    mouse_before = 0;

    /* Flush mouse state */
    while (MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED) {
        MLV_delay_according_to_frame_rate();
    }
    /* Flush keyboard state */
    while (MLV_get_keyboard_state(MLV_KEYBOARD_RETURN) == MLV_PRESSED ||
           MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED ||
           MLV_get_keyboard_state(MLV_KEYBOARD_LEFT)   == MLV_PRESSED ||
           MLV_get_keyboard_state(MLV_KEYBOARD_RIGHT)  == MLV_PRESSED) {
        MLV_delay_according_to_frame_rate();
    }

    /* ===== MAIN LOOP ===== */
    while (running) {

        int hover_left, hover_right;
        int left_now, right_now, enter_now, esc_now;
        int mouse_now;
        char buf[64];

        MLV_get_mouse_position(&mx, &my);

        hover_left  = hit_small(mx, my, arrow_left_x,  arrow_left_y,  ARROW_W, ARROW_H);
        hover_right = hit_small(mx, my, arrow_right_x, arrow_right_y, ARROW_W, ARROW_H);

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

        sprintf(buf, "Page %d / %d", page + 1, TUTORIAL_NB_PAGES);
        MLV_draw_text(
            GAME_WINDOW_WIDTCH/2 - 40,
            GAME_WINDOW_HEIGHT - 120,
            buf,
            MLV_COLOR_BLUE
        );

        draw_left_arrow (arrow_left_x,  arrow_left_y,  hover_left);
        draw_right_arrow(arrow_right_x, arrow_right_y, hover_right);

        MLV_actualise_window();

        /* ===== INPUT ===== */
        left_now  = (MLV_get_keyboard_state(MLV_KEYBOARD_LEFT)   == MLV_PRESSED);
        right_now = (MLV_get_keyboard_state(MLV_KEYBOARD_RIGHT)  == MLV_PRESSED);
        enter_now = (MLV_get_keyboard_state(MLV_KEYBOARD_RETURN) == MLV_PRESSED);
        esc_now   = (MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED);

        if (!sliding) {

            if (left_now && !left_before && page > 0) {
                sliding = 1;
                animate_slide(tutorial_pages[page], tutorial_pages[page-1], -1);
                page--;
                sliding = 0;
            }

            if (right_now && !right_before && page < TUTORIAL_NB_PAGES - 1) {
                sliding = 1;
                animate_slide(tutorial_pages[page], tutorial_pages[page+1], +1);
                page++;
                sliding = 0;
            }

            if ((enter_now && !enter_before) || (esc_now && !esc_before)) {
                running = 0;
            }
        }

        left_before  = left_now;
        right_before = right_now;
        enter_before = enter_now;
        esc_before   = esc_now;

        mouse_now = (MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED);

        if (!sliding && mouse_now && !mouse_before) {

            if (hover_left && page > 0) {
                animate_slide(tutorial_pages[page], tutorial_pages[page-1], -1);
                page--;
            }
            else if (hover_right && page < TUTORIAL_NB_PAGES - 1) {
                animate_slide(tutorial_pages[page], tutorial_pages[page+1], +1);
                page++;
            }
        }

        mouse_before = mouse_now;
        MLV_delay_according_to_frame_rate();
    }
}

/* ============================================================================
 *                          CONFIRM NEW GAME POPUP
 * ==========================================================================*/

/**
 * @brief Confirmation popup before starting a new game.
 *
 * Displays a modal overlay with Yes/No buttons.
 *
 * @return 1 if user clicked YES, 0 if user clicked NO
 */
int confirm_new_game(void){
    int mx, my;
    int bx, by;
    int yes_x, no_x;
    int btn_w, btn_h;
    int decided;
    int result;

    mx = 0;
    my = 0;
    btn_w = 120;
    btn_h = 60;

    bx = (GAME_WINDOW_WIDTCH - 420) / 2;
    by = (GAME_WINDOW_HEIGHT - 200) / 2;

    yes_x = bx + 20;
    no_x  = bx + 280;

    decided = 0;
    result = 0;

    while (!decided) {

        int hover_yes, hover_no;
        int mouse_now;

        MLV_get_mouse_position(&mx, &my);

        hover_yes = hit_small(mx, my, yes_x, by + 120, btn_w, btn_h);
        hover_no  = hit_small(mx, my, no_x,  by + 120, btn_w, btn_h);

        /* Dark overlay */
        MLV_draw_filled_rectangle(
            0, 0,
            GAME_WINDOW_WIDTCH,
            GAME_WINDOW_HEIGHT,
            MLV_rgba(0,0,0,120)
        );

        /* White box */
        MLV_draw_filled_rectangle(bx, by, 420, 200, MLV_COLOR_WHITE);
        MLV_draw_rectangle(bx, by, 420, 200, MLV_COLOR_BLACK);

        MLV_draw_text(
            bx + 40, by + 40,
            "Supprimer la derniere sauvegarde ?",
            MLV_COLOR_BLACK
        );

        draw_round_button(
            yes_x, by + 120, btn_w, btn_h, BTN_R,
            hover_yes ? BTN_HOVER : BG_COLOR
        );

        draw_round_button(
            no_x, by + 120, btn_w, btn_h, BTN_R,
            hover_no ? BTN_HOVER : BG_COLOR
        );

        MLV_draw_text(yes_x + 45, by + 140, "OUI", MLV_COLOR_BLUE);
        MLV_draw_text(no_x  + 45, by + 140, "NON", MLV_COLOR_BLUE);

        MLV_actualise_window();

        mouse_now = (MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED);
        if (mouse_now) {
            if (hover_yes) { result = 1; decided = 1; }
            else if (hover_no) { result = 0; decided = 1; }
        }

        MLV_delay_according_to_frame_rate();
    }

    return result;
}

/* ============================================================================
 *                                 MAIN MENU
 * ==========================================================================*/

/**
 * @brief Show the main menu window and handle user interactions.
 *
 * - Plays intro animation once
 * - Allows switching background modes via "BG" button
 * - Buttons: New game, Load game, Tutorial, Quit
 *
 * @param config Game configuration pointer (passed to start/load)
 */
void mlv_show_menu(struct game_config *config){

    struct StarsState stars;
    struct DustState dust;
    struct ParallaxState parallax;

    enum BackgroundMode background_mode;

    int mx, my;
    int running;

    float fade;
    int direction;

    int play_y;
    int load_y;
    int tut_y;
    int quit_y;

    int bx;

    int bg_mouse_before;

    MLV_create_window("NumberMatch Menu", "NumberMatch",
                      GAME_WINDOW_WIDTCH, GAME_WINDOW_HEIGHT);

    MLV_change_frame_rate(FRAME_RATE);

    mx = 0;
    my = 0;
    running = 1;

    fade = 0.001f;
    direction = 1;

    play_y = 230;
    load_y = 330;
    tut_y  = 430;
    quit_y = 530;

    background_mode = BG_TWINKLING;
    bg_mouse_before = 0;

    init_background(&stars, &dust, &parallax);

    play_intro_animation();

    MLV_ctext_animations_start();

    while (running) {

        int bg_mouse_now;
        int bx_center;

        fade += 0.001f * (float)direction;
        if (fade >= 1.0f) direction = -1;
        if (fade <= 0.0f) direction =  1;

        MLV_get_mouse_position(&mx, &my);
        bg_mouse_now = (MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED);

        /* Background (modes 1–4) */
        draw_background(background_mode, &stars, &dust, &parallax);

        /* Animated title */
        draw_animated_title("NUMBER MATCH", fade);

        bx_center = (GAME_WINDOW_WIDTCH - BTN_W) / 2;
        bx = bx_center;

        /* BG toggle button */
        draw_bg_button(mx, my);

        /* Main buttons */
        draw_button("NOUVELLE PARTIE", play_y, hit_button(mx,my,bx,play_y));
        draw_button("CHARGER PARTIE",  load_y, hit_button(mx,my,bx,load_y));
        draw_button("TUTORIEL",        tut_y,  hit_button(mx,my,bx,tut_y));
        draw_button("QUITTER",         quit_y, hit_button(mx,my,bx,quit_y));

        MLV_actualise_window();

        /* BG button edge-detected */
        if (hit_bg_button(mx, my) && bg_mouse_now && !bg_mouse_before) {
            background_mode = (enum BackgroundMode)((int)background_mode + 1);
            if ((int)background_mode > (int)BG_PARALLAX) {
                background_mode = BG_DRIFTING;
            }
        }

        /* Clicks on main buttons edge-detected too */
        if (bg_mouse_now && !bg_mouse_before) {

            if (hit_button(mx,my,bx,play_y)) {
                /* If you want "confirm before starting" swap order */
                if (confirm_new_game()) {
                    start_game(config);
                }
            }
            else if (hit_button(mx,my,bx,load_y)) {
                load_game(config);
            }
            else if (hit_button(mx,my,bx,tut_y)) {
                show_tutorial_screen();
            }
            else if (hit_button(mx,my,bx,quit_y)) {
                running = 0;
            }
        }

        bg_mouse_before = bg_mouse_now;
        MLV_delay_according_to_frame_rate();
    }

    MLV_ctext_animations_end();
    MLV_free_window();
}
