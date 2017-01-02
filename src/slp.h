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

typedef struct A_stmList_s *A_stmList_p;
typedef struct A_stm_s *A_stm_p;
typedef struct A_exp_s *A_exp_p;
typedef struct A_expList_s *A_expList_p;

/*
* StmList -> Stm, StmList  (StmList)
* StmList -> Stm  epsilon  (StmList)
 */
struct A_stmList_s {
  enum { A_stmList } kind;
  A_stm_p head;
  A_stmList_p tail;
} A_stmList_t;
extern A_stmList_p A_StmList(A_stm_p head, A_stmList_p tail);

/*
 * Stm -> id := Exp      (AssignStm)
 * Stm -> print(ExpList) (PrintStm)
 * Stm -> if(Exp)        (IfStm)
 */
struct A_stm_s {
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
      A_stmList_p then;
      A_stmList_p otherwise;
    } if_kw;
  } u;
} A_stm_t;
extern A_stm_p A_AssignStm(string id, A_exp_p exp);
extern A_stm_p A_PrintStm(A_expList_p exps);
extern A_stm_p A_IfStm(A_exp_p cond, A_stmList_p then, A_stmList_p otherwise);

/*
 * Exp -> id             (IdExp)
 * Exp -> num           (NumExp)
 * Exp -> str           (StrExp)
 * Exp -> Exp Binop Exp  (OpExp)
 */
struct A_exp_s {
  enum { A_idExp, A_numExp, A_strExp, A_opExp } kind;
  union {
    string id;
    int num;
    string str;
    struct {
      A_exp_p left;
      A_binop oper;
      A_exp_p right;
    } op;
  } u;
} A_exp_t;
extern A_exp_p A_IdExp(string id);
extern A_exp_p A_NumExp(int num);
extern A_exp_p A_StrExp(string str);
extern A_exp_p A_OpExp(A_exp_p left, A_binop oper, A_exp_p right);

/*
 * ExpList -> Exp, ExpList  (ExpList)
 * ExpList -> Exp  epsilon  (ExpList)
 */
struct A_expList_s {
  enum { A_expList } kind;
  A_exp_p head;
  A_expList_p tail;
} A_expList_t;
extern A_expList_p A_ExpList(A_exp_p head, A_expList_p tail);

extern void display_stmList(A_stmList_p stmList);
extern void display_stm(A_stm_p stm);
extern void display_exp(A_exp_p exp);
extern void display_expList(A_expList_p expList);

#ifdef __cplusplus
}
#endif

#endif /* SLP_H */
