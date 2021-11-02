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
	char *arg1;
	char *arg2;
} Instruction;

void freeSystem(Dir *target){
	// go thru its children directories
	
	Dir *tmp = target->head_children_dirs;
	Dir *delD;
	while(tmp){
		freeSystem(tmp);
		delD = tmp;
		tmp = tmp->next;
		free(delD->name);
		free(delD);
	}
	
	// free files files
	File *tmp1 = target->head_children_files;
	File *del;
	while(tmp1){
		del = tmp1;
		tmp1 = tmp1->next;
		free(del->name);
		free(del);
	}
}

void tabPrinter(int n){
	for(int i = 0; i < n; i++){
		printf("    ");
	}
}

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
	// check if file already exists
	File *tmp = parent->head_children_files;
	while(tmp){
		if(strcmp(tmp->name, name) ==  0){
			printf("File already exists\n");
			return;
		}
		tmp = tmp->next;
	}
	Dir *tmp1 = parent->head_children_dirs;
	while(tmp1){
		if(strcmp(tmp1->name, name) ==  0){
			printf("File already exists\n");
			return;
		}
		tmp1 = tmp1->next;
	}

	File *file = (File*)malloc(sizeof(File));
	file->name = (char*)malloc(sizeof(name));
	strcpy(file->name, name);
	file->next = NULL;
	file->parent = parent;

	// update current directory's file list
	tmp = parent->head_children_files;
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
	// check if directory already exists
	Dir *tmp = parent->head_children_dirs;
	while(tmp){
		if(strcmp(tmp->name, name) ==  0){
			printf("Directory already exists\n");
			return;
		}
		tmp = tmp->next;
	}
	File *tmp1 = parent->head_children_files;
	while(tmp1){
		if(strcmp(tmp1->name, name) ==  0){
			printf("File already exists\n");
			return;
		}
		tmp1 = tmp1->next;
	}

	Dir *dir = (Dir*)malloc(sizeof(Dir));
	dir->head_children_dirs = NULL;
	dir->head_children_files = NULL;
	dir->name = (char*)malloc(sizeof(name));
	strcpy(dir->name, name);
	dir->next = NULL;
	dir->parent = parent;

	// update current directory's directory list
	tmp = parent->head_children_dirs;
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

void rm (Dir* parent, char* name) {
	// look for file
	File *tmp = parent->head_children_files;
	File *del;
	if(tmp){
		if(strcmp(tmp->name, name) ==  0){
			del = tmp;
			parent->head_children_files = tmp->next;
			free(del->name);
			free(del);
			return;
		}

		while(tmp->next){
			if(strcmp(tmp->next->name, name) ==  0){
				del = tmp->next;
				tmp->next = tmp->next->next;
				free(del->name);
				free(del);
				return;
			}
			tmp = tmp->next;
		}
	}
	
	printf("Could not find the file\n");
}

void rmdir (Dir* parent, char* name) {
	// look for directory
	Dir *tmp = parent->head_children_dirs;
	Dir *del;
	if(tmp){
		if(strcmp(tmp->name, name) ==  0){
			freeSystem(tmp);
			del = tmp;
			parent->head_children_dirs = tmp->next;
			free(del->name);
			free(del);
			return;
		}

		while(tmp->next){
			if(strcmp(tmp->next->name, name) ==  0){
				freeSystem(tmp->next);
				del = tmp->next;
				tmp->next = tmp->next->next;
				free(del->name);
				free(del);
				return;
			}
			tmp = tmp->next;
		}
	}
	
	printf("Could not find the dir\n");
}

void cd(Dir** target, char *name) {
	Dir *tmp = *target;

	if(strcmp(name, "..") == 0){
		if(tmp->parent == NULL){
			return;
		}
		*target = (*target)->parent;
	}
	else{
		tmp = (*target)->head_children_dirs;
		
		while(tmp){
			if(strcmp(tmp->name, name) ==  0){
				*target = tmp;
				return;
			}
			tmp = tmp->next;
		}
		
		printf("No directories found!\n");
	}
}

char *pwd (Dir* target) {
	char *path = (char*)malloc(MAX_INPUT_LINE_SIZE);
	path[0] = '\0';
	char *tmp = (char*)malloc(MAX_INPUT_LINE_SIZE);

	while(target != NULL){
		strcpy(tmp, path);
		strcpy(path, "/");
		strcat(path, target->name);
		strcat(path, tmp);
		target = target->parent;
	}
	free(tmp);
	path = (char*)realloc(path, strlen(path) + 1);
	return path;
}

void stop (Dir* target) {
	freeSystem(target);
	free(target->name);
	free(target);
}

void tree (Dir* target, int level) {
	// print parent dir's name
	if(level != -1){
		tabPrinter(level);
		printf("%s\n", target->name);
	}
	
	// go thru its children directories
	Dir *tmp = target->head_children_dirs;
	while(tmp){
		tree(tmp, level + 1);
		tmp = tmp->next;
	}

	// print files
	File *tmp1 = target->head_children_files;
	while(tmp1){
		tabPrinter(level + 1);
		printf("%s\n", tmp1->name);
		tmp1 = tmp1->next;
	}
}

void mv(Dir* parent, char *oldname, char *newname) {
	// check if directory already exists
	Dir *tmp = parent->head_children_dirs;
	Dir *tmp2;

	if(tmp){
		// when directory in question is found
		if(strcmp(tmp->name, oldname) ==  0){
			Dir *tmp1 = parent->head_children_dirs;
			File *tmp11 = parent->head_children_files;

			// check if newname doesnt already exist
			if(tmp1){
				if(strcmp(tmp1->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				
				while(tmp1->next){
				if(strcmp(tmp1->next->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				tmp1 = tmp1->next;
			}
			}
			
			if(tmp11){
				if(strcmp(tmp11->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				while(tmp11->next){
				if(strcmp(tmp11->next->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				tmp11 = tmp11->next;
			}
			}
			
			strcpy(tmp->name, newname);
			parent->head_children_dirs = (tmp->next == NULL) ? tmp : tmp->next;
			if(tmp->next != NULL){
				tmp->next = NULL;
				tmp1->next = tmp;
			}
			tmp->next = NULL;
			return;
		}
		while(tmp->next){

		// when directory in question is found
		if(strcmp(tmp->next->name, oldname) ==  0){
			Dir *tmp1 = parent->head_children_dirs;
			File *tmp11 = parent->head_children_files;

			// check if newname doesnt already exist
			if(tmp1){
				if(strcmp(tmp1->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				while(tmp1->next){
				if(strcmp(tmp1->next->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				tmp1 = tmp1->next;
			}
			}
			
			if(tmp11){
				if(strcmp(tmp11->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				while(tmp11->next){
				if(strcmp(tmp11->next->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				tmp11 = tmp11->next;
			}
			}
			

			tmp2 = tmp->next;
			strcpy(tmp2->name, newname);
			tmp->next = tmp->next->next;
			tmp2->next = NULL;
			tmp1->next = tmp2;
			return;
		}
		tmp = tmp->next;
	}
	if(tmp->next == NULL){
		// when directory in question is found
		if(strcmp(tmp->name, oldname) ==  0){
			Dir *tmp1 = parent->head_children_dirs;
			File *tmp11 = parent->head_children_files;

			// check if newname doesnt already exist
			while(tmp1){
				if(strcmp(tmp1->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				tmp1 = tmp1->next;
			}
			while(tmp11){
				if(strcmp(tmp11->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				tmp11 = tmp11->next;
			}

			strcpy(tmp->name, newname);
			return;
		}
		tmp = tmp->next;
	}
	}

	// check if file already exists
	File *tmpF = parent->head_children_files;
	File *tmp2F;

	if(tmpF){
		// when file in question is found
		if(strcmp(tmpF->name, oldname) ==  0){
			File *tmp1F = parent->head_children_files;
			Dir *tmp11F = parent->head_children_dirs;

			// check if newname doesnt already exist
			if(tmp1F){
				if(strcmp(tmp1F->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				while(tmp1F->next){
				if(strcmp(tmp1F->next->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				tmp1F = tmp1F->next;
			}
			}
			
			if(tmp11F){
				if(strcmp(tmp11F->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				while(tmp11F->next){
				if(strcmp(tmp11F->next->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				tmp11F = tmp11F->next;
			}
			}
			

			strcpy(tmpF->name, newname);
			parent->head_children_files = tmpF->next;
			tmpF->next = NULL;
			tmp1F->next = tmpF;
			return;
		}
		while(tmpF->next){

		// when file in question is found
		if(strcmp(tmpF->next->name, oldname) ==  0){
			File *tmp1F = parent->head_children_files;
			Dir *tmp11F = parent->head_children_dirs;

			// check if newname doesnt already exist
			if(tmp1F){
				if(strcmp(tmp1F->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				while(tmp1F->next){
				if(strcmp(tmp1F->next->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				tmp1F = tmp1F->next;
			}
			}
			
			if(tmp11F){
				if(strcmp(tmp11F->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				while(tmp11F->next){
				if(strcmp(tmp11F->next->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				tmp11F = tmp11F->next;
			}
			}
			

			tmp2F = tmpF->next;
			strcpy(tmp2->name, newname);
			tmpF->next = tmpF->next->next;
			tmp2F->next = NULL;
			tmp1F->next = tmp2F;
			return;
		}
		tmpF = tmpF->next;
	}
	if(tmpF->next == NULL){
		// when file in question is found
		if(strcmp(tmpF->name, oldname) ==  0){
			File *tmp1F = parent->head_children_files;
			Dir *tmp11F = parent->head_children_dirs;

			// check if newname doesnt already exist
			while(tmp1F){
				if(strcmp(tmp1F->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				tmp1F = tmp1F->next;
			}
			while(tmp11F){
				if(strcmp(tmp11F->name, newname) == 0){
					printf("File/Director already exists\n");
					return;
				}
				tmp11F = tmp11F->next;
			}

			strcpy(tmpF->name, newname);
			return;
		}
		tmpF = tmpF->next;
	}
	}
	
	printf("File/Director not found\n");
}

Instruction* parser(char *str){
	Instruction *instruction = (Instruction*)malloc(sizeof(Instruction));
	instruction->cmd = (char*)calloc(1, MAX_INPUT_LINE_SIZE);
	instruction->arg1 = (char*)malloc(MAX_INPUT_LINE_SIZE);
	instruction->arg2 = (char*)malloc(MAX_INPUT_LINE_SIZE);

	// status 0 for reading cmd; status 1 for reading arg1
	int status = 0, k = 0, l = 0, m = 0;

	for(int i = 0; i < strlen(str) - 1; i++){
		if(str[i] == ' '){
			// start reading arg1/arg2
			status++;
			i++;
		}
		if(status == 0){
			instruction->cmd[m++] = str[i];
		}
		else if(status == 1){
			instruction->arg1[k++] = str[i];
		}
		else if(status == 2){
			instruction->arg2[l++] = str[i];
		}
	}
	instruction->cmd[m] = '\0';
	instruction->cmd = (char*)realloc(instruction->cmd, strlen(instruction->cmd) + 1);
	instruction->arg1[k] = '\0';
	instruction->arg1 = (char*)realloc(instruction->arg1, strlen(instruction->arg1) + 1);
	instruction->arg2[l] = '\0';
	instruction->arg2 = (char*)realloc(instruction->arg2, strlen(instruction->arg2) + 1);
	return instruction;
}

int main (int argc, char *argv[]) {
	// FILE *f = fopen(argv[1], "r");
	char *str = (char*)malloc(MAX_INPUT_LINE_SIZE);
	Dir *ROOT = root("home");
	Dir *currentDir = ROOT;

	do
	{
		// reads instructions one-by-one
		fgets(str, MAX_INPUT_LINE_SIZE, stdin);	

		// parsing the input file
		Instruction *instruction = parser(str);
		// printf("Command: \"%s\", argument1: \"%s\", argument2: \"%s\"\n", instruction->cmd, instruction->arg1, instruction->arg2);

		// apply instructions
		if(strcmp(instruction->cmd, "stop") == 0){
			stop(ROOT);
			free(instruction->arg1);
			free(instruction->arg2);
			free(instruction->cmd);
			free(instruction);
			break;
		}
		else if(strcmp(instruction->cmd, "touch") == 0){
			touch(currentDir, instruction->arg1);
		}
		else if(strcmp(instruction->cmd, "mkdir") == 0){
			mkdir(currentDir, instruction->arg1);
		}
		else if(strcmp(instruction->cmd, "ls") == 0){
			ls(currentDir);
		}
		else if(strcmp(instruction->cmd, "rm") == 0){
			rm(currentDir, instruction->arg1);
		}
		else if(strcmp(instruction->cmd, "rmdir") == 0){
			rmdir(currentDir, instruction->arg1);
		}
		else if(strcmp(instruction->cmd, "cd") == 0){
			cd(&currentDir, instruction->arg1);
		}
		else if(strcmp(instruction->cmd, "pwd") == 0){
			char* path = pwd(currentDir);
			printf("%s\n", path);
			free(path);
		}
		else if(strcmp(instruction->cmd, "tree") == 0){
			tree(currentDir, -1);
		}
		else if(strcmp(instruction->cmd, "mv") == 0){
			mv(currentDir, instruction->arg1, instruction->arg2);
		}
		free(instruction->arg1);
		free(instruction->arg2);
		free(instruction->cmd);
		free(instruction);
	} while (1);

	free(str);
	// fclose(f);

	return 0;
}
