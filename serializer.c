#include"serializer.h"


int serialize_field_cell(field_cell *cell, FILE* file) {
    int res;
    short tmp;

    if (cell != NULL && file != NULL) {

        tmp = cell->value +
            cell->is_available * 128 +
            cell->is_highlited * 64 +
            cell->is_selected * 32 +
            cell->is_cursor * 16;
    
        if(fwrite(&tmp, sizeof(short) / 2, 1, file)) {
            res = 1;
        } else {
            res = 0;
        }
    } else {
        res = 0;
    }

    return res;
}

field_cell deserialize_field_cell(FILE* file) {
    field_cell res;
    short tmp;

    res.is_selected = 0;

    if (!fread(&tmp, sizeof(short) / 2, 1, file)) {
        res.value = 0;
        res.is_available = 0;
        res.is_highlited = 0;
    } else {
        res.value = tmp & 15;
        res.is_available = tmp / 128;
        res.is_highlited = tmp / 64 & 1;
        res.is_selected = tmp / 32 & 1;
        res.is_cursor = tmp / 16 & 1;
    }
    
    return res;
}

int serialize_game_field(game_field* field, const char* file_name) {
    FILE* file;
    int res, i;
    vector2i pos;
    unsigned short tmp;

    if ((file = fopen(file_name, "w")) == NULL) {
        printf("Error while serializing game file\nCant write in file: %s\n", file_name);
        res = 0;
    } else {
        res = 1;
      
        fwrite(&field->width, sizeof(unsigned short), 1, file);
        fwrite(&field->stage, sizeof(unsigned short), 1, file);
        fwrite(&field->score, sizeof(int) / 2, 1, file);
        fwrite(&field->count, sizeof(int) / 2, 1, file);

        tmp = field->additions_max * 16 + field->additions_available;
        fwrite(&tmp, sizeof(unsigned short) / 2, 1, file);
        
        tmp = field->hints_max * 16 + field->hints_available;
        fwrite(&tmp, sizeof(unsigned short) / 2, 1, file);

        for (i = 0; i < field->count; i++) {
            pos.y = i / field->width;
            pos.x = i % field->width;
            
            serialize_field_cell(get_game_field_cell(field, pos), file);
        }


        fclose(file);
    }

    return res;
}

game_field* deserialize_game_field(const char* file_name) {
    FILE* file;
    game_field *res;
    field_cell *cell;
    size_t file_size;
    int i;
    unsigned short additions_data, hints_data;

    res = NULL;
    if ((file = fopen(file_name, "r")) == NULL) {
        printf("Error while deserializing game file\nCant read from file: %s\n", file_name);
    } else {

        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        if (file_size >= 8) {

            res = (game_field*) malloc(sizeof(game_field));

            if (!fread(&res->width, sizeof(unsigned short), 1, file) ||
                !fread(&res->stage, sizeof(unsigned short), 1, file) ||
                !fread(&res->score, sizeof(int) / 2, 1, file) ||
                !fread(&res->count, sizeof(int) / 2, 1, file) ||
                !fread(&additions_data, sizeof(unsigned short) / 2, 1, file) ||
                !fread(&hints_data, sizeof(unsigned short) / 2, 1, file) ||
                (size_t)res->count + 10 != file_size) {
                free(res);
                res = NULL;
            } else {
                init_game_field_table(res);

                res->additions_max = additions_data / 16 & 15;
                res->additions_available = additions_data & 15;

                res->hints_max = hints_data / 16 & 15;
                res->hints_available = hints_data & 15;
                
                for (i = 0; i < res->count; i++) {

                    cell = (field_cell*) malloc(sizeof(field_cell));
                    *cell = deserialize_field_cell(file);

                    add_cell_game_field(res, cell);
                }
                
            }
        }

        fclose(file);
    }

    return res;
}

int serialize_game_score(const char* file_name, int value) {
    FILE *file;
    int res;
    
    file = fopen(file_name, "w");

    if (file == NULL) {
        res = 0;
    } else {
        res = 1;

        fwrite(&value, sizeof(int), 1, file);
    }

    return res;
}

int deserialize_game_score(const char* file_name) {
    FILE *file;
    int res;

    file = fopen(file_name, "r");
    if (file == NULL || (fread(&res, sizeof(int), 1, file) == 0)) {
        res = 0;
    }

    return res;
}