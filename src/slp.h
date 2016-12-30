#ifndef SLP_H
#define SLP_H

#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  A_add,
  A_sub,
  A_mul,
  A_div,
  A_eq,
  A_ne,
  A_le,
  A_lt,
  A_ge,
  A_gt
} A_binop;

typedef struct A_prog_s *A_prog_p;
typedef struct A_exp_s *A_exp_p;
typedef struct A_expList_s *A_expList_p;

/*
 * Stm -> id := Exp      (AssignStm)
 * Stm -> print(ExpList) (PrintStm)
 * Stm -> if(Exp)        (IfStm)
 */
struct A_prog_s {
  enum { A_assignStm, A_printStm, A_ifStm } kind;
  union {
    struct {
      string id;
      A_exp_p exp;
    } assign;
    struct {
      A_expList_p exps;
    } print;
    struct {
      A_exp_p cond;
      A_prog_p then;
      A_prog_p otherwise;
    } if_kw;
  } u;
  A_prog_p tail;
} A_prog_t;
extern A_prog_p A_AssignStm(string id, A_exp_p exp, A_prog_p tail);
extern A_prog_p A_PrintStm(A_expList_p exps, A_prog_p tail);
extern A_prog_p A_IfStm(A_exp_p cond, A_prog_p then, A_prog_p otherwise, A_prog_p tail);

/*
 * Exp -> id             (IdExp)
 * Exp -> num           (NumExp)
 * Exp -> Exp Binop Exp  (OpExp)
 */
struct A_exp_s {
  enum { A_idExp, A_numExp, A_opExp } kind;
  union {
    string id;
    int num;
    struct {
      A_exp_p left;
      A_binop oper;
      A_exp_p right;
    } op;
  } u;
} A_exp_t;
extern A_exp_p A_IdExp(string id);
extern A_exp_p A_NumExp(int num);
extern A_exp_p A_OpExp(A_exp_p left, A_binop oper, A_exp_p right);

/*
 * ExpList -> Exp, ExpList  (PairExpList)
 * ExpList -> Exp  epsilon  (PairExpList)
 */
struct A_expList_s {
  enum { A_expList } kind;
  A_exp_p head;
  A_expList_p tail;
} A_expList_t;
extern A_expList_p A_ExpList(A_exp_p head, A_expList_p tail);

extern void display_stm(A_prog_p stm);
extern void display_exp(A_exp_p exp);
extern void display_expList(A_expList_p expList);

#ifdef __cplusplus
}
#endif

#endif /* SLP_H */
