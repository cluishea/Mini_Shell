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

    char *outputFilePath;
    int fileSet = 0;
    int std_out = 1;
    _CMD_ERR errorCode = 0;
    for(int i = 0;command[i]!=NULL;i++){
        if (strcmp(command[i],">>")==0){
            if (command[i+1]==NULL){
                fprintf(stderr,"Output redirection >> takes a file as argument but none were given.\n");
                return 1;
            }
            if (command[i+2]!=NULL){
                fprintf(stderr,"Output redirection >> takes a file as argument but more were given.\n");
                return 1;
            }
            else{
                outputFilePath = command[i+1];
                command[i] = NULL;
                fileSet = 1;
            }
            break;
        }
    }
    if (fileSet){
        int outputFile = open(outputFilePath,O_WRONLY|O_APPEND|O_CREAT,0644);
        // Duplicating stdout
        std_out = dup(1);
        // Setting stdout to file
        dup2(outputFile,1);
    }
    if(strcmp(command[0],"exit")==0){
        if(command[1]==NULL){
            shellRunning = 0;
            errorCode = 0;
        }
        else{
        fprintf(stderr,"'exit' takes no arguments but were given.\n");
        errorCode=1;
        }  
    }
    else if(strcmp(command[0],"cd")==0){
        if(command[1]==NULL){
            fprintf(stderr,"'cd' takes an argument but no arguments were given.\n");
            errorCode = 1;
        }
        else if (command[2]!=NULL){
            fprintf(stderr,"'cd' takes an argument but more arguments were given.\n");
            errorCode = 1;
        }
        else{
                if(chdir(command[1]) != 0){ 
                    fprintf(stderr,"Could not change directory to %s.\n",command[1]);
                    errorCode = 1;
                }
                else{
                    errorCode = 0;
                }
        } 
    }
    else{
        errorCode = RunPOSIXCommand(command);
    }

    // Setting stdout to 1
    if(fileSet){
        dup2(std_out,1);
    }

    return errorCode;

}


_CMD_ERR ExecuteCommand(char** command){
    int commandChained = 0;
    char *currentCommand[PARSER_NUM_WORDS];
    int j=0;
    for(int i = 0;command[i]!=NULL;i++){
        if (strcmp(command[i],"&&")==0){
            currentCommand[j] = NULL;
            commandChained=1;
            if(Run(currentCommand)==1){
                return 1;
            }
            j=0;
        }
        else{
            commandChained=0;
            currentCommand[j] = command[i];
            j++;
        }
        
    }
    if(!commandChained){
        currentCommand[j]=NULL;
        return Run(currentCommand);
    }

}