#include "main.h"

/**
 * main - Entry point
 * Return: Always 0 (Success)
 */
int main(int argc, char **argv) {
    char *line = NULL;
    size_t len = 0;
    char **commands;
    char *path = "/bin/";
    pid_t pid;
    int status;
    char prev_dir[MAX_LINE] = "";
    char curr_dir[MAX_LINE];
    int i;
    alias_t *aliases = NULL;
    int last_exit_status;
    FILE *input_stream = stdin;

    if (argc > 1) {
        input_stream = fopen(argv[1], "r");
        if (!input_stream) {
            perror("Error opening file");
            return EXIT_FAILURE;
        }
    }

    while(1) {
        _printf("#shell$ ");
        if(get_line(&line, &len, stdin) <= 0) break;

        // Split the line into separate commands
        commands = split_commands(line, ";");

        last_exit_status = 0;

        for (i = 0; commands[i] != NULL; i++) {
            // Tokenize the input
            argv = split_line(commands[i]);

            // Check if the command is "exit"
            if (argv[0] && _strcmp(argv[0], "exit") == 0) {
                int exit_status = 0;
                // Check if exit status argument is provided
                if (argv[1]) {
                    exit_status = atoi(argv[1]);
                }
                free(argv);
                free(commands);
                free(line);
                exit(exit_status);
            }

            // Check if the command is "env"
            if (argv[0] && _strcmp(argv[0], "env") == 0) {
                char **env = environ;
                while (*env) _printf("%s\n", *env++);
                continue;
            }

            // If the command is "setenv"
            if (argv[0] && _strcmp(argv[0], "setenv") == 0) {
                if (argv[1] && argv[2]) {
                    if (setenv(argv[1], argv[2], 1) != 0) {
                        perror("setenv error");
                    }
                } else {
                    fprintf(stderr, "setenv: missing arguments\n");
                }
                continue;
            }

            // If the command is "unsetenv"
            if (argv[0] && _strcmp(argv[0], "unsetenv") == 0) {
                if (argv[1]) {
                    if (unsetenv(argv[1]) != 0) {
                        perror("unsetenv error");
                    }
                } else {
                    fprintf(stderr, "unsetenv: missing arguments\n");
                }
                continue;
            }

            // If the command is "cd"
            if (argv[0] && _strcmp(argv[0], "cd") == 0) {
                getcwd(curr_dir, sizeof(curr_dir));
                
                if (!argv[1] || _strcmp(argv[1], "~") == 0) { // change to home directory
                    if(chdir(getenv("HOME")) != 0) {
                        perror("chdir error");
                    }
                } else if (_strcmp(argv[1], "-") == 0) { // change to previous directory
                    if(strlen(prev_dir) == 0) {
                        fprintf(stderr, "cd: OLDPWD not set\n");
                    } else {
                        if(chdir(prev_dir) != 0) {
                            perror("chdir error");
                        }
                    }
                } else { // change to specified directory
                    if(chdir(argv[1]) != 0) {
                        perror("chdir error");
                    }
                }

                strcpy(prev_dir, curr_dir);
                getcwd(curr_dir, sizeof(curr_dir));
                setenv("PWD", curr_dir, 1);
                continue;
            }

            // Handle the && and || operators
            if (commands[i+1] != NULL) {
                if (_strcmp(commands[i+1], "&&") == 0) {
                    if (WEXITSTATUS(status) != 0) break; // If the command failed, skip the next command
                    i++;
                } else if (_strcmp(commands[i+1], "||") == 0) {
                    if (WEXITSTATUS(status) == 0) break; // If the command succeeded, skip the next command
                    i++;
                }
            }

            //Handle alias
            if (_strcmp(argv[0], "alias") == 0) {
                handle_alias(argv, &aliases);
                continue;
            }

            //Handle $? and $$
            if (_strcmp(argv[i], "$?") == 0) {
                free(argv[i]);
                asprintf(&argv[i], "%d", last_exit_status);
            } else if (_strcmp(argv[i], "$$") == 0) {
                free(argv[i]);
                asprintf(&argv[i], "%d", getpid());
            }

            // Fork a child to execute the command
            pid = fork();

            if (pid < 0)
            {
                perror("fork error");
                exit(EXIT_FAILURE);
            }
            else if(pid == 0) {
                char exec_path[MAX_LINE];
                strcpy(exec_path, path);
                _strcat(exec_path, argv[0]);

                if(execv(exec_path, argv) < 0) {
                    perror("execv error");
                    exit(1);
                }
            } else {
                waitpid(pid, &status, 0);
                if (WIFEXITED(status)) {
                    last_exit_status = WEXITSTATUS(status);
                }
            }

            free(argv);
        }

        free(commands);
    }

    // Close file if we opened one
    if (input_stream != stdin) {
        fclose(input_stream);
    }

    free(line);
    free_aliases(aliases);
    return 0;
}
