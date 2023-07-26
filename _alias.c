#include "main.h"

void handle_alias(char **argv, alias_t **aliases)
{
    int i;
    char *eq_pos, *name, *value;
    alias_t *current;

    if (argv[1] == NULL) {
        // If no arguments were given, print all aliases
        current = *aliases;
        while (current != NULL) {
            printf("%s='%s'\n", current->name, current->value);
            current = current->next;
        }
    } else {
        for (i = 1; argv[i] != NULL; i++) {
            eq_pos = strchr(argv[i], '=');
            if (eq_pos == NULL) {
                // If no equals sign was found, print the value of the alias
                current = *aliases;
                while (current != NULL && strcmp(current->name, argv[i]) != 0) {
                    current = current->next;
                }
                if (current != NULL) {
                    printf("%s='%s'\n", current->name, current->value);
                }
            } else {
                // If an equals sign was found, set the value of the alias
                *eq_pos = '\0';
                name = argv[i];
                value = eq_pos + 1;

                current = *aliases;
                while (current != NULL && strcmp(current->name, name) != 0) {
                    current = current->next;
                }

                if (current != NULL) {
                    // If the alias already exists, update its value
                    free(current->value);
                    current->value = strdup(value);
                } else {
                    // If the alias doesn't exist, create a new one
                    alias_t *new_alias = malloc(sizeof(alias_t));
                    new_alias->name = strdup(name);
                    new_alias->value = strdup(value);
                    new_alias->next = *aliases;
                    *aliases = new_alias;
                }
            }
        }
    }
}

void free_aliases(alias_t *aliases) {
    alias_t *tmp;

    while (aliases) {
        tmp = aliases;
        aliases = aliases->next;
        free(tmp->name);
        free(tmp->value);
        free(tmp);
    }
}
