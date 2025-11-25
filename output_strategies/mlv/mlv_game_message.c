#include"mlv_game_message.h"


MLV_Button MLV_create_close_button() {
    return MLV_create_button("X",
                                  create_vector2i(
                                      GAME_MESSAGE_CLOSE_BTN_POS_X,
                                      GAME_MESSAGE_CLOSE_BTN_POS_Y
                                      ),
                                  create_vector2i(
                                      GAME_MESSAGE_CLOSE_BTN_SIZE,
                                      GAME_MESSAGE_CLOSE_BTN_SIZE
                                      ),
                                  MLV_COLOR_RED,
                                  MLV_COLOR_WHITE,
                                  MLV_COLOR_GRAY);
}

void MLV_show_ok_game_message(const char *text) {
    GAME_MESSAGE_RESULT res;
    MLV_Button ok_btn, close_btn;
    vector2i mouse_p;
    MLV_Button_state mbutton_state;

    ok_btn = MLV_create_base_button("Ok",
                                    create_vector2i(GAME_MESSAGE_POS_X + GAME_MESSAGE_WIDTH / 4,
                                                    GAME_MESSAGE_POS_Y + GAME_MESSAGE_HEIGHT / 2),
                                    create_vector2i(GAME_MESSAGE_WIDTH / 2, GAME_MESSAGE_HEIGHT / 4));
    close_btn = MLV_create_close_button();
        

    res = GAME_MESSAGE_RESULT_NULL;
    
    while (!res) {

        MLV_get_mouse_position(&mouse_p.x, &mouse_p.y);

        MLV_draw_filled_rectangle(GAME_MESSAGE_POS_X, GAME_MESSAGE_POS_Y,
                                  GAME_MESSAGE_WIDTH, GAME_MESSAGE_HEIGHT,
                                  MLV_COLOR_WHITE);

        MLV_draw_text_box(GAME_MESSAGE_POS_X, GAME_MESSAGE_POS_Y,
                          GAME_MESSAGE_WIDTH, GAME_MESSAGE_HEIGHT / 2,
                          text, 1,
                          MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                          MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);

        MLV_draw_rectangle(GAME_MESSAGE_POS_X, GAME_MESSAGE_POS_Y,
                           GAME_MESSAGE_WIDTH, GAME_MESSAGE_HEIGHT,
                           MLV_COLOR_BLACK);

        MLV_draw_button(&ok_btn, &mouse_p);
        MLV_draw_button(&close_btn, &mouse_p);
        
        mbutton_state = MLV_get_mouse_button_state(MLV_BUTTON_LEFT);

        if (mbutton_state == MLV_PRESSED) {
            if (MLV_mouse_is_on_button(&ok_btn, &mouse_p))
                res = GAME_MESSAGE_RESULT_OK;
            if (MLV_mouse_is_on_button(&close_btn, &mouse_p))
                res = GAME_MESSAGE_RESULT_NONE;
        }

        MLV_delay_according_to_frame_rate();
        MLV_actualise_window();
    }
}

GAME_MESSAGE_RESULT MLV_show_yesno_game_message(const char *text) {
    GAME_MESSAGE_RESULT res;
    MLV_Button yes_btn, no_btn, close_btn;
    vector2i mouse_p;
    MLV_Button_state mbutton_state;

    yes_btn = MLV_create_base_button("Yes",
                                    create_vector2i(GAME_MESSAGE_POS_X + GAME_MESSAGE_WIDTH / 4 - GAME_MESSAGE_WIDTH / 3 - 20,
                                                    GAME_MESSAGE_POS_Y + GAME_MESSAGE_HEIGHT / 2),
                                    create_vector2i(GAME_MESSAGE_WIDTH / 3, GAME_MESSAGE_HEIGHT / 4));
    yes_btn = MLV_create_base_button("No",
                                     create_vector2i(GAME_MESSAGE_POS_X + GAME_MESSAGE_WIDTH / 4 + GAME_MESSAGE_WIDTH / 3 + 20,
                                     GAME_MESSAGE_POS_Y + GAME_MESSAGE_HEIGHT / 2),
                                     create_vector2i(GAME_MESSAGE_WIDTH / 3, GAME_MESSAGE_HEIGHT / 4));
    close_btn = MLV_create_close_button();
        

    res = GAME_MESSAGE_RESULT_NULL;
    
    while (!res) {

        MLV_get_mouse_position(&mouse_p.x, &mouse_p.y);

        MLV_draw_filled_rectangle(GAME_MESSAGE_POS_X, GAME_MESSAGE_POS_Y,
                                  GAME_MESSAGE_WIDTH, GAME_MESSAGE_HEIGHT,
                                  MLV_COLOR_WHITE);

        MLV_draw_text_box(GAME_MESSAGE_POS_X, GAME_MESSAGE_POS_Y,
                          GAME_MESSAGE_WIDTH, GAME_MESSAGE_HEIGHT / 2,
                          text, 1,
                          MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                          MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);

        MLV_draw_rectangle(GAME_MESSAGE_POS_X, GAME_MESSAGE_POS_Y,
                           GAME_MESSAGE_WIDTH, GAME_MESSAGE_HEIGHT,
                           MLV_COLOR_BLACK);

        MLV_draw_button(&yes_btn, &mouse_p);
        MLV_draw_button(&no_btn, &mouse_p);
        MLV_draw_button(&close_btn, &mouse_p);
        
        mbutton_state = MLV_get_mouse_button_state(MLV_BUTTON_LEFT);

        if (mbutton_state == MLV_PRESSED) {
            if (MLV_mouse_is_on_button(&yes_btn, &mouse_p))
                res = GAME_MESSAGE_RESULT_OK;
            if (MLV_mouse_is_on_button(&yes_btn, &mouse_p))
                res = GAME_MESSAGE_RESULT_NO;
            if (MLV_mouse_is_on_button(&close_btn, &mouse_p))
                res = GAME_MESSAGE_RESULT_NONE;
        }

        MLV_delay_according_to_frame_rate();
        MLV_actualise_window();
    }

    return res;
}

GAME_MESSAGE_RESULT MLV_show_game_message(const char *text, GAME_MESSAGE_TYPE type) {
    GAME_MESSAGE_RESULT res;
    
    switch (type) {
    case GAME_MESSAGE_TYPE_OK:
        MLV_show_ok_game_message(text);
        res = GAME_MESSAGE_RESULT_OK;
        break;
    case GAME_MESSAGE_TYPE_YESNO:
        res = MLV_show_yesno_game_message(text);
        break;
    default:
        res = GAME_MESSAGE_RESULT_NULL;
    }

    return res;
}