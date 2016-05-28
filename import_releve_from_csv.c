#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>
#include "import_releve_from_csv.h"

int get_number_lines(char *filename) {
  FILE *fp = fopen(filename, "r");
  int lines = 0;
  char ch;
  while(!feof(fp))
  {
    ch = fgetc(fp);
    if(ch == '\n')
    {
      lines++;
    }
  }
  fclose(fp);
  return lines;
}
