#include "main.h"

ssize_t get_line(char **lineptr, size_t *n, FILE *stream) {
    char *buffer;
    size_t buf_len;
    char *pos;

    buffer = malloc(sizeof(char) * MAX_LINE);
    if(buffer == NULL)
        return -1;

    if(fgets(buffer, MAX_LINE, stream) != NULL) {
        pos = strchr(buffer, '#'); // find the first occurrence of #
        if (pos != NULL) *pos = '\0'; // if # is found, replace it with '\0' to end the string

        buf_len = strlen(buffer);

        // If the string is empty, return 0
        if (buf_len == 0) {
            free(buffer);
            return 0;
        }

        *lineptr = buffer;
        *n = buf_len;

        return buf_len;
    }

    free(buffer);
    return -1;
}
