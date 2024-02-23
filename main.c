/*
 * main.c
 *
 *  Created on: Mar 17 2017
 *      Author: david
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "dsh.h"

#define MAX_PROC 250

int main(int argc, char *argv[]) {
	int len = 0; // num of paths to check
	
	char *cmdline = (char*) malloc(MAXBUF); // stores user input from commmand line

	// DO NOT REMOVE THE BLOCK BELOW (FORK BOMB PREVENTION) //
	struct rlimit limit;
	limit.rlim_cur = MAX_PROC;
	limit.rlim_max = MAX_PROC;
	setrlimit(RLIMIT_NPROC, &limit);
	// DO NOT REMOVE THE BLOCK ABOVE THIS LINE //


	//char *tokens = (char*) malloc(256);
	while (1){
		char** path_locs = (char**)malloc(sizeof(char*)*100);
		char* path = (char*) malloc(4096);
		char* checks = (char*) malloc(256);
		char* directory = (char*)malloc(4096);
		char* tokens = (char*) malloc(50);
		char** input_list = (char**)malloc(sizeof(char*)*100); //a random # for now
		char* dir = (char*) malloc(4096);
		printf("dsh> ");
		//scanf("%s", cmdline);
		if(fgets(cmdline, 256, stdin) == NULL){
			exit(0);
		}
		
		// get first command
		
		strcpy(tokens, cmdline);
		
		tokens = strtok(tokens, " \n\t");

		char cmd[50];
		if(tokens == NULL){ 
			continue;
		}
		strcpy(cmd, tokens);
		

		int i;
		i = 0;	
	
		input_list = split(cmdline, " \n\t", &i);

		// mode one; full path is given
		
		if(cmd[0] =='/'){
			// then its a full path
			
			if(access(cmd, F_OK | X_OK) == 0){

				// file exists

				__pid_t p = fork();

				if(p == -1){
					if(errno == 11){
						printf("Failed to fork: Too many processes: Resource temporarily unavailable");
					}
				}
				if(p > 0){
					if(i > 0 && strcmp(input_list[i-1], "&")!= 0){ //the final is &
						wait(NULL); 
					} 
				} else if (p == 0){
					
					execv(cmd, input_list);
				}
			} else{
				printf("That command cant be executed. Please try again.\n");
			}

		} 
		else if(strcmp(cmd, "cd") == 0){
			// execute cd
			if(input_list[1] == NULL){
				strcpy(dir, getenv("HOME"));
			}else{
				getcwd(dir, 4096);
				strcat(dir, "/");
				strcat(dir, input_list[1]);
			}
			chdir(dir);

		}else if(strcmp(cmd, "pwd") == 0){
			// execute pwd
			getcwd(directory, 4096);
			printf("%s\n", directory);
		}else if(strcmp(cmd, "exit") == 0){

			free(checks);
			free(path_locs);
			free_2D_array(input_list, i);
			free(input_list);
			free(tokens);
			free(cmdline);
			free(path);
			free(directory);
			free(dir);
			exit(0);
		}
		else{
			// its something else
			
			getcwd(directory, 4096);
			strcat(directory, "/");
			strcat(directory, cmd);
			if(access(directory, F_OK | X_OK) == 0){ // try to access file
				// if success, do something here
				__pid_t p = fork();

				if(p == -1){
					if(errno == 11){
						printf("Failed to fork: Too many processes: Resource temporarily unavailable");
					}
				}
				else if(p > 0){
			
					if(i > 0 && strcmp(input_list[i-1], "&")!= 0){ 
						//extra commands, and the final is &
						wait(NULL);
					}
				} else if (p == 0){
					execv(directory, input_list);
				}
			} else{ // not success :(
				
				strcpy(path, getenv("PATH"));
				path_locs = split(path, ":", &len);


				for (int j = 0; j < len; j++){
					strcpy(checks, path_locs[j]);
					strcat(checks, "/");
					strcat(checks, cmd);
					if(access(checks, F_OK | X_OK) == 0){ // file exists

						__pid_t p = fork();

						if(p == -1){
							if(errno == 11){
								printf("Failed to fork: Too many processes: Resource temporarily unavailable");
							}
						}
						else if(p > 0){
							if(i > 0 && strcmp(input_list[i-1], "&")!= 0){ //extra commands, and the final is &
								wait(NULL);
							}
						
						} else if (p == 0){
							execv(checks, input_list);
						}
						break;
					
					}
					else if(j == len-1){
						printf("Command not found.\n");
						
					}
					
				}
				free_2D_array(path_locs, len);
				
				
			}
		}
			free(checks);
			free(path_locs);
			free_2D_array(input_list, i);
			free(input_list);
			free(tokens);
			free(path);
			free(directory);
			free(dir);


	}
	return 0;
}
