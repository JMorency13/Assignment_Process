# Mini-Shell Implementation in C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINE 1024  // Maximum length of input command
#define MAX_ARGS 100   // Maximum number of arguments

void execute_command(char **args);
void handle_built_in_commands(char **args);

int main() {
    char *args[MAX_ARGS];
    char input[MAX_LINE];
    while (1) {
        printf("mini-shell> ");
        fgets(input, MAX_LINE, stdin);
        input[strcspn(input, "\n")] = 0;  // Remove newline character

        if (strcmp(input, "exit") == 0) {
            break;
        }

        // Tokenize the input string
        char *token = strtok(input, " ");
        int i = 0;
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // Null-terminate the arguments

        handle_built_in_commands(args);
        execute_command(args);
    }
    return 0;
}

void execute_command(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {  // Child process
        if (execvp(args[0], args) == -1) {
            perror("mini-shell: command not found");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("mini-shell: fork failed");
    } else {  // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

void handle_built_in_commands(char **args) {
    // Handle I/O redirection
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0) {
            args[i] = NULL; // Null-terminate before redirection
            int fd = open(args[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd < 0) {
                perror("mini-shell: open failed");
                return;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            break;
        }
        else if (strcmp(args[i], "<") == 0) {
            args[i] = NULL;
            int fd = open(args[i + 1], O_RDONLY);
            if (fd < 0) {
                perror("mini-shell: open failed");
                return;
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            break;
        }
    }
    // Handle pipes
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            args[i] = NULL; // Null-terminate first command
            int pipe_fd[2];
            pipe(pipe_fd);
            if (fork() == 0) {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[0]);
                execute_command(args);
                exit(0);
            }
            if (fork() == 0) {
                dup2(pipe_fd[0], STDIN_FILENO);
                close(pipe_fd[1]);
                execute_command(args + i + 1);
                exit(0);
            }
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            wait(NULL);
            wait(NULL);
            return;
        }
    }
}