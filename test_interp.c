#include <stdio.h>

#include "interp.h"
#include "prog1.h"
#include "slp.h"
#include "util.h"

int main() {

  A_prog_p stm;

  printf("prog():\n");
  // stm = prog_test2();
  stm = prog_test3();
  printf("display_stm:\n");
  display_stm(stm);
  printf("interp:\n");
  interper(stm);

  return 0;
}
