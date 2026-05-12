#ifndef COMMANDS_H
#define COMMANDS_H

typedef __int8_t _CMD_ERR;

_CMD_ERR ExecuteCommand(char** ccommand);


#define COMMAND_LENGTH 64
#define PARSER_NUM_WORDS 32
#define WORD_LENGTH 64

extern int shellRunning;

#endif