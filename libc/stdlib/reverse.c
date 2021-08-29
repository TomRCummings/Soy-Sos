#include <stdlib.h>

char* reverse(char* buffer, int start, int end) {
    char* head = buffer;
    char* tail = buffer + end;
    for ( ; head < tail; ++head, --tail) {
        char h = *head;
        char t = *tail;
        *head = t;
        *tail = h;
    }
    return buffer;
}