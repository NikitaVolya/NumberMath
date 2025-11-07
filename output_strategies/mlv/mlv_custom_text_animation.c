#include"mlv_custom_text_animation.h"


#ifndef CTEXTA_VECTOR
#define CTEXTA_VECTOR

#define VECTOR_TYPE ctext_animation*
#define VECTOR_NAME ctexta_vector
#include"../../game_objects/vector.h"

#endif /* CTEXTA_VECTOR */

static struct ctexta_vector *ctext_animations = NULL;
static int old_time = 0;

ctext_animation* free_ctext_animation(ctext_animation *value) {

    if (value != NULL) {
        free(value->text);
        MLV_free_font(value->font);
        free(value);
    }

    return NULL;
}

float get_animation_step(int max_time, int current_time, CTEXT_ANIMATION_TYPE a_type) {
    float res, x;

    if (max_time != current_time)
        x = (float) (max_time - current_time) / max_time;
    else
        x = 0.01f;

    switch (a_type) {
    case CTEXT_ANIMATION_LINE:
        res = x;
        break;
    case CTEXT_ANIMATION_EASEOUT:
        res = 1.f / (-10.f * x) + 1.1;
        if (res < 0) res = 0;
        break;
    case CTEXT_ANIMATION_EASEIN:
        res = x * x * x;
        break;
    default:
        res = x;
        break;
    }

    return res;
}

void draw_ctext_animation(ctext_animation *value) {
    if (value != NULL) {

        MLV_draw_adapted_text_box_with_font(
            value->pos.x, value->pos.y,
            value->text, value->font, 2,
            MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
            MLV_TEXT_LEFT
        );
    }
}

ctext_animation* update_ctext_animation(ctext_animation *value) {
    int delta_time;
    ctext_animation *res;
    float d_x, d_y, f;

    delta_time = MLV_get_time() - old_time;

    value->currentTime -= delta_time;

    if (value->currentTime <= 0) {
        res = NULL;
        free_ctext_animation(value);
    } else {
        res = value;
    }

    f = get_animation_step(value->time, value->currentTime, value->animation_type);

    d_x = (float) (value->end.x - value->start.x);
    d_y = (float) (value->end.y - value->start.y);

    value->pos.x = value->start.x + d_x * f;
    value->pos.y = value->start.y + d_y * f;

    return res;
}

void MLV_create_ctext_animation(const char *text, int font_size,
                                int time, vector2i start, vector2i end,
                                CTEXT_ANIMATION_TYPE animation_type) {
    size_t text_len;
    ctext_animation *new_animation;

    if (ctext_animations == NULL) {
        fprintf(stderr, "Error create custom text animation : ctext_animation is not started\n");
        exit(EXIT_FAILURE);
    }

    new_animation = (ctext_animation*) malloc(sizeof(ctext_animation));

    text_len = strlen(text);
    new_animation->text = (char*) calloc(text_len + 1, sizeof(char));
    strcpy(new_animation->text, text);

    new_animation->time = time;
    new_animation->currentTime = time;
    
    new_animation->start = start;
    new_animation->end = end;
    new_animation->pos = start;

    new_animation->font = MLV_load_font("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", font_size);
    
    new_animation->animation_type = animation_type;

    ctexta_vector_push(ctext_animations, new_animation);
}

int check_is_not_null(ctext_animation *value) {
    return value != NULL;
}

void MLV_update_ctext_animations() {

    ctexta_vector_map(ctext_animations, &update_ctext_animation);

    ctexta_vector_filter(ctext_animations, &check_is_not_null);

    old_time = MLV_get_time();
}

void MLV_draw_ctext_animations() {

    MLV_update_ctext_animations();

    ctexta_vector_foreach(ctext_animations, &draw_ctext_animation);
}


void MLV_ctext_animations_start() {
    if (ctext_animations != NULL) {
        fprintf(stderr, "Error custom text animation start : ctext_animation already started\n");
        exit(EXIT_FAILURE);
    }

    ctext_animations = ctexta_vector_create();

    /* get global time */
    old_time = MLV_get_time();
}


void MLV_ctext_animations_end() {

    ctexta_vector_map(ctext_animations, &free_ctext_animation);
    ctexta_vector_free(ctext_animations);
}