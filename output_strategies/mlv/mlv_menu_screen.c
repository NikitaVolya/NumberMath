/**
 * @file mlv_menu_screen.c
 * @brief Menu, intro, tutorial and animated backgrounds for Number Match.
 */

#include "mlv_menu_screen.h"
#include <MLV/MLV_all.h>
#include <stdlib.h>
#include <math.h>

/* ============================================================================
 *                           BASIC UI HELPER FUNCTIONS
 * ==========================================================================*/

int hit_button(int mx, int my, int x, int y) {
    return mx >= x && mx <= x + BTN_W &&
           my >= y && my <= y + BTN_H;
}

int hit_small(int mx, int my, int x, int y, int w, int h) {
    return mx >= x && mx <= x + w &&
           my >= y && my <= y + h;
}

/* ============================================================================
 *                        ROUNDED BUTTON DRAWING HELPERS
 * ==========================================================================*/

void draw_round_button(int x, int y, int w, int h, int r, MLV_Color c) {
    MLV_draw_filled_rectangle(x + r, y,     w - 2*r, h,       c);
    MLV_draw_filled_rectangle(x,     y + r, w,       h - 2*r, c);

    MLV_draw_filled_circle(x + r,     y + r,     r, c);
    MLV_draw_filled_circle(x + w - r, y + r,     r, c);
    MLV_draw_filled_circle(x + r,     y + h - r, r, c);
    MLV_draw_filled_circle(x + w - r, y + h - r, r, c);
}

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

int hit_bg_button(int mx, int my) {
    int x, y;
    x = GAME_WINDOW_WIDTCH - 60;
    y = 20;
    return hit_small(mx, my, x, y, 40, 40);
}

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

/*
 Star background rendering:

 BG DRIFTING = Stars move vertically downward
 BG TWINKLING = static stars pulsating
 BG NEBULA = slow moving dust
 BG PARALLAX = multi layer stars with depth

 mode: enum BackgroundMode

 s: struct StarState*
 _ used by BG_DRIFTING and BG_TWINKLING
 _ contains star positions, speed, alpha values and alpha directions

 d: struct DustState*
 _ used by BG_NEBULA
 _ contains drifting dust particle positions and horizontal offsets

 p: struct ParallaxState*
 _ used by BG_PARALLAX
 _ contains layered stars with random speeds, phases and one time intialization

 FUNCTION FLOW =
 1- Clear the window with dark blue space color

 2- depending on mode:
 _ draw stars/dust/parallax points
 - update positions and alpha values of stars
 - wrap elements when they exit screen width and height

 3- apply frame rate pacing to ensure smooth animation independant of machine speed.
 
*/
        
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

int intro_skip_requested() {
    return (MLV_get_keyboard_state(MLV_KEYBOARD_RETURN) == MLV_PRESSED);
}


void draw_intro_skip_hint() {
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


void show_tutorial_screen(void) {

    /* ===== DECLARATIONS ===== */
    const char* tutorial_pages[TUTORIAL_NB_PAGES] = {
        "Bienvenue dans Number Match !\n\n"
        
        "Le but est d'effacer tous les chiffres du plateau.  \n"
        "\nTrouvez des paires de nombres égaux (1 et 1, 7 et 7).  \n"
        "\nOu des paires dont la somme est égale à 10 (6 et 4).  \n"
        "\nSélectionnez les nombres un par un pour les rayer.  \n"
        "\nChaque paire trouvée vous fait gagner des points.  \n"
        "\nEffacez tout le plateau pour remporter la partie !  ",
        
        "S’il ne reste plus de chiffres sur une ligne,  \n"
        "\nelle sera supprimée et le reste fusionné.  \n"
        "\nVérifiez toutes les directions possibles :  \n"
        "\nles paires peuvent être horizontales,  \n"
        "\nverticales ou même diagonales.  \n",
        
        "\nCherchez les chiffres séparés par des cases :  \n"
        "\nles diagonales opposées peuvent former paires.  \n"
        "\nVérifiez ligne par ligne : la fin d’une ligne  \n"
        "\net le début de la suivante peuvent cacher paires.  ",
    
        "Bonne chance et amusez-vous bien!"
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
