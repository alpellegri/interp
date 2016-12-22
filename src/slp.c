#include <stdio.h>

#include "slp.h"
#include "util.h"

// struct A_expList_ {
//   enum { A_pairExpList } kind;
char *A_expList_decriptor[1] = {"A_pairExpList"};

void display_expList(A_expList expList) {
  printf("expList: %s\n", A_expList_decriptor[expList->kind]);
  if (expList->kind == A_pairExpList) {
    display_exp(expList->head);
    if (expList->tail != NULL) {
      display_expList(expList->tail);
    } else {
      printf("expList: null tail\n");
    }
  } else {
    printf("error %s\n", A_expList_decriptor[expList->kind]);
  }
}

// struct A_exp_ {
//   enum { A_idExp, A_numExp, A_opExp, A_eseqExp } kind;
char *A_exp_decriptor[4] = {
    "A_idExp", "A_numExp", "A_opExp", "A_eseqExp",
};
// enum { A_plus, A_minus, A_times, A_div } A_binop;
char *A_binop_decriptor[4] = {
    "+", "-", "*", "/",
};

void display_exp(A_exp exp) {
  printf("exp: %s\n", A_exp_decriptor[exp->kind]);
  if (exp->kind == A_idExp) {
    printf(">%s_\n", exp->u.id);
  } else if (exp->kind == A_numExp) {
    printf(">%d_\n", exp->u.num);
  } else if (exp->kind == A_opExp) {
    display_exp(exp->u.op.left);
    printf(">%s_\n", A_binop_decriptor[exp->u.op.oper]);
    display_exp(exp->u.op.right);
  } else if (exp->kind == A_eseqExp) {
    display_exp(exp->u.eseq.exp);
  } else {
    printf("error %s\n", A_exp_decriptor[exp->kind]);
  }
}

// struct A_stm_ {
//   enum { A_compoundStm, A_assignStm, A_printStm } kind;
char *A_stm_decriptor[3] = {
    "A_compoundStm", "A_assignStm", "A_printStm",
};

void display_stm(A_stm stm) {
  printf("stm: %s\n", A_stm_decriptor[stm->kind]);
  if (stm->kind == A_compoundStm) {
    display_stm(stm->u.compound.stm1);
    if (stm->u.compound.stm2 != NULL) {
      display_stm(stm->u.compound.stm2);
    } else {
      printf("expList: null stm\n");
    }
  } else if (stm->kind == A_assignStm) {
    display_exp(stm->u.assign.exp);
  } else if (stm->kind == A_printStm) {
    if (stm->u.print.exps != NULL) {
      display_expList(stm->u.print.exps);
    }
  } else {
    printf("error %s\n", A_stm_decriptor[stm->kind]);
  }
}

A_stm A_CompoundStm(A_stm stm1, A_stm stm2) {
  printf("spl create A_CompoundStm\n");
  A_stm s = checked_malloc(sizeof *s);
  s->kind = A_compoundStm;
  s->u.compound.stm1 = stm1;
  s->u.compound.stm2 = stm2;
  return s;
}

A_stm A_AssignStm(string id, A_exp exp) {
  printf("spl create A_AssignStm\n");
  A_stm s = checked_malloc(sizeof *s);
  s->kind = A_assignStm;
  s->u.assign.id = id;
  s->u.assign.exp = exp;
  return s;
}

A_stm A_PrintStm(A_expList exps) {
  printf("spl create A_PrintStm\n");
  A_stm s = checked_malloc(sizeof *s);
  s->kind = A_printStm;
  s->u.print.exps = exps;
  return s;
}

A_exp A_IdExp(string id) {
  printf("spl create A_IdExp\n");
  A_exp e = checked_malloc(sizeof *e);
  e->kind = A_idExp;
  e->u.id = id;
  return e;
}

A_exp A_NumExp(int num) {
  printf("spl create A_NumExp\n");
  A_exp e = checked_malloc(sizeof *e);
  e->kind = A_numExp;
  e->u.num = num;
  return e;
}

A_exp A_OpExp(A_exp left, A_binop oper, A_exp right) {
  printf("spl create A_OpExp\n");
  A_exp e = checked_malloc(sizeof *e);
  e->kind = A_opExp;
  e->u.op.left = left;
  e->u.op.oper = oper;
  e->u.op.right = right;
  return e;
}

A_exp A_EseqExp(A_stm stm, A_exp exp) {
  printf("spl create A_EseqExp\n");
  A_exp e = checked_malloc(sizeof *e);
  e->kind = A_eseqExp;
  e->u.eseq.stm = stm;
  e->u.eseq.exp = exp;
  return e;
}

A_expList A_PairExpList(A_exp head, A_expList tail) {
  printf("spl create A_PairExpList\n");
  A_expList e = checked_malloc(sizeof *e);
  e->kind = A_pairExpList;
  e->head = head;
  e->tail = tail;
  return e;
}
