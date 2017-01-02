#include <stdio.h>
#include <string.h>

#include "prog1.h"
#include "slp.h"
#include "util.h"

/*
 * a := 5;
 * print(a);
 */

A_stmList_p prog_test0(void) {
  return A_StmList(A_PrintStm(A_ExpList(A_StrExp("Hello World!"), NULL)), NULL);
}

/*
 * a := 5;
 * print(a);
 */

A_stmList_p prog_test1(void) {
  return A_StmList(A_AssignStm("a", A_NumExp(5)),
                   A_StmList(A_PrintStm(A_ExpList(A_IdExp("a"), NULL)), NULL));
}

/*
 * a = 5+3;
 * b = 10*a;
 * print(b);
 */
A_stmList_p prog_test2(void) {
  return A_StmList(
      A_AssignStm("a", A_OpExp(A_NumExp(5), A_add, A_NumExp(3))),
      A_StmList(A_AssignStm("b", A_OpExp(A_NumExp(10), A_mul, A_IdExp("a"))),
                A_StmList(A_PrintStm(A_ExpList(A_IdExp("b"), NULL)), NULL)));
}
