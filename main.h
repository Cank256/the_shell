#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "_printf/main.h"

#define BUFFER_SIZE 1024
#define MAX_LINE 1024
#define TOKEN_BUFSIZE 64

typedef struct alias {
    char *name;
    char *value;
    struct alias *next;
} alias_t;

extern char **environ;

ssize_t get_line(char **lineptr, size_t *n, FILE *stream);

char **split_line(char *line);

char **split_commands(char *line, char *delim);

void handle_alias(char **argv, alias_t **aliases);

void free_aliases(alias_t *aliases);

char *_strcat(char* destination, const char* source);

int _strlen(char *str);

int _strcmp(const char *X, const char *Y);

#endif
