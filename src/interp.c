#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "interp.h"
#include "slp.h"
#include "util.h"

// #define DEBUG
#ifdef DEBUG
#define debug_printf(fmt, args...) printf(fmt, ##args)
#else
#define debug_printf(fmt, args...) /* Don't do anything in release builds */
#endif

void Table_display(table_p t) {
  debug_printf("Table_display\n");
  while (t != NULL) {
    debug_printf("ptr: %x, id: %s, value: %d\n", (unsigned int)t, t->id,
                 t->value);
    t = t->tail;
  }
}

void TableFunction_display(table_function_p t) {
  debug_printf("TableFunction_display\n");
  while (t != NULL) {
    debug_printf("ptr: %x, id: %s, body: %x\n", (unsigned int)t, t->id,
                 (unsigned int)t->body);
    t = t->tail;
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
  return t;
}

/* linked list of function definitions */
table_function_p TableFunction(string id, A_expList_p vars, A_stmList_p body,
                               table_function_p tail) {
  table_function_p t = checked_malloc(sizeof(*t));
  t->id = id;
  t->vars = vars;
  t->body = body;
  t->tail = tail;
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

int lookup_var(table_p t, string key) {
  debug_printf("lookup_var\n");
  while (t != NULL) {
    if (strcmp(t->id, key) == 0) {
      return t->value;
    }
    t = t->tail;
  }

  /* This should not happen! */
  assert(!"Table_t pointer should not be NULL!");
}

table_function_p lookup_function(table_function_p t, string key) {
  debug_printf("lookup_function\n");
  while (t != NULL) {
    debug_printf("lookup_function iter _%s_, _%s_\n", t->id, key);
    if (strcmp(t->id, key) == 0) {
      debug_printf("found\n");
      return t;
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

// construct a new Table on the head
table_function_p update_function(table_function_p t, string id,
                                 A_expList_p vars, A_stmList_p body) {
  debug_printf("update_function\n");
  t = TableFunction(id, vars, body, t);
  TableFunction_display(t);
  return t;
}

IntAndTable_p interpExp(interp_ctx_p context, A_exp_p e) {
  table_p t = context->vars;

  debug_printf("interpExp\n");
  switch (e->kind) {
  case A_idExp:
    debug_printf("A_idExp\n");
    return IntAndTable(lookup_var(t, e->u.id), t);
  case A_numExp:
    debug_printf("A_numExp\n");
    return IntAndTable(e->u.num, t);
  case A_strExp:
    debug_printf("A_strExp\n");
    return StrAndTable(e->u.str, t);
  case A_opExp: {
    debug_printf("A_opExp\n");
    int lval, rval;
    IntAndTable_p it_tmp;

    it_tmp = interpExp(context, e->u.op.left);
    lval = it_tmp->i;
    it_tmp = interpExp(context, e->u.op.right);
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
    debug_printf("A_functionExp\n");
    table_function_p function_p;
    interp_ctx_t _fcontext = {NULL, NULL};
    interp_ctx_p fcontext = &_fcontext;
    /* find the function */
    function_p = lookup_function(context->function, e->u.function.id);
    interpStmList(fcontext, function_p->body);
    /* cleanup context */
    // Table_destroy(ctx);
    return &_it;
  } break;
  default:
    /* This should not happen! */
    assert(!"Wrong kind-value for A_exp_p!");
  }
}

void interpStm(interp_ctx_p context, A_stm_p s) {
  IntAndTable_p it;
  table_p t = context->vars;

  debug_printf("interpStm\n");
  switch (s->kind) {
  case A_assignStm:
    debug_printf("A_assignStm\n");
    it = interpExp(context, s->u.assign.exp);
    t = update(it->t, s->u.assign.id, it->i);
    context->vars = t;
    return;
  case A_printStm:
    debug_printf("A_printStm\n");
    A_expList_p list;
    list = s->u.print.exps;
    while (list != NULL) {
      it = interpExp(context, list->head);
      if (it->str == NULL) {
        printf("%d", it->i);
      } else {
        printf("%s", it->str);
      }
      list = list->tail;
    }
    context->vars = it->t;
    return;
  case A_functionStm:
    debug_printf("A_functionStm\n");
    context->function = update_function(context->function, s->u.function.id,
                                        s->u.function.vars, s->u.function.body);
    return;
  case A_ifStm:
    debug_printf("A_ifStm\n");
    it = interpExp(context, s->u.if_kw.cond);
    if (it->i != 0) {
      interpStmList(context, s->u.if_kw.then);
    } else {
      if (s->u.if_kw.otherwise != NULL) {
        interpStmList(context, s->u.if_kw.otherwise);
      }
    }
    context->vars = it->t;
    return;
  case A_whileStm:
    debug_printf("A_whileStm\n");
    it = interpExp(context, s->u.while_kw.cond);
    while (it->i != 0) {
      interpStmList(context, s->u.while_kw.body);
      it = interpExp(context, s->u.while_kw.cond);
    }
    context->vars = it->t;
    return;
  default:
    /* This should not happen! */
    assert(!"Wrong kind-value for A_stm_p!");
  }

  context->vars = t;
}

void interpStmList(interp_ctx_p context, A_stmList_p stmList) {
  debug_printf("interpStmList\n");
  while (stmList != NULL) {
    interpStm(context, stmList->head);
    /* move to next */
    stmList = stmList->tail;
  }
}

void interp(A_stmList_p stmList) {
  table_p ctx = NULL;
  interp_ctx_t _context = {NULL, NULL};
  interp_ctx_p context = &_context;
  interpStmList(context, stmList);
  /* cleanup context */
  Table_destroy(ctx);
}

static table_p interp_Stm_ctx = NULL;
static A_stmList_p interp_Stm_stmList;
static interp_ctx_t _context = {NULL, NULL};
static interp_ctx_p context = &_context;

void interp_init(A_stmList_p stmList) {
  /* init */
  interp_Stm_stmList = stmList;
}

unsigned int interp_step(void) {
  unsigned int ret = 0;

  if (interp_Stm_stmList != NULL) {
    interpStm(context, interp_Stm_stmList->head);
    /* move to next */
    interp_Stm_stmList = interp_Stm_stmList->tail;
    ret = 1;
  }

  return ret;
}

void interp_destroy(void) {
  /* cleanup context */
  Table_destroy(interp_Stm_ctx);
}
