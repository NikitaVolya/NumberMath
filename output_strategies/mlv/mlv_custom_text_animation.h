/**
 * @file mlv_custom_text_animation.h
 * @brief Implements custom animated text handling using MLV.
 *
 * This module provides functions to create, update, draw, and manage
 * animated text objects. Animations can have different easing types
 * and move text from a start to an end position over a specified duration.
 *
 * It uses an internal dynamic vector of @ref ctext_animation structures
 * to manage active animations globally.
 *
 * @note All text animations are rendered with the font
 *       "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf" by default.
 */

#ifndef _MLV_CUSTOM_TEXT_ANIMATION_H
#define _MLV_CUSTOM_TEXT_ANIMATION_H

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<MLV/MLV_all.h>
#include"../../game_objects/vector2i.h"

/**
 * @enum CTEXT_ANIMATION_TYPE
 * @brief Defines types of text animations.
 *
 * @var CTEXT_ANIMATION_TYPE::CTEXT_ANIMATION_LINE
 * Linear animation (constant speed).
 *
 * @var CTEXT_ANIMATION_TYPE::CTEXT_ANIMATION_EASEOUT
 * Ease-out animation (slows down at the end).
 *
 * @var CTEXT_ANIMATION_TYPE::CTEXT_ANIMATION_EASEIN
 * Ease-in animation (speeds up at the start).
 */ 
typedef enum {
    CTEXT_ANIMATION_LINE,
    CTEXT_ANIMATION_EASEIN,
    CTEXT_ANIMATION_EASEOUT
} CTEXT_ANIMATION_TYPE;

/**
 * @struct ctext_animation
 * @brief Represents a single animated text object.
 *
 * @var ctext_animation::text
 * Pointer to the text string to display.
 *
 * @var ctext_animation::font
 * Font used to render the text.
 *
 * @var ctext_animation::time
 * Total animation duration in milliseconds.
 *
 * @var ctext_animation::currentTime
 * Remaining animation time in milliseconds.
 *
 * @var ctext_animation::start
 * Start position of the animation (vector2i).
 *
 * @var ctext_animation::end
 * End position of the animation (vector2i).
 *
 * @var ctext_animation::pos
 * Current position of the text (vector2i), updated every frame.
 *
 * @var ctext_animation::animation_type
 * Type of animation applied (linear, ease-in, ease-out).
 */
typedef struct {
    char *text;
    int time, currentTime;
    vector2i start, end, pos;
    MLV_Font *font;
    CTEXT_ANIMATION_TYPE animation_type;
} ctext_animation;

/**
 * @brief Frees memory associated with a single text animation.
 *
 * @param value Pointer to the animation to free.
 * @return NULL Always returns NULL to facilitate vector filtering.
 */
ctext_animation* free_ctext_animation(ctext_animation *value);

/**
 * @brief Computes the progress of an animation step.
 *
 * @param max_time Total duration of the animation.
 * @param current_time Remaining time of the animation.
 * @param a_type Animation type (linear, ease-in, ease-out).
 * @return float Progress ratio between 0 and 1.
 */
float get_animation_step(int max_time, int current_time, CTEXT_ANIMATION_TYPE a_type);

/**
 * @brief Draws a text animation at its current position.
 *
 * @param value Pointer to the animation to draw.
 */
void display_ctext_animation(ctext_animation *value);

/**
 * @brief Creates and starts a new text animation.
 *
 * @param text Text to animate.
 * @param font_size Font size for the text.
 * @param time Animation duration in milliseconds.
 * @param start Starting position (vector2i).
 * @param end Ending position (vector2i).
 * @param animation_type Type of animation.
 */
void MLV_create_ctext_animation(const char *text, int font_size,
                                int time, vector2i start, vector2i end,
                                CTEXT_ANIMATION_TYPE animation_type);

 /**
 * @brief Updates all active text animations globally.
 */
void MLV_update_ctext_animations();

/**
 * @brief Draws a text animation at its current position.
 *
 * @param value Pointer to the animation to draw.
 */
void MLV_draw_ctext_animations();

/**
 * @brief Initializes the global text animation system.
 */
void MLV_ctext_animations_start();

/**
 * @brief Frees all active animations and stops the system.
 */
void MLV_ctext_animations_end();

#endif /* _MLV_CUSTOM_TEXT_ANIMATION_H */