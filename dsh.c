/*
 * dsh.c
 *
 *  Created on: Aug 2, 2013
 *      Author: chiu
 */
#include "dsh.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <errno.h>
#include <err.h>
#include <sys/stat.h>
#include <string.h>


// TODO: Your function definitions below (declarations in dsh.h)

/**
 * This is just an example. Delete this before 
 * submission.
 */
void example(int* x) {
    //*x = thisIsGlobal;
    x = 0;
} 

char** split_cmd(char *cmdline, char *delim, int *i, int *and){
    char **input_list = (char**)malloc(sizeof(char*)*100); //arb # for now
    char *tokens = (char*) malloc(256);
    tokens = strtok(cmdline, delim);
    i[0] = 0;
    and[0] = 0;

    //tokens = strtok(NULL, delim);

    while(tokens != NULL){
        if(strcmp(tokens, "&")==0){
            and[0] = 1;
            break;
        }
        
        input_list[i[0]] = (char*) malloc(256);
        strcpy(input_list[i[0]], tokens);

        tokens = strtok(NULL, delim);
        i[0]++;
    }
    //free(tokens);
    return input_list;
}

char** split(char* cmdline, char* delim, int* i){
    char **input_list = (char**)malloc(sizeof(char*)*100); //arb # for now
    char *tokens = (char*) malloc(256);
    tokens = strtok(cmdline, delim);
    i[0] = 0;
    
    while(tokens != NULL){
        input_list[i[0]] = (char*) malloc(256);
        strcpy(input_list[i[0]], tokens);
        tokens = strtok(NULL, delim);
        i[0]++;
    }
    free(tokens);
    return input_list;
}
