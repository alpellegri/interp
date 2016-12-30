#include <stdio.h>

#include "slp.h"
#include "util.h"

// struct A_expList_ {
//   enum { A_pairExpList } kind;
char *A_expList_decriptor[1] = {"A_pairExpList"};

void display_expList(A_expList_p expList) {
  printf("A_expList: %s\n", A_expList_decriptor[expList->kind]);
  if (expList->kind == A_pairExpList) {
    display_exp(expList->head);
    if (expList->tail != NULL) {
      display_expList(expList->tail);
    } else {
      printf("A_expList: end tail\n");
    }
  } else {
    printf("A_expList error %s\n", A_expList_decriptor[expList->kind]);
  }
}

// struct A_exp_ {
//   enum { A_idExp, A_numExp, A_opExp, A_eseqExp } kind;
char *A_exp_decriptor[4] = {
    "A_idExp", "A_numExp", "A_opExp", "A_eseqExp",
};
// enum { A_add, A_sub, A_mul, A_div, A_eq, A_ne, A_le, A_lt, A_ge, A_gt }
// A_binop;
char *A_binop_decriptor[10] = {
    "+", "-", "*", "/", "==", "!=", "<=", "<", ">=", ">",
};

void display_exp(A_exp_p exp) {
  printf("A_exp: %s\n", A_exp_decriptor[exp->kind]);
  if (exp->kind == A_idExp) {
    printf("_%s_\n", exp->u.id);
  } else if (exp->kind == A_numExp) {
    printf("_%d_\n", exp->u.num);
  } else if (exp->kind == A_opExp) {
    display_exp(exp->u.op.left);
    printf("_%s_\n", A_binop_decriptor[exp->u.op.oper]);
    display_exp(exp->u.op.right);
  } else {
    printf("A_exp error %s\n", A_exp_decriptor[exp->kind]);
  }
}

// struct A_stm_ {
//   enum { A_assignStm, A_printStm, A_ifStm } kind;
char *A_stm_decriptor[3] = {
    "A_assignStm", "A_printStm", "A_ifStm",
};

void display_stm(A_prog_p stm) {
  printf("A_progStm: %s\n", A_stm_decriptor[stm->kind]);
  if (stm->kind == A_assignStm) {
    display_exp(stm->u.assign.exp);
  } else if (stm->kind == A_printStm) {
    if (stm->u.print.exps != NULL) {
      display_expList(stm->u.print.exps);
    }
  } else if (stm->kind == A_ifStm) {
    if (stm->u.if_kw.cond != NULL) {
      display_exp(stm->u.if_kw.cond);
    } else {
      printf("A_progStm error if_kw %s\n", A_stm_decriptor[stm->kind]);
    }
    if (stm->u.if_kw.then != NULL) {
      display_stm(stm->u.if_kw.then);
    } else {
      printf("A_progStm error if_kw %s\n", A_stm_decriptor[stm->kind]);
    }
    if (stm->u.if_kw.otherwise != NULL) {
      display_stm(stm->u.if_kw.otherwise);
    }
  } else {
    printf("A_progStm error %s\n", A_stm_decriptor[stm->kind]);
  }

  if (stm->tail != NULL) {
    display_stm(stm->tail);
  } else {
    printf("A_progStm: end of A_progStm\n");
  }
}

A_prog_p A_AssignStm(string id, A_exp_p exp) {
  printf("spl create A_AssignStm\n");
  A_prog_p s = checked_malloc(sizeof *s);
  s->kind = A_assignStm;
  s->u.assign.id = id;
  s->u.assign.exp = exp;
  s->tail = NULL;
  return s;
}

A_prog_p A_PrintStm(A_expList_p exps) {
  printf("spl create A_PrintStm\n");
  A_prog_p s = checked_malloc(sizeof *s);
  s->kind = A_printStm;
  s->u.print.exps = exps;
  s->tail = NULL;
  return s;
}

A_prog_p A_IfStm(A_exp_p cond, A_prog_p then, A_prog_p otherwise) {
  printf("spl create A_IfStm\n");
  A_prog_p s = checked_malloc(sizeof *s);
  s->kind = A_ifStm;
  s->u.if_kw.cond = cond;
  s->u.if_kw.then = then;
  s->u.if_kw.otherwise = otherwise;
  s->tail = NULL;
  return s;
}

A_exp_p A_IdExp(string id) {
  printf("spl create A_IdExp\n");
  A_exp_p e = checked_malloc(sizeof *e);
  e->kind = A_idExp;
  e->u.id = id;
  return e;
}

A_exp_p A_NumExp(int num) {
  printf("spl create A_NumExp\n");
  A_exp_p e = checked_malloc(sizeof *e);
  e->kind = A_numExp;
  e->u.num = num;
  return e;
}

A_exp_p A_OpExp(A_exp_p left, A_binop oper, A_exp_p right) {
  printf("spl create A_OpExp\n");
  A_exp_p e = checked_malloc(sizeof *e);
  e->kind = A_opExp;
  e->u.op.left = left;
  e->u.op.oper = oper;
  e->u.op.right = right;
  return e;
}

A_expList_p A_PairExpList(A_exp_p head, A_expList_p tail) {
  printf("spl create A_PairExpList\n");
  A_expList_p e = checked_malloc(sizeof *e);
  e->kind = A_pairExpList;
  e->head = head;
  e->tail = tail;
  return e;
}
