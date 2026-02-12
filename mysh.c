/**
 * mysh.c - A simple Unix shell
 * 
 * COP 5614 - Operating Systems
 * Homework 1: Mini-Shell
 * Spring 2026
 * 
 * Author(s): [Your Name(s)]
 * Panther ID(s): [Your Panther ID(s)]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

/* Function prototypes */
void parse_input(char *input, char **args, int *argc);
int is_builtin(char **args);
void execute_builtin(char **args);
void execute_command(char **args);
void execute_redirection(char **args);
void execute_pipe(char *input);
int has_pipe(char *input);
int has_redirection(char **args);

/**
 * Main function - implements the shell's REPL loop
 */
int main(void)
{
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    int argc;

    while (1) {
        /* Step 1: Print prompt */
        printf("mysh> ");
        fflush(stdout);

        /* Step 2: Read input */
        if (fgets(input, sizeof(input), stdin) == NULL) {
            /* EOF (Ctrl+D) - exit gracefully */
            printf("\n");
            break;
        }

        /* Remove trailing newline */
        input[strcspn(input, "\n")] = '\0';

        /* Skip empty input */
        if (strlen(input) == 0) {
            continue;
        }

        /* Step 3: Check for pipe */
        if (has_pipe(input)) {
            execute_pipe(input);
            continue;
        }

        /* Step 4: Parse input into args */
        parse_input(input, args, &argc);

        /* Skip if no command */
        if (argc == 0) {
            continue;
        }

        /* Step 5: Execute command */
        if (is_builtin(args)) {
            execute_builtin(args);
        } else if (has_redirection(args)) {
            execute_redirection(args);
        } else {
            execute_command(args);
        }
    }

    return 0;
}

/**
 * Parse input string into an array of arguments
 * 
 * @param input  The input string (will be modified by strtok)
 * @param args   Array to store argument pointers
 * @param argc   Pointer to store the argument count
 */
void parse_input(char *input, char **args, int *argc)
{
    *argc = 0;
    char *token = strtok(input, " ");
    
    while (token != NULL && *argc < MAX_ARGS - 1) {
        args[*argc] = token;
        (*argc)++;
        token = strtok(NULL, " ");
    }
    
    args[*argc] = NULL;  /* NULL-terminate for execvp */
}

/**
 * Check if the command is a built-in command
 * 
 * @param args  The argument array
 * @return      1 if built-in, 0 otherwise
 */
int is_builtin(char **args)
{
    if (args[0] == NULL) {
        return 0;
    }
    
    /* TODO: Check if args[0] is "cd", "pwd", or "exit" */
    /* Return 1 if it's a built-in, 0 otherwise */
    
    return 0;  /* Replace this */
}

/**
 * Execute a built-in command (cd, pwd, exit)
 * 
 * @param args  The argument array
 */
void execute_builtin(char **args)
{
    /* TODO: Implement cd */
    /* Hint: Use chdir(). If args[1] is NULL, use getenv("HOME") */
    
    /* TODO: Implement pwd */
    /* Hint: Use getcwd() */
    
    /* TODO: Implement exit */
    /* Hint: Use exit(0) */
}

/**
 * Execute an external command (fork + exec + wait)
 * 
 * @param args  The argument array
 */
void execute_command(char **args)
{
    /* TODO: Implement fork + execvp + wait */
    /* 
     * Hint:
     * 1. Fork a child process
     * 2. In child: call execvp(args[0], args)
     * 3. In child: if execvp returns, it failed - print error and exit(1)
     * 4. In parent: wait for child to finish
     */
}

/**
 * Check if the input contains a pipe character
 * 
 * @param input  The raw input string
 * @return       1 if contains pipe, 0 otherwise
 */
int has_pipe(char *input)
{
    /* TODO: Check if input contains '|' */
    return 0;  /* Replace this */
}

/**
 * Check if the args contain redirection operators
 * 
 * @param args  The argument array
 * @return      1 if contains redirection, 0 otherwise
 */
int has_redirection(char **args)
{
    /* TODO: Check if args contains ">", ">>", or "<" */
    return 0;  /* Replace this */
}

/**
 * Execute a command with I/O redirection
 * 
 * @param args  The argument array (contains >, >>, or <)
 */
void execute_redirection(char **args)
{
    /* TODO: Implement I/O redirection */
    /*
     * Hint:
     * 1. Find the redirection operator and filename in args
     * 2. Remove them from args (set to NULL)
     * 3. Fork a child process
     * 4. In child: open the file with appropriate flags
     *    - ">"  : O_WRONLY | O_CREAT | O_TRUNC
     *    - ">>" : O_WRONLY | O_CREAT | O_APPEND
     *    - "<"  : O_RDONLY
     * 5. In child: use dup2() to redirect stdin/stdout
     * 6. In child: close the file descriptor
     * 7. In child: call execvp()
     * 8. In parent: wait for child
     */
}

/**
 * Execute a pipe command (cmd1 | cmd2)
 * 
 * @param input  The raw input string containing the pipe
 */
void execute_pipe(char *input)
{
    /* TODO: Implement pipe */
    /*
     * Hint:
     * 1. Split input into two parts at '|'
     * 2. Parse each part into its own args array
     * 3. Create pipe: int pipefd[2]; pipe(pipefd);
     * 4. Fork first child:
     *    - Close pipefd[0] (read end)
     *    - dup2(pipefd[1], STDOUT_FILENO)
     *    - Close pipefd[1]
     *    - execvp first command
     * 5. Fork second child:
     *    - Close pipefd[1] (write end)
     *    - dup2(pipefd[0], STDIN_FILENO)
     *    - Close pipefd[0]
     *    - execvp second command
     * 6. Parent: close both pipefd[0] and pipefd[1]
     * 7. Parent: wait for both children
     */
}
