#include"mlv_end_game_message.h"



void mlv_end_game_message(struct game_config *config) {
    MLV_Button continue_btn;
    vector2i mouse_p;
    MLV_Button_state mbutton_state;
    int exit;
    char message[31];

    strcpy(message, "GAME OVER!!!\nnew score: ");
    itos(message + 24, config->field->score, 6);

    exit = 0;

    continue_btn = MLV_create_base_button("Continue",
                                    create_vector2i(END_GAME_MESSAGE_POS_X + END_GAME_MESSAGE_WIDTH / 4,
                                                    END_GAME_MESSAGE_POS_Y + END_GAME_MESSAGE_HEIGHT / 2),
                                    create_vector2i(END_GAME_MESSAGE_WIDTH / 2, END_GAME_MESSAGE_HEIGHT / 4));

    while (!exit) {
                                    
        MLV_draw_filled_rectangle(END_GAME_MESSAGE_POS_X, END_GAME_MESSAGE_POS_Y,
                                  END_GAME_MESSAGE_WIDTH, END_GAME_MESSAGE_HEIGHT,
                                  MLV_COLOR_WHITE);

        MLV_draw_text_box(END_GAME_MESSAGE_POS_X, END_GAME_MESSAGE_POS_Y,
                          END_GAME_MESSAGE_WIDTH, END_GAME_MESSAGE_HEIGHT / 2,
                          message, 1,
                          MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                          MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);

        MLV_draw_rectangle(END_GAME_MESSAGE_POS_X, END_GAME_MESSAGE_POS_Y,
                           END_GAME_MESSAGE_WIDTH, END_GAME_MESSAGE_HEIGHT,
                           MLV_COLOR_BLACK);

        MLV_draw_button(&continue_btn, &mouse_p);

        MLV_get_mouse_position(&mouse_p.x, &mouse_p.y);
        mbutton_state = MLV_get_mouse_button_state(MLV_BUTTON_LEFT);

        if (mbutton_state == MLV_PRESSED) {
            if (MLV_mouse_is_on_button(&continue_btn, &mouse_p))
                exit = 1;
        }

        MLV_delay_according_to_frame_rate();
        MLV_actualise_window();
    }
}