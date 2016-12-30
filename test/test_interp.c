#include <stdio.h>

#include "interp.h"
#include "prog1.h"
#include "slp.h"
#include "util.h"

int main() {

  A_stmList_p stmList;

  printf("\nprog_test1:\n");
  stmList = prog_test1();
  printf("display_stm:\n");
  display_stmList(stmList);
  printf("interp:\n");
  interper(stmList);

  printf("\nprog_test2:\n");
  stmList = prog_test2();
  printf("display_stm:\n");
  display_stmList(stmList);
  printf("interp:\n");
  interper(stmList);

  return 0;
}