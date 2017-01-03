#include <stdio.h>

#include "slp.h"
#include "util.h"

// #define DEBUG
#ifdef DEBUG
#define debug_printf(fmt, args...) printf(fmt, ##args)
#else
#define debug_printf(fmt, args...) /* Don't do anything in release builds */
#endif

A_stmList_p A_StmList(A_stm_p head, A_stmList_p tail) {
  debug_printf("spl create A_StmList\n");
  A_stmList_p s = checked_malloc(sizeof *s);
  s->head = head;
  s->tail = tail;
  return s;
}

A_stm_p A_AssignStm(string id, A_exp_p exp) {
  debug_printf("spl create A_AssignStm\n");
  A_stm_p s = checked_malloc(sizeof *s);
  s->kind = A_assignStm;
  s->u.assign.id = id;
  s->u.assign.exp = exp;
  return s;
}

A_stm_p A_PrintStm(A_expList_p exps) {
  debug_printf("spl create A_PrintStm\n");
  A_stm_p s = checked_malloc(sizeof *s);
  s->kind = A_printStm;
  s->u.print.exps = exps;
  return s;
}

A_stm_p A_IfStm(A_exp_p cond, A_stmList_p then, A_stmList_p otherwise) {
  debug_printf("spl create A_IfStm\n");
  A_stm_p s = checked_malloc(sizeof *s);
  s->kind = A_ifStm;
  s->u.if_kw.cond = cond;
  s->u.if_kw.then = then;
  s->u.if_kw.otherwise = otherwise;
  return s;
}

A_exp_p A_IdExp(string id) {
  debug_printf("spl create A_IdExp\n");
  A_exp_p e = checked_malloc(sizeof *e);
  e->kind = A_idExp;
  e->u.id = id;
  return e;
}

A_exp_p A_NumExp(int num) {
  debug_printf("spl create A_NumExp\n");
  A_exp_p e = checked_malloc(sizeof *e);
  e->kind = A_numExp;
  e->u.num = num;
  return e;
}

A_exp_p A_StrExp(string str) {
  debug_printf("spl create A_StrExp\n");
  A_exp_p e = checked_malloc(sizeof *e);
  e->kind = A_strExp;
  e->u.str = str;
  return e;
}

A_exp_p A_OpExp(A_exp_p left, A_binop oper, A_exp_p right) {
  debug_printf("spl create A_OpExp\n");
  A_exp_p e = checked_malloc(sizeof *e);
  e->kind = A_opExp;
  e->u.op.left = left;
  e->u.op.oper = oper;
  e->u.op.right = right;
  return e;
}

A_expList_p A_ExpList(A_exp_p head, A_expList_p tail) {
  debug_printf("spl create A_ExpList\n");
  A_expList_p e = checked_malloc(sizeof *e);
  e->kind = A_expList;
  e->head = head;
  e->tail = tail;
  return e;
}

// struct A_expList_ {
//   enum { A_expList } kind;
char *A_expList_decriptor[1] = {"A_expList"};

void display_expList(A_expList_p expList) {
  printf("A_expList: %s\n", A_expList_decriptor[expList->kind]);
  if (expList->kind == A_expList) {
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
//   enum { A_idExp, A_numExp, A_strExp, A_opExp, A_eseqExp } kind;
char *A_exp_decriptor[5] = {
    "A_idExp", "A_numExp", "A_strExp", "A_opExp", "A_eseqExp",
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
  } else if (exp->kind == A_strExp) {
    printf("_%s_\n", exp->u.str);
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

void display_stm(A_stm_p stm) {
  printf("A_stmStm: %s\n", A_stm_decriptor[stm->kind]);
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
      printf("A_stmStm error if_kw %s\n", A_stm_decriptor[stm->kind]);
    }
    if (stm->u.if_kw.then != NULL) {
      display_stmList(stm->u.if_kw.then);
    } else {
      printf("A_stmStm error if_kw %s\n", A_stm_decriptor[stm->kind]);
    }
    if (stm->u.if_kw.otherwise != NULL) {
      display_stmList(stm->u.if_kw.otherwise);
    }
  } else {
    printf("A_stmStm error %s\n", A_stm_decriptor[stm->kind]);
  }
}

// struct A_stm_ {
//   enum { A_assignStm, A_printStm, A_ifStm } kind;
char *A_stmList_decriptor[1] = {"A_StmList"};

void display_stmList(A_stmList_p stmList) {
  printf("display_stmList: %s\n", A_stmList_decriptor[stmList->kind]);
  display_stm(stmList->head);
  if (stmList->tail != NULL) {
    display_stmList(stmList->tail);
  } else {
    printf("A_stmList: end of A_stmList\n");
  }
}

void destroy_expList(A_expList_p expList) {
  debug_printf("destroy_expList entry\n");
  if (expList->kind == A_expList) {
    if (expList->tail != NULL) {
      destroy_expList(expList->tail);
    }
    if (expList->head != NULL) {
      destroy_exp(expList->head);
      debug_printf("destroy_expList\n");
      checked_free(expList);
    }
  } else {
    printf("destroy_exp error\n");
  }
}

void destroy_exp(A_exp_p exp) {
  debug_printf("destroy_exp entry\n");
  if (exp->kind == A_idExp) {
    debug_printf("destroy_exp A_idExp\n");
    checked_free(exp->u.id);
    checked_free(exp);
  } else if (exp->kind == A_numExp) {
    debug_printf("destroy_exp A_numExp\n");
    checked_free(exp);
  } else if (exp->kind == A_strExp) {
    debug_printf("destroy_exp A_strExp\n");
    checked_free(exp->u.str);
    checked_free(exp);
  } else if (exp->kind == A_opExp) {
    destroy_exp(exp->u.op.left);
    destroy_exp(exp->u.op.right);
    debug_printf("destroy_exp A_opExp\n");
    checked_free(exp);
  } else {
    printf("destroy_exp error\n");
  }
}

void destroy_stm(A_stm_p stm) {
  debug_printf("destroy_stm entry\n");
  if (stm->kind == A_assignStm) {
    destroy_exp(stm->u.assign.exp);
    debug_printf("destroy_stm A_assignStm\n");
    checked_free(stm->u.assign.id);
    checked_free(stm);
  } else if (stm->kind == A_printStm) {
    if (stm->u.print.exps != NULL) {
      destroy_expList(stm->u.print.exps);
      debug_printf("destroy_stm A_printStm\n");
      checked_free(stm);
    }
  } else if (stm->kind == A_ifStm) {
    if (stm->u.if_kw.cond != NULL) {
      destroy_exp(stm->u.if_kw.cond);
    } else {
      printf("A_stmStm error if_kw %s\n", A_stm_decriptor[stm->kind]);
    }
    if (stm->u.if_kw.then != NULL) {
      destroy_stmList(stm->u.if_kw.then);
    } else {
      printf("A_stmStm error if_kw %s\n", A_stm_decriptor[stm->kind]);
    }
    if (stm->u.if_kw.otherwise != NULL) {
      destroy_stmList(stm->u.if_kw.otherwise);
    }
    debug_printf("destroy_stm A_ifStm\n");
    checked_free(stm);
  } else {
    printf("destroy_stm error\n");
  }
}

void destroy_stmList(A_stmList_p stmList) {
  A_stmList_p next = stmList->tail;
  debug_printf("destroy_stmList entry\n");
  destroy_stm(stmList->head);
  debug_printf("destroy_stmList\n");
  checked_free(stmList);
  if (next != NULL) {
    destroy_stmList(next);
  }
}
