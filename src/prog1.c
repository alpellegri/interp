#include <stdio.h>
#include <string.h>

#include "prog1.h"
#include "slp.h"
#include "util.h"

/*
 * a := 5+3;
 * b := (print(a, a-1), 10*a);
 * print(b);
 */
A_stm prog(void) {

  return A_CompoundStm(
      A_AssignStm("a", A_OpExp(A_NumExp(5), A_add, A_NumExp(3))),
      A_CompoundStm(
          A_AssignStm("b",
                      A_EseqExp(A_PrintStm(A_PairExpList(
                                    A_IdExp("a"),
                                    A_PairExpList(A_OpExp(A_IdExp("a"), A_sub,
                                                          A_NumExp(1)),
                                                  0))),
                                A_OpExp(A_NumExp(10), A_mul, A_IdExp("a")))),
          A_PrintStm(A_PairExpList(A_IdExp("b"), 0))));
}

/*
 * a := 5;
 * print(a);
 */
A_stm _prog_test1(void) {

  return A_CompoundStm(A_AssignStm("a", A_NumExp(5)),
                       A_PrintStm(A_PairExpList(A_IdExp("a"), 0)));
}

A_stm prog_test1(void) {

  return A_CompoundStm(
      A_AssignStm("a", A_NumExp(5)),
      A_CompoundStm(A_PrintStm(A_PairExpList(A_IdExp("a"), 0)), NULL));
}

/*
 * a := 5;
 * print(a, a);
 */
A_stm prog_test2(void) {

  return A_CompoundStm(
      A_AssignStm("a", A_NumExp(5)),
      A_CompoundStm(A_PrintStm(A_PairExpList(A_IdExp("a"),
                                             A_PairExpList(A_IdExp("a"), 0))),
                    NULL));
}

/*
 * a = 5+3;
 * b = (print(a, (print(a, a, 100, 200, 300), 100), a-1), 10*a);
 * print(b);
 */
A_stm _prog_test3(void) {

  return A_CompoundStm(
      A_AssignStm("a", A_OpExp(A_NumExp(5), A_add, A_NumExp(3))),
      A_CompoundStm(
          A_AssignStm(
              "b",
              A_EseqExp(
                  A_PrintStm(A_PairExpList(
                      A_IdExp("a"),
                      A_PairExpList(
                          A_EseqExp(A_PrintStm(A_PairExpList(
                                        A_IdExp("a"),
                                        A_PairExpList(
                                            A_IdExp("a"),
                                            A_PairExpList(
                                                A_NumExp(100),
                                                A_PairExpList(
                                                    A_NumExp(200),
                                                    A_PairExpList(A_NumExp(300),
                                                                  0)))))),
                                    A_NumExp(100)),
                          A_PairExpList(
                              A_OpExp(A_IdExp("a"), A_sub, A_NumExp(1)),
                              0)))),
                  A_OpExp(A_NumExp(10), A_mul, A_IdExp("a")))),
          A_PrintStm(A_PairExpList(A_IdExp("b"), 0))));
}

/*
 * a = 5+3;
 * b = 10*a;
 * print(b);
 */
A_stm prog_test3(void) {

  return A_CompoundStm(
      A_AssignStm("a", A_OpExp(A_NumExp(5), A_add, A_NumExp(3))),
      A_CompoundStm(
          A_AssignStm("b", A_OpExp(A_NumExp(10), A_mul, A_IdExp("a"))),
          A_CompoundStm(A_PrintStm(A_PairExpList(A_IdExp("b"), 0)), NULL)));
}
