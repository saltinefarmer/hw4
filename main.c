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

	// DO NOT REMOVE THE BLOCK BELOW (FORK BOMB PREVENTION) //
	struct rlimit limit;
	limit.rlim_cur = MAX_PROC;
	limit.rlim_max = MAX_PROC;
	setrlimit(RLIMIT_NPROC, &limit);
	// DO NOT REMOVE THE BLOCK ABOVE THIS LINE //

	char *cmdline = (char*) malloc(MAXBUF); // stores user input from commmand line

	//char *tokens = (char*) malloc(256);
	while (1){
		printf("dsh> ");
		//scanf("%s", cmdline);
		if(fgets(cmdline, 256, stdin) == NULL){
			exit(0);
		}
		
		// get first command
		char* tokens = (char*) malloc(50);
		
		strcpy(tokens, cmdline);
		
		tokens = strtok(tokens, " \n\t");

		char cmd[50];
		if(tokens == NULL){
			continue;
		}
		strcpy(cmd, tokens);
		

		int i[1];
		i[0] = 0;		
		int and[1];
		and[0] = 0;

		char **input_list = (char**)malloc(sizeof(char*)*100); //arb # for now
			
		input_list = split_cmd(cmdline, " \n\t", i, and);

		// mode one; full path is given
		
		if(cmd[0] =='/'){
			// then its a full path
			
			if(access(cmd, F_OK | X_OK) == 0){

				// file exists

				__pid_t p = fork();

				if(p == -1){
					if(errno == 11){
						printf("Failed to fork: Too many processes: Resource temporarily unavailable");
						continue;
					}
				}
				if(p > 0){
					printf("made it\n");

					if(and[0] != 0){ //the final is &
						printf("not waiting...\n");
					} else{ // no extra commands or extras w/ no &
						printf("waiting...\n");
						wait();
					}
					continue;
				} else if (p == 0){
					printf("Doing!\n");
					
					execv(cmd, input_list);
				}
			} else{
				printf("That command cant be executed. Please try again.\n");
			}

		} 
		else if(strcmp(cmd, "cd") == 0){
			// execute cd
			char* dir = (char*) malloc(4096);
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
			char* directory = (char*)malloc(4096);
			getcwd(directory, 4096);
			printf("%s\n", directory);
			free(directory);
		}else if(strcmp(cmd, "exit") == 0){
			exit(0);
		}
		else{
			// its something else
			char* directory = (char*)malloc(4096);
			getcwd(directory, 4096);
			strcat(directory, "/");
			strcat(directory, cmd);
			if(access(directory, F_OK | X_OK) == 0){ // try to access file
				// if success, do something here
				printf("in current directory :)\n"); // execute cmd
			} else{ // not success :(
				char* path = (char*) malloc(4096);
				strcpy(path, getenv("PATH"));
				int len[1]; // num of paths to check
				len[0] = 0;
				char** path_locs = split(path, ":", len);
				char* checks = (char*) malloc(256);


				for (int j = 0; j < len[0]; j++){
					strcpy(checks, path_locs[j]);
					strcat(checks, "/");
					strcat(checks, cmd);
					if(access(checks, F_OK | X_OK) == 0){ // file exists

						printf("Command found\n");


						__pid_t p = fork();

						if(p == -1){
							if(errno == 11){
								printf("Failed to fork: Too many processes: Resource temporarily unavailable");
								continue;
							}
						}
						if(p > 0){
					
							if(i[0] > 0 && strcmp(input_list[i[0]-1], "&")== 0){ //extra commands, and the final is &
								printf("not waiting...\n");
							} else{ // no extra commands or extras w/ no &
								printf("waiting...\n");
								wait();
						}
						
						} else if (p == 0){
							printf("Doing!\n");
							execv(checks, input_list);
						}
						break;
					
					}
					if(j == len[0]-1){
						printf("Command not found.\n");
						
					}
				}

				
			}
		}
	
		
		
		
	}
	return 0;
}
