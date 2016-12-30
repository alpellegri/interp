#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "interp.h"
#include "slp.h"
#include "util.h"

void display_Table(Table_ t) {
  Table_ temp = t;
  printf("display_Table\n");
  while (temp != NULL) {
    printf("ptr: %x, id: %s, value: %d\n", (unsigned int)temp, temp->id,
           temp->value);
    temp = temp->tail;
  }
}

// linked list of variables (id,value)
Table_ Table(string id, int value, Table_ tail) {
  Table_ t = checked_malloc(sizeof(*t));
  t->id = id;
  t->value = value;
  t->tail = tail;
  // display_Table(t);
  return t;
}

#if 0
IntAndTable_p IntAndTable(int i, Table_ t) {
  printf("IntAndTable: ptr: %x, value: %d\n", (unsigned int)t, i);
  IntAndTable_p it = checked_malloc(sizeof(*it));
  it->i = i;
  it->t = t;
  return it;
}
#else
intAndTable_t _it = {0, NULL};
IntAndTable_p IntAndTable(int i, Table_ t) {
  _it.i = i;
  _it.t = t;
  return &_it;
}
#endif

int lookup(Table_ t, string key) {
  Table_ temp = t;
  while (temp != NULL) {
    if (strcmp(temp->id, key) == 0) {
      return t->value;
    }
    temp = temp->tail;
  }

  /* This should not happen! */
  assert(!"Table_t pointer should not be NULL!");
}

int remove_old_id(Table_ t, string key) {
  Table_ temp = t;
  while (temp->tail != NULL) {
    if (strcmp(temp->tail->id, key) == 0) {
      Table_ temp2 = temp->tail;
      if (temp->tail->tail != NULL) {
        temp->tail = temp->tail->tail;
      } else {
        temp->tail = NULL;
      }
      checked_free(temp2);
      return 1;
    }
    temp = temp->tail;
  }
  return 0;
}

// construct a new Table on the head
Table_ update(Table_ t, string id, int value) {
  t = Table(id, value, t);
  remove_old_id(t, id);
  // display_Table(t);
  return t;
}

IntAndTable_p interpExp(A_exp_p e, Table_ t) {

  switch (e->kind) {
  case A_idExp:
    return IntAndTable(lookup(t, e->u.id), t);
  case A_numExp:
    return IntAndTable(e->u.num, t);
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
    // printf("A_opExp lvalue: %d, rvalue: %d\n", lval, rval);
    return IntAndTable(value, it_tmp->t);
  }
  default:
    /* This should not happen! */
    assert(!"Wrong kind-value for A_exp_p!");
  }
}

IntAndTable_p interpExpList(A_expList_p expList, Table_ t) {
  IntAndTable_p it;

  switch (expList->kind) {
  case A_expList:
    it = interpExp(expList->head, t);
    // printf("%d\n", it->i);
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

Table_ interpStm(A_stm_p s, Table_ t) {
  IntAndTable_p it;

  switch (s->kind) {
  case A_assignStm:
    it = interpExp(s->u.assign.exp, t);
    t = update(it->t, s->u.assign.id, it->i);
    return t;
  case A_printStm:
    it = interpExpList(s->u.print.exps, t);
    printf("%d", it->i);
    return it->t;
  case A_ifStm:
    it = interpExp(s->u.if_kw.cond, t);
    if (it->i != 0) {
      t = interpStmList(s->u.if_kw.then, t);
    } else {
      if (s->u.if_kw.otherwise != NULL) {
        t = interpStmList(s->u.if_kw.otherwise, t);
      }
    }
    return it->t;
  default:
    /* This should not happen! */
    assert(!"Wrong kind-value for A_stm_p!");
  }

  return t;
}

Table_ interpStmList(A_stmList_p stmList, Table_ ctx) {
  while (stmList != NULL) {
    ctx = interpStm(stmList->head, ctx);
    stmList = stmList->tail;
  }
  return ctx;
}

void interper(A_stmList_p stmList) {
  Table_ ctx = NULL;
  ctx = interpStmList(stmList, ctx);
}
