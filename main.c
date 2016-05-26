#include <stdlib.h>
#include <stdio.h>
#include "bdd.h"
#include "gui.h"

int main(int argc, char **argv) {

  char str[] = "12345.56";
  char str2[] = "123a123";
  double d, c;

  sscanf(str, "%lf", &d);
  sscanf(str2, "%lf", &c);

  printf("%lf\n", d);

  printf("%lf\n", c);

  bdd_init();
  gui_init(argc, argv);

  return 0;
}
