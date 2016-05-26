#include <stdlib.h>
#include <stdio.h>
#include "bdd.h"
#include "gui.h"

int main(int argc, char **argv) {

  bdd_init();
  gui_init(argc, argv);

  return 0;
}
