#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Commands.h"

_CMD_ERR RunPOSIXCommand(char **command){
    pid_t childProcessId = fork();
    if(childProcessId <0 ){
        fprintf(stderr,"Error creating process.\n");
        return 1;
    }
    if (childProcessId == 0){
        if(execvp(command[0],command)<0){
            fprintf(stderr,"Error running command '%s'.\n",command[0]);
            exit(1);
        }
        else{
            exit(0);
        }
    }
    else{
        int exitCode;
        wait(&exitCode);
        return exitCode;
    }
}

_CMD_ERR Run(char **command){
    if(strcmp(command[0],"exit")==0){
        if(command[1]==NULL){
            shellRunning = 0;
            return 0;
        }
        else{
        fprintf(stderr,"'exit' takes no arguments but were given.\n");
        return 1;
        }  
    }
    else if(strcmp(command[0],"cd")==0){
        if(command[1]==NULL){
            fprintf(stderr,"'cd' takes an argument but no arguments were given.\n");
            return 1;
        }
        else if (command[2]!=NULL){
            fprintf(stderr,"'cd' takes an argument but more arguments were given.\n");
            return 1;
        }
        else{
                if(chdir(command[1]) != 0){ 
                    fprintf(stderr,"Could not change directory to %s.\n",command[1]);
                    return 1;
                }
                else{
                    return 0;
                }
        } 
    }
    else{
        return RunPOSIXCommand(command);
    }
}


_CMD_ERR ExecuteCommand(char** command){
    int commandExecuted = 0;
    char *currentCommand[PARSER_NUM_WORDS];

    int j=0;
    for(int i = 0;command[i]!=NULL;i++){
        if (strcmp(command[i],"&&")==0){
            currentCommand[j] = NULL;
            commandExecuted=1;
            if(Run(currentCommand)==1){
                break;
            }
            j=-1;
        }else{
            commandExecuted=0;
            currentCommand[j] = command[i];
        }
        j++;
    }
    if(!commandExecuted){
        currentCommand[j]=NULL;
        Run(currentCommand);
    }

}