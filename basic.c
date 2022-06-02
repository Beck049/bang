#include "basic.h"

size_t fgets_n(char *str, int size, FILE *stream) {
    char *ptr = fgets(str, size, stream);
    if(!ptr) return 0;
    char *end = strchr(ptr, '\n');
    if(end) *end = '\0';
    return strlen(ptr);
}

void strncatf(char *dest, char *format, size_t n, ...) {
	size_t used = strlen(dest);
    va_list args;
    va_start(args, n);
	vsnprintf(dest+used, n-min(n, used), format, args);
	va_end(args);
}