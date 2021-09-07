#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include <assert.h>
#include <ctype.h>

/* allocates memory */
void *emalloc(size_t s){
    void *result = malloc(s);
    if(NULL == result){
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/* reallocates memory when we need more size */
void *erealloc(void *p, size_t s){
    void *result = realloc(p, s);
    if(NULL == result){
        fprintf(stderr, "Memory reallocation failed\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

int getword(char *s, int limit, FILE *stream){
    int c;
    char *w = s;
    assert(limit > 0 && s !=NULL && stream != NULL);

    /*skip to start of word*/
    while(!isalnum(c = getc(stream)) && EOF != c);

    if (EOF == c) {
        return EOF;
    } else if (--limit > 0) {
        *w++ = tolower(c);
    }

    while (--limit > 0) {
        if (isalnum(c = getc(stream))) {
            *w++ = tolower(c);
        } else if ('\'' == c) {
            limit++;
        } else {
            break;
        }
    }
    *w = '\0';
    return w -s;
}
