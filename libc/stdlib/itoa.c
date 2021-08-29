#include <stdlib.h>

#include <stdbool.h>

char* itoa(int value, char* buffer, int base) {
    int i = 0;
    int r = 0;
    int n = value;

    bool negative = n < 0 && base == 10;

    if (base < 2 || base > 36) {
        return buffer;
    }

    n = abs(n);

    while (n) {
        r = n % base;

        if (r >= 10) {
            buffer[i++] = 'A' + (r - 10);
        } else {
            buffer[i++] = '0' + r;
        }

        n = n / base;
    }

    if (i == 0) {
        buffer[i++] = '0';
    }

    if (negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    return reverse(buffer, 0, i - 1);
}