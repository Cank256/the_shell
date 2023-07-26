#include "main.h"

char **split_line(char *line) {
    int buffer_size = MAX_LINE;
    int position = 0;
    char **tokens = malloc(buffer_size * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = line;
    while (token != NULL && *token) {
        while (*token == ' ' || *token == '\t' || *token == '\n') {
            *token = '\0';
            token++;
        }

        if (*token != '\0') {
            tokens[position] = token;
            position++;

            if (position >= buffer_size) {
                buffer_size += MAX_LINE;
                tokens = realloc(tokens, buffer_size * sizeof(char*));

                if (!tokens) {
                    fprintf(stderr, "Allocation error\n");
                    exit(EXIT_FAILURE);
                }
            }

            /* Skip token */
            while (*token != '\0' && *token != ' ' && *token != '\t' && *token != '\n') {
                token++;
            }
        }
    }

    tokens[position] = NULL;
    return tokens;
}
