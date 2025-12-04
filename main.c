#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<unistd.h>

#include"game_config.h"


int select_output_function(struct game_config *config, const char *name) {
    int res;

    res = 0;
    printf("%s\n", name);
    if (strcmp("mlv", name) == 0) {
        set_mlv_output(config);
    } else if (strcmp("console", name) == 0) {
        set_console_output(config);
    } else {
        res = 1;
    }

    return res;
}

int main(int argc, char **argv) {
    const char *optstring = "ho:";
    int val;
    struct game_config *config;
    
    srand(time(NULL));

    config = create_game_config();
    set_mlv_output(config);

    val = getopt(argc, argv, optstring);

    while(val!=EOF){

        switch(val){
        case 'h':
            printf("numbermatch -o [console | mlv] \"to select output mode\"\n"); 
            exit(EXIT_SUCCESS);
            break;
        case 'o': 
            if (select_output_function(config, optarg)) {
                fprintf(stderr, "Unknown output mode %s\n", optarg); 
                exit(EXIT_FAILURE);
            }
            break;
        case ':': 
            fprintf(stderr, "Argument missing for option %c\n", optopt);
            exit(EXIT_FAILURE);
            break;
        case '?': 
            fprintf(stderr, "type %s -h to see all options\n", argv[0]); 
            exit(EXIT_FAILURE);
            break;
        }

        val=getopt(argc, argv, optstring);
    }

    execute_game(config);

    free_game_config(config);
    exit(EXIT_SUCCESS);
}