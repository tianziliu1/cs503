#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

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

// Allocate memory for the command buffer
int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->_cmd_buffer = (char *)malloc(SH_CMD_MAX);
    if (cmd_buff->_cmd_buffer == NULL) {
        return ERR_MEMORY;
    }
    clear_cmd_buff(cmd_buff);
    return OK;
}

// Free the command buffer memory
int free_cmd_buff(cmd_buff_t *cmd_buff) {
    if (cmd_buff->_cmd_buffer != NULL) {
        free(cmd_buff->_cmd_buffer);
        cmd_buff->_cmd_buffer = NULL;
    }
    return OK;
}

// Clear the contents of the command buffer
int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->argc = 0;
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv));
    if (cmd_buff->_cmd_buffer != NULL) {
        memset(cmd_buff->_cmd_buffer, 0, SH_CMD_MAX);
    }
    return OK;
}

// Parse user input into commands and arguments
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

/*
int close_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff){

}
*/

int build_cmd_list(char *cmd_line, command_list_t *clist){
    clist->num = 0;
    char *cmd_start = cmd_line;
    char *pipe_ptr;

    while((pipe_ptr = strchr(cmd_start, PIPE_CHAR)) != NULL){
        *pipe_ptr = '\0';
        build_cmd_buff(cmd_start, &clist->commands[clist->num++]);
        cmd_start = pipe_ptr + 1;
    }

    if(*cmd_start != '\0'){
        build_cmd_buff(cmd_start, &clist->commands[clist->num++]);
    }

    if(clist->num > CMD_MAX){
        return ERR_TOO_MANY_COMMANDS;
    }
    return OK;
}
/*
int free_cmd_list(command_list_t *cmd_lst){

}
*/

// Execute a built-in command (e.g., cd, exit)
Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    Built_In_Cmds cmd_type = match_command(cmd->argv[0]);
    if (cmd_type == BI_CMD_EXIT) {
        exit(OK_EXIT);
    } else if (cmd_type == BI_CMD_CD) {
        if (cmd->argc < 2) {
            const char *home_dir = getenv("HOME");
            if (home_dir == NULL || chdir(home_dir) != 0) {
                perror("cd failed");
            }
            else{
                printf("%s\n", home_dir);
            }
        } else {
            if (chdir(cmd->argv[1]) != 0) {
                perror("cd failed");
            }
            else{
                char cwd[1024];
                if(getcwd(cwd, sizeof(cwd)) != NULL){
                    printf("%s\n", cwd);
                }
            }
        }
        return BI_EXECUTED;
    } else if (cmd_type == BI_CMD_RC) {
        printf("Last return code: %d\n", last_return_code);
        return BI_EXECUTED;
    } else if (cmd_type == BI_CMD_DRAGON) {
        print_dragon();
        return BI_EXECUTED;
    }
    return BI_NOT_BI;
}

// Execute an external command
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

// Main command loop
int exec_local_cmd_loop() {
    char cmd_buff[SH_CMD_MAX];
    int rc = 0;
    //cmd_buff_t cmd;
    command_list_t clist;

    /*if (alloc_cmd_buff(&cmd) != OK) {
        perror("Failed to allocate command buffer");
        return ERR_MEMORY;
    } */

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

        rc = build_cmd_list(cmd_buff, &clist);
        if (rc == ERR_TOO_MANY_COMMANDS) {
            printf("%s", CMD_ERR_PIPE_LIMIT);
            continue;
        }

        if (clist.num == 1) {
            if (exec_built_in_cmd(&clist.commands[0]) == BI_EXECUTED) {
                continue;
            }

            rc = exec_cmd(&clist.commands[0]);
            if (rc != OK) {
                printf("%s", CMD_ERR_EXECUTE);
            }
        } else {
            rc = execute_pipeline(&clist);
            if (rc != OK) {
                printf("%s", CMD_ERR_EXECUTE);
            }
        }
    }
    //clear_cmd_buff(&clist);
    free_cmd_list(&clist);

    return OK;
}


int execute_pipeline(command_list_t *clist) {
    int pipe_fds[(CMD_MAX - 1)] [2];  // Array of pipes
    pid_t pids[CMD_MAX];                 // Array to store process IDs
    int status;

    // Create all necessary pipes
    for (int i = 0; i < clist->num - 1; i++) {
        if (pipe(pipe_fds[i]) == -1){
            perror("pipe failed");
            return ERR_MEMORY;
        }
    }

    // Create processes for each command
    for (int i = 0; i < clist->num; i++) {
        pids[i] = fork();
        if (pids[i] == -1){
            perror("fork failed");
            return ERR_EXEC_CMD;
        }

        if (pids[i] == 0) {  // Child process
            // Set up input pipe for all except first process
            if (i > 0) {
                dup2(pipe_fds[(i - 1)] [0], STDIN_FILENO);
            }
            // Set up output pipe for all except lat process
            if (i < clist->num - 1) {
                dup2(pipe_fds[i][1], STDOUT_FILENO);
            }

            // Close all pipe ends in child
            for (int j = 0; j < (clist->num - 1); j++) {
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
            }

            // Execute the command
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp failed");
            exit(ERR_EXEC_CMD);
        }
    }

    // Parent process; close all pipe ends
    for (int i = 0; i < (clist->num - 1); i++) {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < clist->num; i++) {
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status)) {
            last_return_code = WEXITSTATUS(status);
        }
    }

    return OK;
}    
  