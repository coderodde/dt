#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define VERSION			0.1
#define DTHOME			"/.dt"
#define TABLE_FILE		"/table"
#define MAX_TAG_LENGTH	32

int main (int argc, const char * argv[]) {
	int tmpint;
	FILE* f;
	char * home;
	char tag[MAX_TAG_LENGTH + 1];
	char file_path[FILENAME_MAX];
	
	if (argc != 2) {
		return EXIT_FAILURE;
	}
	
	memset(tag, 0, MAX_TAG_LENGTH + 1);
	memset(file_path, 0, FILENAME_MAX);
	
	/* construct the path to file "~/.dt/table" */
	home = getenv("HOME");	
	strcpy(file_path, home);
	strcpy(&file_path[strlen(file_path)], DTHOME);
	
	/* create the "~/.dt" folder if not yet present */
	mkdir(file_path, S_IRWXU);
	
	tmpint = strlen(file_path);
	strcpy(&file_path[strlen(file_path)], TABLE_FILE);
	
	/* now the path to the table file is in file_path */
	f = fopen(file_path, "rw");
	
	if (!f) {
		return EXIT_FAILURE;
	}
	
	while (!feof(f)
		   && !ferror(f)
		   && (tmpint = fscanf(f,
							   "%s %s\n",
							   tag,
							   file_path)) != EOF) {
			   if (strcmp(tag, argv[1]) == 0) {
				   puts(file_path);
				   break;
			   }
		   }
	
	fclose(f);
	return EXIT_SUCCESS;
}
