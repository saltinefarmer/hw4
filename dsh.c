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





char** split(char* cmdline, char* delim, int *i){
    char **input_list = (char**)malloc(sizeof(char*)*100); //a random # for now
    char *tokens = (char*) malloc(256);
    tokens = strtok(cmdline, delim);
    *i = 0;
    
    while(tokens != NULL){
        input_list[*i] = (char*) malloc(256);
        strcpy(input_list[*i], tokens);
        tokens = strtok(NULL, delim);
        i[0]++;
    }
    free(tokens);
    return input_list;
}

void free_2D_array(char** input, int len){
    for(int i = 0; i < len; i++){
        free(input[i]);
    }
}
