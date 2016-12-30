#include <stdio.h>
#include <string.h>

#include "prog1.h"
#include "slp.h"
#include "util.h"

/*
 * a := 5;
 * print(a);
 */

A_prog_p prog_test1(void) {

  return A_AssignStm("a", A_NumExp(5),
                     A_PrintStm(A_ExpList(A_IdExp("a"), 0), NULL));
}

/*
 * a := 5;
 * print(a, a);
 */
A_prog_p prog_test2(void) {

  return A_AssignStm(
      "a", A_NumExp(5),
      A_PrintStm(A_ExpList(A_IdExp("a"), A_ExpList(A_IdExp("a"), 0)), NULL));
}

/*
 * a = 5+3;
 * b = 10*a;
 * print(b);
 */
A_prog_p prog_test3(void) {

  return A_AssignStm("a", A_OpExp(A_NumExp(5), A_add, A_NumExp(3)),
                     A_AssignStm("b",
                                 A_OpExp(A_NumExp(10), A_mul, A_IdExp("a")),
                                 A_PrintStm(A_ExpList(A_IdExp("b"), 0), NULL)));
}
