
#ifndef _MLV_CUSTOM_TEXT_ANIMATION_H
#define _MLV_CUSTOM_TEXT_ANIMATION_H

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<MLV/MLV_all.h>
#include"../../game_objects/vector2i.h"

typedef enum {
    CTEXT_ANIMATION_LINE,
    CTEXT_ANIMATION_EASEIN,
    CTEXT_ANIMATION_EASEOUT
} CTEXT_ANIMATION_TYPE;

typedef struct {
    char *text;
    int time, currentTime;
    vector2i start, end, pos;
    MLV_Font *font;
    CTEXT_ANIMATION_TYPE animation_type;
} ctext_animation;


ctext_animation* free_ctext_animation(ctext_animation *value);

float get_animation_step(int max_time, int current_time, CTEXT_ANIMATION_TYPE a_type);

void display_ctext_animation(ctext_animation *value);

void MLV_create_ctext_animation(const char *text, int font_size,
                                int time, vector2i start, vector2i end,
                                CTEXT_ANIMATION_TYPE animation_type);

void MLV_update_ctext_animations();

void MLV_draw_ctext_animations();

void MLV_ctext_animations_start();

void MLV_ctext_animations_end();

void MLV_ctext_animations_set_action(void (*func)(ctext_animation *value));

void MLV_ctext_animations_clear_action();

#endif /* _MLV_CUSTOM_TEXT_ANIMATION_H */