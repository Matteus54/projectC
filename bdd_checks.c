#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "bdd_checks.h"


int isNumeric(const char *string, int isDecimal) {
  int virgulePresente = 0;
  int isDigit = 1;
  int length = strlen(string);
  if(length == 0) {
    isDigit = 0;
  }
  int i;
  for(i = 0; i < length; i++) {
    char c = string[i];
    if(!isdigit(c)) {
      if(isDecimal) {
        if(!(c == '.')) {
          isDigit = 0;
        }
        else {
          if(!virgulePresente) {
            virgulePresente = 1;
          }
          else {
            isDigit = 0;
          }
        }
      }
      else {
        isDigit = 0;
      }
    }
  }
  return isDigit;
}
