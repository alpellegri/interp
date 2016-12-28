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
IntAndTable_ IntAndTable(int i, Table_ t) {
  printf("IntAndTable: ptr: %x, value: %d\n", (unsigned int)t, i);
  IntAndTable_ it = checked_malloc(sizeof(*it));
  it->i = i;
  it->t = t;
  return it;
}
#else
struct intAndTable _it = {0, NULL};
IntAndTable_ IntAndTable(int i, Table_ t) {
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

IntAndTable_ interpExp(A_exp_t e, Table_ t) {

  switch (e->kind) {
  case A_idExp:
    return IntAndTable(lookup(t, e->u.id), t);
  case A_numExp:
    return IntAndTable(e->u.num, t);
  case A_opExp: {
    int lval, rval;
    IntAndTable_ it_tmp;

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
      assert(!"Wrong value for A_exp_t->u.op.oper!");
    }
    // printf("A_opExp lvalue: %d, rvalue: %d\n", lval, rval);
    return IntAndTable(value, it_tmp->t);
  }
  case A_eseqExp:
    t = interpStm(e->u.eseq.stm, t);
    return interpExp(e->u.eseq.exp, t);
  default:
    /* This should not happen! */
    assert(!"Wrong kind-value for A_exp_t!");
  }
}

IntAndTable_ interpExpList(A_expList_t expList, Table_ t) {
  IntAndTable_ it;

  switch (expList->kind) {
  case A_pairExpList:
    it = interpExp(expList->head, t);
    // printf("%d\n", it->i);
    if (expList->tail == NULL) {
      return it;
    } else {
      return interpExpList(expList->tail, it->t);
    }
  default:
    /* This should not happen! */
    assert(!"Wrong kind-value for A_expList_t->kind.");
  }
}

Table_ interpStm(A_stm_t s, Table_ t) {
  IntAndTable_ it;

  switch (s->kind) {
  case A_compoundStm:
    t = interpStm(s->u.compound.stm1, t);
    if (s->u.compound.stm2) {
      t = interpStm(s->u.compound.stm2, t);
    } else {
      printf("interpStm end\n");
    }
    return t;
  case A_assignStm:
    it = interpExp(s->u.assign.exp, t);
    t = update(it->t, s->u.assign.id, it->i);
    return t;
  case A_printStm:
    it = interpExpList(s->u.print.exps, t);
    printf("A_printStm %d\n", it->i);
    return it->t;
  case A_ifStm:
    it = interpExp(s->u.if_kw.exp, t);
    return it->t;
  default:
    /* This should not happen! */
    assert(!"Wrong kind-value for A_stm_t!");
  }

  return t;
}

void interp(A_stm_t stm) { interpStm(stm, NULL); }

Table_ ctx = NULL;
void interp_context(A_stm_t stm) { ctx = interpStm(stm, ctx); }
