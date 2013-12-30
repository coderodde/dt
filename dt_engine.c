#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define DTHOME         "/.dt"
#define TABLE_FILE     "/table"
#define MAX_TAG_LENGTH 32
#define MIN(x,y) ((x) < (y) ? (x) : (y))

static int lev_distance_impl(const char* a, const char* b, int i, int j) {
  if (i == 0) {
    if (j == 0) {
      return 0;
    } else {
      return j;
    }
  } else if (j == 0) {
    return i;
  }

  return MIN(MIN(lev_distance_impl(a, b, i - 1, j) + 1,
                 lev_distance_impl(a, b, i, j - 1) + 1),
             lev_distance_impl(a, b, i - 1, j - 1) +
                 (a[i - 1] != b[j - 1] ? 1 : 0));
}

void lower(char* a) {
  int i;

  for (i = 0; a[i]; ++i) {
    a[i] = tolower(a[i]);
  }
}

int lev_distance(const char* a, const char* b) {
  return lev_distance_impl(a, b, strlen(a), strlen(b));
}

int main (int argc, const char * argv[]) {
	int tmpint;
  int mindist = 1000 * 1000 * 1000;
  int tmpdist;
  int max_allowed_dist = 4;
  char* bestpath[FILENAME_MAX];
	FILE* f;
	char* home;
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
	
  lower(argv[1]);

	while (!feof(f)
		   && !ferror(f)
		   && (tmpint = fscanf(f,
                           "%s %s\n",
                           tag,
                           file_path)) != EOF) {
    lower(tag);

    if (strcmp(tag, argv[1]) == 0) {
      puts(file_path);
      return EXIT_SUCCESS;
    } else if (mindist > (tmpdist = lev_distance(tag, argv[1]))) {
      mindist = tmpdist;
      strcpy(bestpath, file_path);
    }
  }

  // If here, no exact match.
  if (mindist <= max_allowed_dist) {
    puts(bestpath);
  }

	fclose(f);
	return EXIT_SUCCESS;
}
