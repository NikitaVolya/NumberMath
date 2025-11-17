
#ifndef _OUTILS_H
#define _OUTILS_H


/*  
    Converts an integer value into a null-terminated string.

    input:
        dest  - pointer to a character buffer where the result will be written  
        value - integer number to convert  
        count - maximum number of characters allowed in 'dest', includin '\0'

    output:
        'dest' will contain the string representation of 'value', safely null-terminated  
*/
void itos(char *dest, int value, int count);


#endif /* _OUTILS_H */