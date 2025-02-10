#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */


 static char *trim_spaces(char *str) {
    char *end;

    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';

    return str;
}

static int parse_command(char *cmd_str, command_t *cmd) {
    char *arg_token;
    int arg_length = 0;

    arg_token = strtok(cmd_str, " ");
    while (arg_token != NULL) {
        if (arg_length == 0) {
            if (strlen(arg_token) >= EXE_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            strcpy(cmd->exe, arg_token);
        } else {
            if (strlen(cmd->args) + strlen(arg_token) + 1 >= ARG_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            if (arg_length > 1) {
                strcat(cmd->args, " ");
            }
            strcat(cmd->args, arg_token);
        }
        arg_length++;
        arg_token = strtok(NULL, " ");
    }

    return OK;
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    char *pipe_pos;
    char *start = cmd_line;
    int cmd_count = 0;

    memset(clist, 0, sizeof(command_list_t));

    if (cmd_line == NULL || strlen(cmd_line) == 0) {
        return WARN_NO_CMDS;
    }

    if (cmd_line[0] == PIPE_CHAR || cmd_line[strlen(cmd_line) - 1] == PIPE_CHAR) {
        return WARN_NO_CMDS;
    }

    while ((pipe_pos = strchr(start, PIPE_CHAR)) != NULL) {
        *pipe_pos = '\0';

        if (cmd_count > CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        start = trim_spaces(start);
        int parse_result = parse_command(start, &clist->commands[cmd_count]);
        if (parse_result != OK) {
            return parse_result;
        }

        cmd_count++;
        start = pipe_pos + 1;
    }

    start = trim_spaces(start);
    int parse_result = parse_command(start, &clist->commands[cmd_count]);
    if (parse_result != OK) {
        return parse_result;
    }

    clist->num = ++cmd_count;

    return OK;
}