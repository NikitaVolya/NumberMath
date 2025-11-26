
#include"custom_output.h"


void repeat_print(const char* text, int n) {
    int i;

    for (i = 0; i < n; i++)
        printf("%s", text);
}

void set_input_mode(struct termios *old_tio) {
    struct termios new_tio;

    tcgetattr(STDIN_FILENO, old_tio);
    new_tio = *old_tio;
    new_tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

void get_cursor_position(int *row, int *col) {
    struct termios old_tio;
    set_input_mode(&old_tio);

    printf("\033[6n");
    fflush(stdout);

    if (scanf("\033[%d;%dR", row, col) != 2) {
        *row = *col = -1;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}

void print_over(const char *text, vector2i pos) {
    int i, j, max_line_len, tmp;
    vector2i start_p;

    get_cursor_position(&start_p.y, &start_p.x);

    max_line_len = 0;
    i = 0;

    /* get max size of line in text */
    while (text[i] != '\0') {
        tmp = 0;
        while(text[i] != '\n' && text[i] != '\0') {
            i++;
            tmp++;
        }
        if (tmp > max_line_len)
            max_line_len = tmp;
        if (text[i] == '\n')
            i++;
    }

    gotopos(pos);
    pos.y++;
    for(i = 0; i < max_line_len + BORDER_SIZE * 2; i++)
        printf(BORDER_SYMBL);


    for (i=0; i < BORDER_SIZE; i++) {
        gotopos(pos);
        pos.y++;
        for(j = 0; j < max_line_len + BORDER_SIZE * 2; j++)
            printf(" ");
    }


    i = 0;
    while (text[i] != '\0') {

        gotopos(pos);
        pos.y++;

        for(j = 0; j < BORDER_SIZE - 1; j++)
            printf(" ");
       
        printf(" ");
        j = 0;
        while (j < max_line_len && text[i] != '\n' && text[i] != '\0') {
            printf("%c", text[i]);
            i++;
            j++;
        }

        for(;j < max_line_len + BORDER_SIZE; j++)
            printf(" ");

        if (text[i] == '\n')
            i++;
    }

    for (i = 0; i < BORDER_SIZE; i++) {
        gotopos(pos);
        pos.y++;
        for(j = 0; j < max_line_len + BORDER_SIZE * 2; j++)
            printf(" ");
    }

    gotopos(pos);
    pos.y++;
    for(i = 0; i < max_line_len + BORDER_SIZE * 2; i++)
        printf(BORDER_SYMBL);

    gotopos(start_p);
}