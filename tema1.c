#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

typedef struct Instruction{
	char *cmd;
	char *arg;
} Instruction;

Dir* root(char *name){
	Dir *dir = (Dir*)malloc(sizeof(Dir));
	dir->head_children_dirs = NULL;
	dir->head_children_files = NULL;
	dir->name = (char*)malloc(sizeof(name));
	strcpy(dir->name, name);
	dir->next = NULL;
	dir->parent = NULL;
	return dir;
}

void touch (Dir* parent, char* name) {
	File *file = (File*)malloc(sizeof(File));
	file->name = (char*)malloc(sizeof(name));
	strcpy(file->name, name);
	file->next = NULL;
	file->parent = parent;

	// update current directory's file list
	File *tmp = parent->head_children_files;
	if(tmp == NULL){
		parent->head_children_files = file;
	}
	else{
		while(tmp->next){
			tmp = tmp->next;
		}
		tmp->next = file;
	}
}

void mkdir (Dir* parent, char* name) {
	Dir *dir = (Dir*)malloc(sizeof(Dir));
	dir->head_children_dirs = NULL;
	dir->head_children_files = NULL;
	dir->name = (char*)malloc(sizeof(name));
	strcpy(dir->name, name);
	dir->next = NULL;
	dir->parent = parent;

	// update current directory's directory list
	Dir *tmp = parent->head_children_dirs;
	if(tmp == NULL){
		parent->head_children_dirs = dir;
	}
	else{
		while(tmp->next){
			tmp = tmp->next;
		}
		tmp->next = dir;
	}
}

void ls (Dir* parent) {
	Dir* tmp1 = parent->head_children_dirs;
	while(tmp1){
		printf("%s\n", tmp1->name);
		tmp1 = tmp1->next;
	}
	File* tmp2 = parent->head_children_files;
	while(tmp2){
		printf("%s\n", tmp2->name);
		tmp2 = tmp2->next;
	}
}

void rm (Dir* parent, char* name) {}

void rmdir (Dir* parent, char* name) {}

void cd(Dir** target, char *name) {}

char *pwd (Dir* target) {}

void stop (Dir* target) {}

void tree (Dir* target, int level) {}

void mv(Dir* parent, char *oldname, char *newname) {}

Instruction* parser(char *str){
	Instruction *instruction = (Instruction*)malloc(sizeof(Instruction));
	instruction->cmd = (char*)malloc(MAX_INPUT_LINE_SIZE);
	instruction->arg = (char*)malloc(MAX_INPUT_LINE_SIZE);

	// status 0 for reading cmd; status 1 for reading arg
	int status = 0, k = 0;

	for(int i = 0; i < strlen(str) - 1; i++){
		if(str[i] == ' '){
			instruction->cmd[i] = '\0';
			instruction->cmd = (char*)realloc(instruction->cmd, strlen(instruction->cmd) + 1);

			// start reading arg
			status = 1;
			i++;
		}
		if(status == 0){
			instruction->cmd[i] = str[i];
		}
		else {
			instruction->arg[k++] = str[i];
		}
	}
	instruction->arg[k] = '\0';
	instruction->arg = (char*)realloc(instruction->arg, strlen(instruction->arg) + 1);
	return instruction;
}

void freeSystem(Dir *root){

}

int main (int argc, char *argv[]) {
	FILE *f = fopen(argv[1], "r");
	char *str = (char*)malloc(MAX_INPUT_LINE_SIZE);
	Dir *ROOT = root("ROOT");
	Dir *currentDir = ROOT;

	do
	{
		// reads instructions one-by-one
		fgets(str, MAX_INPUT_LINE_SIZE, f);
		//printf("%s", str);

		// parsing the input file
		Instruction *instruction = parser(str);
		// printf("Command: \"%s\", argument: \"%s\"\n", instruction->cmd, instruction->arg);

		// apply instructions
		if(strcmp(instruction->cmd, "stop") == 0){
			free(instruction);
			break;
		}
		else if(strcmp(instruction->cmd, "touch") == 0){
			// TODO: File exists!
			touch(currentDir, instruction->arg);
		}
		else if(strcmp(instruction->cmd, "mkdir") == 0){
			mkdir(currentDir, instruction->arg);
		}
		else if(strcmp(instruction->cmd, "ls") == 0){
			ls(currentDir);
		}
		else if(strcmp(instruction->cmd, "rm") == 0){

		}
		else if(strcmp(instruction->cmd, "rmdir") == 0){

		}
		else if(strcmp(instruction->cmd, "cd") == 0){

		}
		else if(strcmp(instruction->cmd, "pwd") == 0){

		}
		else if(strcmp(instruction->cmd, "tree") == 0){

		}
		else if(strcmp(instruction->cmd, "mv") == 0){

		}

		// apply function
		

		free(instruction);
		
	} while (1);

	free(str);
	freeSystem(ROOT);
	fclose(f);

	return 0;
}
