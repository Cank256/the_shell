#include "main.h"

/**
 * strcat - concatenates two strings
 * @destination: destination string
 * @source: source string
 * Return: concatenated string
 */
char *_strcat(char* destination, const char* source)
{
    char* ptr = destination + _strlen(destination);

    while (*source != '\0') {
        *ptr++ = *source++;
    }

    *ptr = '\0';

    return destination;
}
