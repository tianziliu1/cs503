#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"
#include <errno.h>

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */

int last_return_code = 0;

int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->_cmd_buffer = (char *)malloc(SH_CMD_MAX);
    if (cmd_buff->_cmd_buffer == NULL) {
        return ERR_MEMORY;
    }
    clear_cmd_buff(cmd_buff);
    return OK;
}

int free_cmd_buff(cmd_buff_t *cmd_buff) {
    if (cmd_buff->_cmd_buffer != NULL) {
        free(cmd_buff->_cmd_buffer);
        cmd_buff->_cmd_buffer = NULL;
    }
    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->argc = 0;
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv));
    if (cmd_buff->_cmd_buffer != NULL) {
        memset(cmd_buff->_cmd_buffer, 0, SH_CMD_MAX);
    }
    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    clear_cmd_buff(cmd_buff);

    bool in_quotes = false;
    char *arg_start = NULL;

    for (char *ptr = cmd_line; *ptr != '\0'; ptr++) {
        if (*ptr == '"') {
            in_quotes = !in_quotes;
            if (in_quotes) {
                arg_start = ptr + 1;
            } else {
                *ptr = '\0';
                cmd_buff->argv[cmd_buff->argc++] = arg_start;
            }
        } else if (*ptr == ' ' && !in_quotes) {
            *ptr = '\0';
            if (arg_start != NULL) {
                cmd_buff->argv[cmd_buff->argc++] = arg_start;
                arg_start = NULL;
            }
        } else {
            if (arg_start == NULL) {
                arg_start = ptr;
            }
        }
    }

    if (arg_start != NULL) {
        cmd_buff->argv[cmd_buff->argc++] = arg_start;
    }

    cmd_buff->argv[cmd_buff->argc] = NULL;

    return OK;
}

Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, "exit") == 0) {
        return BI_CMD_EXIT;
    } else if (strcmp(input, "cd") == 0) {
        return BI_CMD_CD;
    } else if (strcmp(input, "rc") == 0) {
        return BI_RC;
    } else if (strcmp(input, DRAGON_CMD) == 0) {
        return BI_CMD_DRAGON;
    }
    return BI_NOT_BI;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    Built_In_Cmds cmd_type = match_command(cmd->argv[0]);
    if (cmd_type == BI_CMD_EXIT) {
        exit(OK_EXIT);
    } else if (cmd_type == BI_CMD_CD) {
        if (cmd->argc < 2) {
            // No arguments provided
            // Optional: Do nothing or change to home directory (as an enhancement)
        } else {
            if (chdir(cmd->argv[1]) != 0) {
                perror("cd failed");
            }
        }
        return BI_EXECUTED;
    } else if (cmd_type == BI_RC) {
        printf("Last return code: %d\n", last_return_code);
        return BI_EXECUTED;
    } else if (cmd_type == BI_CMD_DRAGON) {
        print_dragon();
        return BI_CMD_DRAGON;
    }    
    return BI_NOT_BI;
}

int exec_cmd(cmd_buff_t *cmd) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(cmd->argv[0], cmd->argv);
        perror("execvp failed");
        exit(ERR_EXEC_CMD);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            last_return_code = WEXITSTATUS(status);
            return last_return_code;
        }
    } else {
        perror("fork failed");
        return ERR_EXEC_CMD;
    }

    return OK;
}

int exec_local_cmd_loop() {
    char cmd_buff[SH_CMD_MAX];
    int rc = 0;
    cmd_buff_t cmd;

    if (alloc_cmd_buff(&cmd) != OK) {
        perror("Failed to allocate command buffer");
        return ERR_MEMORY;
    }

    // TODO IMPLEMENT MAIN LOOP
    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        if (strlen(cmd_buff) == 0) {
            printf(CMD_WARN_NO_CMD);
            continue;
        }

    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff
        rc = build_cmd_buff(cmd_buff, &cmd);
        if (rc != OK) {
            if (rc == ERR_TOO_MANY_COMMANDS) {
                printf("%s", CMD_ERR_PIPE_LIMIT);
            } else {
                printf("Command parsing error\n");
            }
            continue;
        }

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing
        if (exec_built_in_cmd(&cmd) == BI_EXECUTED) {
            continue;
        }

        // Execute external command
        if ((rc = exec_cmd(&cmd)) != OK) {
            if (rc == ERR_EXEC_CMD) {
                printf("%s", CMD_ERR_EXECUTE);
            }
        }
    }

    clear_cmd_buff(&cmd);

    free_cmd_buff(&cmd);

    return OK;
}

