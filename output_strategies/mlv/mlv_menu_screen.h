#ifndef MLV_MENU_SCREEN_H
#define MLV_MENU_SCREEN_H

/************************************************************
 *                     MENU SCREEN MODULE
 ************************************************************/

#include <MLV/MLV_all.h>
#include "../../game_objects/vector2i.h"
#include "../../game.h"
#include "mlv_game_setup.h"
#include "../../game_config.h"

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
 *                                  BACKGROUND ENUM
 * ==========================================================================*/

enum BackgroundMode {
    BG_DRIFTING = 1,
    BG_TWINKLING,
    BG_NEBULA,
    BG_PARALLAX
};
/************************************************************
 *               PUBLIC ENUM: MENU RESULT
 ************************************************************/

typedef enum {
    MENU_PLAY       = 1,
    MENU_SCOREBOARD = 2,
    MENU_TUTORIAL   = 3,
    MENU_QUIT       = 4
} MENU_RESULT;

/************************************************************
 *                 PUBLIC FUNCTION: MENU
 ************************************************************/

/**
 * @brief Show the main menu window and handle user interactions.
 *
 * - Plays intro animation once
 * - Allows switching background modes via "BG" button
 * - Buttons: New game, Load game, Tutorial, Quit
 *
 * @param config Game configuration pointer (passed to start/load)
 */
void mlv_show_menu(struct game_config *config);

/* ============================================================================
 *                              STATE STRUCTURES
 * ==========================================================================*/

struct StarsState {
    int x[STAR_COUNT];
    int y[STAR_COUNT];
    int speed[STAR_COUNT];
    int alpha[STAR_COUNT];
    int alpha_dir[STAR_COUNT];
};

struct DustState {
    int x[STAR_COUNT];
    int y[STAR_COUNT];
    int dx[STAR_COUNT];
};

struct ParallaxState {
    int init;
    int px[60];
    int py[60];
    int layer[60];
    int phase[60];
};

/************************************************************
 *         FUNCTIONS USED ACROSS THIS MODULE
 *         (DECLARED HERE, DEFINED IN .C)
 ************************************************************/

/**
 * @brief Initialize background states (stars, dust, parallax).
 * @param s Stars state (output)
 * @param d Dust state (output)
 * @param p Parallax state (output)
 */
void init_background(struct StarsState* s, struct DustState* d, struct ParallaxState* p);
    
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
    );
    
/* --- UI hit tests --- */
int hit_button(int mx, int my, int x, int y);
int hit_small(int mx, int my, int x, int y, int w, int h);

/**
 * @brief Hit-test for background mode toggle button ("BG") at top-right.
 * @param mx Mouse X
 * @param my Mouse Y
 * @return 1 if hit, 0 otherwise
 */
int hit_bg_button(int mx, int my);

/* --- UI drawing --- */
void draw_round_button(int x, int y, int w, int h, int r, MLV_Color c);

/**
 * @brief Draw a centered menu button (fixed size BTN_W x BTN_H) with label.
 * @param label Text to draw
 * @param y     Button Y (top-left)
 * @param hover Non-zero if hovered
 */
void draw_button(const char *label, int y, int hover);

/**
 * @brief Draw background toggle button ("BG") at top-right.
 * @param mx Mouse X
 * @param my Mouse Y
 */
void draw_bg_button(int mx, int my);

/**
 * @brief Draw a left arrow triangle used for tutorial navigation.
 * @param x     Arrow top-left X
 * @param y     Arrow top-left Y
 * @param hover Non-zero if hovered
 */
void draw_left_arrow(int x, int y, int hover);

/**
 * @brief Draw a right arrow triangle used for tutorial navigation.
 * @param x     Arrow top-left X
 * @param y     Arrow top-left Y
 * @param hover Non-zero if hovered
 */
void draw_right_arrow(int x, int y, int hover);

/* --- Intro & title --- */

/**
 * @brief Check if ENTER is pressed to skip intro.
 * @return 1 if skip requested, 0 otherwise
 */
int  intro_skip_requested();

/**
 * @brief Draw skip hint at bottom center.
 */
void draw_intro_skip_hint();

/**
 * @brief Draw intro nebula-like background (animated using time parameter).
 * @param t Time parameter / phase
 */
void draw_intro_background(double t);

/**
 * @brief Play the full intro animation (skippable with ENTER).
 */
void play_intro_animation();


/**
 * @brief Draw animated title at a default Y position.
 * @param text  Title string
 * @param phase Animation phase (used to shift colors)
 */
void draw_animated_title(const char* text, float phase);

/**
 * @brief Draw animated title at a specific Y position.
 * @param text  Title string
 * @param phase Animation phase (used to shift colors)
 * @param y     Y position for text
 */
void draw_animated_title_at_y(const char* text, float phase, int y);

/**
 * @brief Draw a small number spiral (used in intro collapse).
 * @param angle  Current angle
 * @param radius Current radius
 */
void draw_number_spiral(double angle, double radius);

/* --- Tutorial --- */
/**
 * @brief Animate a sliding transition between tutorial pages.
 * @param old_page  Text of current page
 * @param new_page  Text of next page
 * @param direction -1: slide to previous, +1: slide to next
 */
void animate_slide(const char* old_page, const char* new_page, int direction);

/**
 * @brief Show the tutorial screen with multiple pages and slide transitions.
 *
 * Navigation:
 * - Left/Right arrows or LEFT/RIGHT keys
 * - ENTER or ESC exits tutorial
 */
void show_tutorial_screen();

/* --- Game flow --- */
/**
 * @brief Confirmation popup before starting a new game.
 *
 * Displays a modal overlay with Yes/No buttons.
 *
 * @return 1 if user clicked YES, 0 if user clicked NO
 */
int confirm_new_game();

#endif /* MLV_MENU_SCREEN_H */
