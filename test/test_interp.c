#include <stdio.h>

#include "interp.h"
#include "prog1.h"
#include "slp.h"
#include "util.h"

int main() {

  A_stmList_p stmList;

  printf("\nprog_test0:\n");
  stmList = prog_test0();
  printf("display_stm:\n");
  A_stmList_display(stmList);
  printf("interp:\n");
  interp(stmList);

  printf("\nprog_test1:\n");
  stmList = prog_test1();
  printf("display_stm:\n");
  A_stmList_display(stmList);
  printf("interp:\n");
  interp(stmList);

  printf("\nprog_test2:\n");
  stmList = prog_test2();
  printf("display_stm:\n");
  A_stmList_display(stmList);
  printf("interp:\n");
  interp(stmList);

  return 0;
}
