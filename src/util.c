#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int memswap(void *A, void *B, int len)
{
    char *tmp = malloc(len);
    if (tmp == NULL) {
        return -1;
    }
    memcpy(tmp, A, len);
    memcpy(A, B, len);
    memcpy(B, tmp, len);
    free(tmp);
}

int Equal(void *value1, void *value2, char type)
{
    switch(type) {
        case 'c': return !strcmp(value1, value2);
        case 'i': return (*(int *) value1 == *(int *) value2);
        case 'f': return (*(float *) value1 == *(float *) value2);
    }
}

int Greater(void *value1, void *value2, char type)
{
    switch(type) {
        case 'c': return (strcmp(value1, value2) > 0);
        case 'i': return (*(int *) value1 > *(int *) value2);
        case 'f': return (*(float *) value1 > *(float *) value2);
    }
}

int GrtOrEq(void *value1, void *value2, char type)
{
    switch(type) {
        case 'c': return (strcmp(value1, value2) >= 0);
        case 'i': return (*(int *) value1 >= *(int *) value2);
        case 'f': return (*(float *) value1 >= *(float *) value2);
    }
}


int LesOrEq(void *value1, void *value2, char type)
{
    switch(type) {
        case 'c': return (strcmp(value1, value2) <= 0);
        case 'i': return (*(int *) value1 <= *(int *) value2);
        case 'f': return (*(float *) value1 <= *(float *) value2);
    }
}

int Less(void *value1, void *value2, char type)
{
    switch(type) {
        case 'c': return (strcmp(value1, value2) < 0);
        case 'i': return (*(int *) value1 < *(int *) value2);
        case 'f': return (*(float *) value1 < *(float *) value2);
    }
}
