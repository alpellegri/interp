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

A_stm_p A_PrintStm(A_expList_p expList) {
  debug_printf("spl create A_PrintStm\n");
  A_stm_p s = checked_malloc(sizeof *s);
  s->kind = A_printStm;
  s->u.print.exps = expList;
  return s;
}

A_stm_p A_FunctionStm(string id, A_expList_p expList, A_stmList_p body) {
  debug_printf("spl create A_FunctionStm\n");
  A_stm_p s = checked_malloc(sizeof *s);
  s->kind = A_functionStm;
  s->u.function.id = id;
  s->u.function.exps = expList;
  s->u.function.body = body;
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

A_stm_p A_WhileStm(A_exp_p cond, A_stmList_p body) {
  debug_printf("spl create A_WhileStm\n");
  A_stm_p s = checked_malloc(sizeof *s);
  s->kind = A_whileStm;
  s->u.while_kw.cond = cond;
  s->u.while_kw.body = body;
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

char *A_expList_decriptor[1] = {"A_expList"};
void A_expList_display(A_expList_p expList) {
  printf("A_expList: %s\n", A_expList_decriptor[expList->kind]);
  if (expList->kind == A_expList) {
    A_exp_display(expList->head);
    if (expList->tail != NULL) {
      A_expList_display(expList->tail);
    } else {
      printf("A_expList: end tail\n");
    }
  } else {
    printf("A_expList error %s\n", A_expList_decriptor[expList->kind]);
  }
}

char *A_exp_decriptor[5] = {
    "A_idExp", "A_numExp", "A_strExp", "A_opExp", "A_eseqExp",
};
char *A_binop_decriptor[10] = {
    "+", "-", "*", "/", "==", "!=", "<=", "<", ">=", ">",
};
void A_exp_display(A_exp_p exp) {
  printf("A_exp: %s\n", A_exp_decriptor[exp->kind]);
  if (exp->kind == A_idExp) {
    printf("_%s_\n", exp->u.id);
  } else if (exp->kind == A_numExp) {
    printf("_%d_\n", exp->u.num);
  } else if (exp->kind == A_strExp) {
    printf("_%s_\n", exp->u.str);
  } else if (exp->kind == A_opExp) {
    A_exp_display(exp->u.op.left);
    printf("_%s_\n", A_binop_decriptor[exp->u.op.oper]);
    A_exp_display(exp->u.op.right);
  } else {
    printf("A_exp error %s\n", A_exp_decriptor[exp->kind]);
  }
}

char *A_stm_decriptor[5] = {
    "A_assignStm", "A_printStm", "A_functionStm", "A_ifStm", "A_whileStm",
};

void A_stm_display(A_stm_p stm) {
  printf("A_stmStm: %s\n", A_stm_decriptor[stm->kind]);
  if (stm->kind == A_assignStm) {
    A_exp_display(stm->u.assign.exp);
  } else if (stm->kind == A_printStm) {
    if (stm->u.print.exps != NULL) {
      A_expList_display(stm->u.print.exps);
    }
  } else if (stm->kind == A_functionStm) {
    printf("_%s_\n", stm->u.function.id);
    if (stm->u.function.exps != NULL) {
      A_expList_display(stm->u.function.exps);
      if (stm->u.function.body != NULL) {
        A_stmList_display(stm->u.function.body);
      } else {
        printf("A_stmStm error function %s\n", A_stm_decriptor[stm->kind]);
      }
    }
  } else if (stm->kind == A_ifStm) {
    if (stm->u.if_kw.cond != NULL) {
      A_exp_display(stm->u.if_kw.cond);
    } else {
      printf("A_stmStm error if_kw %s\n", A_stm_decriptor[stm->kind]);
    }
    if (stm->u.if_kw.then != NULL) {
      A_stmList_display(stm->u.if_kw.then);
    } else {
      printf("A_stmStm error if_kw %s\n", A_stm_decriptor[stm->kind]);
    }
    if (stm->u.if_kw.otherwise != NULL) {
      A_stmList_display(stm->u.if_kw.otherwise);
    }
  } else if (stm->kind == A_whileStm) {
    if (stm->u.while_kw.cond != NULL) {
      A_exp_display(stm->u.while_kw.cond);
    } else {
      printf("A_stmStm error while_kw %s\n", A_stm_decriptor[stm->kind]);
    }
    if (stm->u.while_kw.body != NULL) {
      A_stmList_display(stm->u.while_kw.body);
    } else {
      printf("A_stmStm error while_kw %s\n", A_stm_decriptor[stm->kind]);
    }
  } else {
    printf("A_stmStm error %s\n", A_stm_decriptor[stm->kind]);
  }
}

char *A_stmList_decriptor[1] = {"A_StmList"};
void A_stmList_display(A_stmList_p stmList) {
  printf("A_stmList_display: %s\n", A_stmList_decriptor[stmList->kind]);
  A_stm_display(stmList->head);
  if (stmList->tail != NULL) {
    A_stmList_display(stmList->tail);
  } else {
    printf("A_stmList: end of A_stmList\n");
  }
}

void A_exp_destroyList(A_expList_p expList) {
  debug_printf("A_exp_destroyList entry\n");
  if (expList->kind == A_expList) {
    if (expList->tail != NULL) {
      A_exp_destroyList(expList->tail);
    }
    if (expList->head != NULL) {
      A_exp_destroy(expList->head);
      debug_printf("A_exp_destroyList\n");
      checked_free(expList);
    }
  } else {
    printf("A_exp_destroy error\n");
  }
}

void A_exp_destroy(A_exp_p exp) {
  debug_printf("A_exp_destroy entry\n");
  if (exp->kind == A_idExp) {
    debug_printf("A_exp_destroy A_idExp\n");
    checked_free(exp->u.id);
    checked_free(exp);
  } else if (exp->kind == A_numExp) {
    debug_printf("A_exp_destroy A_numExp\n");
    checked_free(exp);
  } else if (exp->kind == A_strExp) {
    debug_printf("A_exp_destroy A_strExp\n");
    checked_free(exp->u.str);
    checked_free(exp);
  } else if (exp->kind == A_opExp) {
    A_exp_destroy(exp->u.op.left);
    A_exp_destroy(exp->u.op.right);
    debug_printf("A_exp_destroy A_opExp\n");
    checked_free(exp);
  } else {
    printf("A_exp_destroy error\n");
  }
}

void A_stm_destroy(A_stm_p stm) {
  debug_printf("A_stm_destroy entry\n");
  if (stm->kind == A_assignStm) {
    A_exp_destroy(stm->u.assign.exp);
    debug_printf("A_stm_destroy A_assignStm\n");
    checked_free(stm->u.assign.id);
    checked_free(stm);
  } else if (stm->kind == A_printStm) {
    if (stm->u.print.exps != NULL) {
      A_exp_destroyList(stm->u.print.exps);
      debug_printf("A_stm_destroy A_printStm\n");
      checked_free(stm);
    } else {
    }
  } else if (stm->kind == A_functionStm) {
    checked_free(stm->u.function.id);
    if (stm->u.function.exps != NULL) {
      A_exp_destroyList(stm->u.function.exps);
      A_stmList_destroy(stm->u.function.body);
      debug_printf("A_stm_destroy A_functionStm\n");
      checked_free(stm);
    } else {
    }
  } else if (stm->kind == A_ifStm) {
    if (stm->u.if_kw.cond != NULL) {
      A_exp_destroy(stm->u.if_kw.cond);
    } else {
      printf("A_stmStm error if_kw %s\n", A_stm_decriptor[stm->kind]);
    }
    if (stm->u.if_kw.then != NULL) {
      A_stmList_destroy(stm->u.if_kw.then);
    } else {
      printf("A_stmStm error if_kw %s\n", A_stm_decriptor[stm->kind]);
    }
    if (stm->u.if_kw.otherwise != NULL) {
      A_stmList_destroy(stm->u.if_kw.otherwise);
    }
    debug_printf("A_stm_destroy A_ifStm\n");
    checked_free(stm);
  } else if (stm->kind == A_whileStm) {
    if (stm->u.while_kw.cond != NULL) {
      A_exp_destroy(stm->u.while_kw.cond);
    } else {
      printf("A_stmStm error while_kw %s\n", A_stm_decriptor[stm->kind]);
    }
    if (stm->u.while_kw.body != NULL) {
      A_stmList_destroy(stm->u.while_kw.body);
    } else {
      printf("A_stmStm error while_kw %s\n", A_stm_decriptor[stm->kind]);
    }
    debug_printf("A_stm_destroy A_whileStm\n");
    checked_free(stm);
  } else {
    printf("A_stm_destroy error\n");
  }
}

void A_stmList_destroy(A_stmList_p stmList) {
  A_stmList_p next = stmList->tail;
  debug_printf("A_stmList_destroy entry\n");
  A_stm_destroy(stmList->head);
  debug_printf("A_stmList_destroy\n");
  checked_free(stmList);
  if (next != NULL) {
    A_stmList_destroy(next);
  }
}
