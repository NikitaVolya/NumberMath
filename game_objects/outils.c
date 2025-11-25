#include"outils.h"


void itos(char *dest, int value, int count) {
    int i, j, tmp;
    
    if (count <= 0) {
        
    } else if (count == 1) {
        dest[0] = '\0';
    } else if (value == 0) {
        dest[0] = '0';
        dest[1] = '\0';
    } else {
        for (i = 0; i < count - 1 && value > 0; i++) {
            dest[i] = '0' + value % 10;
            value /= 10;
        }
        dest[i] = '\0';
        
        j = 0;
        i -= 1;
        while (j < i) {
            tmp = dest[i];
            dest[i] = dest[j];
            dest[j] = tmp;

            i--;
            j++;
        }
    }
}