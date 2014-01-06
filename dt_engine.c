#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define DTHOME               "/.dt"
#define TABLE_FILE           "/table"
#define MAX_TAG_LENGTH       32
#define MAX_TAG_LENGTH_STR   "32"
#define MIN(x,y)             ((x) < (y) ? (x) : (y))
#define DEFAULT_MAX_DISTANCE 2

static char *const TOO_LONG_TAG =
"tag is longer than " MAX_TAG_LENGTH_STR " characters";

static int 
lev_distance_impl(const char* a, const char* b, int i, int j) 
{
  if (i == 0) {
    if (j == 0) 
      return 0;
    else 
      return j;
    }
  else if (j == 0)
    return i;

  return MIN(MIN(lev_distance_impl(a, b, i - 1, j) + 1,
                 lev_distance_impl(a, b, i, j - 1) + 1),
             lev_distance_impl(a, b, i - 1, j - 1) +
                 (a[i - 1] != b[j - 1] ? 1 : 0));
}

static int
lev_distance(const char* a, const char* b) 
{
  return lev_distance_impl(a, b, strlen(a), strlen(b));
}

static void 
lower(char* a) 
{
  int i;

  for (i = 0; a[i]; ++i) 
    a[i] = tolower(a[i]);
}

static char*
expand_tilde() 
{
  char* buffer = (char*) malloc(sizeof(char) * FILENAME_MAX);
  strncpy(buffer, getenv("HOME"), FILENAME_MAX - 1);
  return buffer;
}

/* Parses the tag and path from a line if possible.
Returns NULL if no (tag-path) pair is found.
If the parsed tag contains more characters than MAX_TAG_LENGTH,
returns a char[2] where first index points to the tag and the 
second one points to static string TOO_LONG_TAG.  */
static char**
split_line(char* line) 
{
  int i;
  int i1; /* The index of a first character of a tag.  */
  int i2; /* The index of a last character of a tag.  */
  int i3; /* The index of a first character of a path.  */
  int i4; /* The index of a last character of a path.  */
  int sz = strlen(line);
  int from_index;
  char** ret;

  i1 = 0;

  while (i1 < sz && isspace(line[i1])) 
    i1++;

  if (i1 == sz) 
    /* Empty line or contains only white space.  */
    return NULL;

  i2 = i1;

  while (i2 < sz && !isspace(line[i2]))
    i2++;

  if (i2 == sz) 
    /* No room for the path. */
    return NULL;

  i3 = i2;
  i2--;

  while (i3 < sz && isspace(line[i3]))
    i3++;

  if (i3 == sz)
    /* No path at all. */
    return NULL;

  i4 = sz - 1;

  while (i4 >= i3 && isspace(line[i4]))
    i4--;

  if (i4 < i3) 
    /* No path here either. */
    return NULL;

  ret = calloc (2, sizeof(char*));
  ret[0] = malloc (i2 - i1 + 2);
  
  for (i = i1; i <= i2; ++i)
    ret[0][i - i1] = line[i];

  ret[0][i2 + 1] = '\0';

  if (i2 - i1 + 1 > MAX_TAG_LENGTH) {
    ret[1] = TOO_LONG_TAG;
    return ret;
  }

  ret[1] = malloc((i4 - i3 + 2) * sizeof(char));
  
  for (i = i3; i <= i4; ++i)
    ret[1][i - i3] = line[i];
    
  ret[1][i4 + 1] = '\0';
  return ret; 
}

int main (int argc, const char * argv[]) {
	int i;
  int sz;
  int tmpint;
  int mindist = 1000 * 1000 * 1000;
  int tmpdist;
  int max_allowed_dist = DEFAULT_MAX_DISTANCE;
  char bestpath[FILENAME_MAX];
	FILE* f;
	char* home;
  char line[2 *  FILENAME_MAX];
	char tag[MAX_TAG_LENGTH + 1];
	char file_path[FILENAME_MAX];
  char arg_tag[MAX_TAG_LENGTH + 1];
  char* tilde; /* For storing result of expanding home operator '~'.  */
  char* tmp;

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
	
  strncpy(arg_tag, argv[1], MAX_TAG_LENGTH);
  lower(arg_tag);

  while (!feof(f) && !ferror(f)) 
  {
    tmp = fgets(line, 2 * FILENAME_MAX - 1, f);

    if (!tmp) 
      break;

    char** pair = split_line(line);
  
    if (!pair) 
      {
      /* Bad line.  */
      continue;
      }
    
    if (pair[1] == TOO_LONG_TAG) 
      {
      fprintf(stderr, "'%s': tag is too long.", pair[0]);
      free(pair[0]);
      free(pair);
      continue;
      }

    if (strlen(pair[1]) > FILENAME_MAX) 
      {
      fprintf(stderr, "'%s': path name is too long.", pair[1]);
      free(pair[0]);
      free(pair[1]);
      free(pair);
      continue;      
      }

    strcpy(tag, pair[0]);
    strcpy(file_path, pair[1]);

    lower(tag);

    if (file_path[0] == '~') {
      tilde = expand_tilde();
      sz = strlen(tilde);
      
      if (file_path[1] == '/') {
        for (i = strlen(file_path); i > 0; --i) {
          file_path[sz + i - 1] = file_path[i];
        }

        for (i = 0; i < sz; ++i) {
          file_path[i] = tilde[i];
        }
      } else {
        file_path[sz] = '\0';
      }

      for (i = 0; i < sz; i++) {
        file_path[i] = tilde[i];
      }
    }
    
    if (strcmp(tag, arg_tag) == 0) {
      puts(file_path);
      return EXIT_SUCCESS;
    } else if (mindist > (tmpdist = lev_distance(tag, arg_tag))) {
      mindist = tmpdist;
      strcpy(bestpath, file_path);
    }
  }

  /* If here, no exact match. */
  if (mindist <= max_allowed_dist) {
    puts(bestpath);
  }

	fclose(f);
	return EXIT_SUCCESS;
}
