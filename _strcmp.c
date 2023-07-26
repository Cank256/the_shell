#include "main.h"

/**
 * _strcmp - compares two strings
 * @X: pointer to string
 * @Y: pointer to string
 * Return: ASCII difference (integer)
 */
int _strcmp(const char *X, const char *Y)
{
    while (*X)
    {
        if (*X != *Y) {
            break;
        }

    	X++;
        Y++;
    }

    return *(const unsigned char*)X - *(const unsigned char*)Y;
}
