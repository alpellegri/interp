#include <stdio.h>

#include "interp.h"
#include "prog1.h"
#include "slp.h"
#include "util.h"

int main() {

  A_stm stm;

  printf("prog():\n");
  // stm = prog_test2();
  stm = prog_test3();
  printf("display_stm:\n");
  display_stm(stm);
  printf("interp:\n");
  interp(stm);

  // printf("prog_test4():\n");
  // interp_multi(prog_test4());
  //
  // printf("prog_test5():\n");
  // interp_multi(prog_test5());

  return 0;
}
