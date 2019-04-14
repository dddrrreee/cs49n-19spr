#include "rpi.h"

int putk(const char *p) {
    for(; *p; p++)
        rpi_putchar(*p);
    rpi_putchar('\n');
    return 1;
}
