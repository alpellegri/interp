#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "interp.h"
#include "slp.h"
#include "util.h"

#define DEBUG
#ifdef DEBUG
#define debug_printf(fmt, args...) printf(fmt, ##args)
#else
#define debug_printf(fmt, args...) /* Don't do anything in release builds */
#endif

void Table_display(table_p t) {
  table_p temp = t;
  debug_printf("Table_display\n");
  while (temp != NULL) {
    debug_printf("ptr: %x, id: %s, value: %d\n", (unsigned int)temp, temp->id,
                 temp->value);
    temp = temp->tail;
  }
}

void Table_destroy(table_p t) {
  table_p temp;
  debug_printf("Table_destroy\n");
  while (t != NULL) {
    /* take a copy of tail */
    temp = t->tail;
    debug_printf("ptr: %x, id: %s, value: %d\n", (unsigned int)t, t->id,
                 t->value);
    checked_free(t);
    t = temp;
  }
}

/* linked list of variables (id,value) */
table_p Table(string id, int value, table_p tail) {
  table_p t = checked_malloc(sizeof(*t));
  t->id = id;
  t->value = value;
  t->tail = tail;
  // display_Table(t);
  return t;
}

/* linked list of function definitions */
table_function_p TableFunction(string id, A_expList_p vars, A_stmList_p body, table_function_p tail) {
  table_function_p t = checked_malloc(sizeof(*t));
  t->id = id;
  t->vars = vars;
  t->body = body;
  t->tail = tail;
  // display_TableFunction(t);
  return t;
}

#if 0
IntAndTable_p IntAndTable(int i, table_p t) {
  printf("IntAndTable: ptr: %x, value: %d\n", (unsigned int)t, i);
  IntAndTable_p it = checked_malloc(sizeof(*it));
  it->i = i;
  it->t = t;
  return it;
}
#else
intAndTable_t _it = {0, NULL, NULL};
IntAndTable_p IntAndTable(int i, table_p t) {
  _it.i = i;
  _it.str = NULL;
  _it.t = t;
  return &_it;
}
IntAndTable_p StrAndTable(string str, table_p t) {
  _it.i = 0;
  _it.str = str;
  _it.t = t;
  return &_it;
}
#endif

int lookup(table_p t, string key) {
  while (t != NULL) {
    if (strcmp(t->id, key) == 0) {
      return t->value;
    }
    t = t->tail;
  }

  /* This should not happen! */
  assert(!"Table_t pointer should not be NULL!");
}

int remove_obsolete_id(table_p t, string key) {
  debug_printf("remove_obsolete_id\n");
  table_p next = t->tail;
  while (next != NULL) {
    if (strcmp(next->id, key) == 0) {
      t->tail = next->tail;
      checked_free(next);
      return 1;
    }
    t = t->tail;
    next = t->tail;
  }
  return 0;
}

// construct a new Table on the head
table_p update(table_p t, string id, int value) {
  debug_printf("update\n");
  t = Table(id, value, t);
  remove_obsolete_id(t, id);
  Table_display(t);
  return t;
}

IntAndTable_p interpExp(A_exp_p e, table_p t) {

  switch (e->kind) {
  case A_idExp:
    return IntAndTable(lookup(t, e->u.id), t);
  case A_numExp:
    return IntAndTable(e->u.num, t);
  case A_strExp:
    return StrAndTable(e->u.str, t);
  case A_opExp: {
    int lval, rval;
    IntAndTable_p it_tmp;

    it_tmp = interpExp(e->u.op.left, t);
    lval = it_tmp->i;
    it_tmp = interpExp(e->u.op.right, it_tmp->t);
    rval = it_tmp->i;

    int value;
    switch (e->u.op.oper) {
    case A_add:
      value = lval + rval;
      break;
    case A_sub:
      value = lval - rval;
      break;
    case A_mul:
      value = lval * rval;
      break;
    case A_div:
      value = lval / rval;
      break;
    case A_eq:
      value = lval == rval;
      break;
    case A_ne:
      value = lval != rval;
      break;
    case A_le:
      value = lval <= rval;
      break;
    case A_lt:
      value = lval < rval;
      break;
    case A_ge:
      value = lval >= rval;
      break;
    case A_gt:
      value = lval > rval;
      break;
    default:
      /* This should not happen! */
      assert(!"Wrong value for A_exp_p->u.op.oper!");
    }
    debug_printf("A_opExp lvalue: %d, rvalue: %d\n", lval, rval);
    return IntAndTable(value, it_tmp->t);
  }
  case A_functionExp: {
    return &_it;
  } break;
  default:
    /* This should not happen! */
    assert(!"Wrong kind-value for A_exp_p!");
  }
}

#if 0
IntAndTable_p interpExpList(A_expList_p expList, table_p t) {
  IntAndTable_p it;

  switch (expList->kind) {
  case A_expList:
    it = interpExp(expList->head, t);
    if (expList->tail == NULL) {
      return it;
    } else {
      return interpExpList(expList->tail, it->t);
    }
  default:
    /* This should not happen! */
    assert(!"Wrong kind-value for A_expList_p->kind.");
  }
}
#endif

table_p interpStm(A_stm_p s, table_p t) {
  IntAndTable_p it;

  switch (s->kind) {
  case A_assignStm:
    debug_printf("A_assignStm\n");
    it = interpExp(s->u.assign.exp, t);
    t = update(it->t, s->u.assign.id, it->i);
    return t;
  case A_printStm:
    debug_printf("A_printStm\n");
    A_expList_p list;
    list = s->u.print.exps;
    while (list != NULL) {
      it = interpExp(list->head, t);
      if (it->str == NULL) {
        printf("%d", it->i);
      } else {
        printf("%s", it->str);
      }
      list = list->tail;
    }
    return it->t;
  case A_functionStm:
    debug_printf("A_functionStm\n");
    return t;
  case A_ifStm:
    debug_printf("A_ifStm\n");
    it = interpExp(s->u.if_kw.cond, t);
    if (it->i != 0) {
      t = interpStmList(s->u.if_kw.then, t);
    } else {
      if (s->u.if_kw.otherwise != NULL) {
        t = interpStmList(s->u.if_kw.otherwise, t);
      }
    }
    return it->t;
  case A_whileStm:
    debug_printf("A_whileStm\n");
    it = interpExp(s->u.while_kw.cond, t);
    while (it->i != 0) {
      t = interpStmList(s->u.while_kw.body, t);
      it = interpExp(s->u.while_kw.cond, t);
    }
    return it->t;
  default:
    /* This should not happen! */
    assert(!"Wrong kind-value for A_stm_p!");
  }

  return t;
}

table_p interpStmList(A_stmList_p stmList, table_p ctx) {
  while (stmList != NULL) {
    ctx = interpStm(stmList->head, ctx);
    /* move to next */
    stmList = stmList->tail;
  }
  return ctx;
}

void interp(A_stmList_p stmList) {
  table_p ctx = NULL;
  ctx = interpStmList(stmList, ctx);
  /* cleanup context */
  Table_destroy(ctx);
}

static table_p interp_Stm_ctx = NULL;
static A_stmList_p interp_Stm_stmList;

void interp_Stm_init(A_stmList_p stmList) {
  /* init */
  interp_Stm_stmList = stmList;
}

unsigned int interp_Stm(void) {
  unsigned int ret = 0;

  if (interp_Stm_stmList != NULL) {
    interp_Stm_ctx = interpStm(interp_Stm_stmList->head, interp_Stm_ctx);
    /* move to next */
    interp_Stm_stmList = interp_Stm_stmList->tail;
    ret = 1;
  }

  return ret;
}

void interp_Stm_destroy(void) {
  /* cleanup context */
  Table_destroy(interp_Stm_ctx);
}
