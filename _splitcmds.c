#include "main.h"

char **split_commands(char *line, char *delim) {
    int bufsize = TOKEN_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    token = strtok(line, delim);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += TOKEN_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
        }

        token = strtok(NULL, delim);
    }
    tokens[position] = NULL;
    return tokens;
}
