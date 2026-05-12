#include <stdlib.h>
#include <stdio.h>

#include "Commands.h"

int shellRunning = 1;

// Takes in the command character array and splits it across space and NULL terminates it
void ParseCommand(const char* command, char **parsedCommand){
    int i=0,j=0,l=0;
    char ch;
    while ((ch=command[i])!='\n'){
        if(ch==' '){
            if(l!=0){
                parsedCommand[j][l]='\0';
                l=0;
                j++;
            }
        }else{
            parsedCommand[j][l]=ch;
            l++;
        }
        i++;
    }
    parsedCommand[j][l]='\0';
    parsedCommand[j+1] = NULL;
}

void RunShell(){
    char command[COMMAND_LENGTH];
    char *parsedCommand[PARSER_NUM_WORDS];

    // Allocate memory in the heap
    for (int i = 0; i<PARSER_NUM_WORDS;i++){
        parsedCommand[i] = (char*)malloc(WORD_LENGTH);
    }

    printf("\nShell>");
    if(fgets(command,sizeof(command),stdin)==NULL){
        printf("[Error] Invalid input.");
        return; // Return an error code
    }
    
    ParseCommand(command,parsedCommand);

    //for(int i=0;parsedCommand[i]!=NULL;i++){
    //    printf("%s ",parsedCommand[i]);
    //}

    _CMD_ERR errorCode = ExecuteCommand(parsedCommand);

    // Free the allocated memory
    for (int i = 0; i<PARSER_NUM_WORDS;i++){
        free(parsedCommand[i]);
    }   
    
}

int main(void){

    while(shellRunning){
        RunShell();
    }   
    printf("Exiting Shell...\n");

    return 0;
}