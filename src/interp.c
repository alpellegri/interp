#include <stdio.h>
#include <string.h>

#include "interp.h"
#include "slp.h"
#include "util.h"

Table_ Table(string id, int value, struct table *tail) {
  Table_ t = checked_malloc(sizeof(*t));
  t->id = id;
  t->value = value;
  t->tail = tail;
  return t;
}

IntAndTable_ IntAndTable(int i, Table_ t) {
  IntAndTable_ it = checked_malloc(sizeof(*it));
  it->i = i;
  it->t = t;
  return it;
}

Table_ update(Table_ t, string id, int value) {
  printf("update: id: %s, value: %d\n", id, value);
  return Table(id, value, t);
}

int lookup(Table_ t, string key) {
  // printf("lookup ptr: %x, key: %s\n", t, key);
  Table_ temp = t;
  while (temp != NULL) {
    // printf("lookup while: id: %s, key: %s\n", temp->id, key);
    // if (temp->id == key) {
    if (strcmp(temp->id, key) == 0) {
      return t->value;
    }
    temp = temp->tail;
  }

  /* This should not happen! */
  assert(!"Table_t pointer should not be NULL!");
}

IntAndTable_ interpExp(A_exp e, Table_ t) {

  switch (e->kind) {
  case A_idExp:
    // printf("A_idExp: %s\n", e->u.id);
    return IntAndTable(lookup(t, e->u.id), t);
  case A_numExp:
    // printf("A_numExp: %d\n", e->u.num);
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
      assert(!"Wrong value for A_exp->u.op.oper!");
    }
    // printf("A_opExp lvalue: %d, rvalue: %d\n", lval, rval);
    return IntAndTable(value, it_tmp->t);
  }
  case A_eseqExp:
    t = interpStm(e->u.eseq.stm, t);
    return interpExp(e->u.eseq.exp, t);
  default:
    /* This should not happen! */
    assert(!"Wrong kind-value for A_exp!");
  }
}

IntAndTable_ interpExpList(A_expList expList, Table_ t) {
  IntAndTable_ it;

  switch (expList->kind) {
  case A_pairExpList:
    it = interpExp(expList->head, t);
    // printf("%d ", it->i);
    if (expList->tail == NULL) {
      return it;
    } else {
      return interpExpList(expList->tail, it->t);
    }
  default:
    /* This should not happen! */
    assert(!"Wrong kind-value for A_expList->kind.");
  }
}

Table_ interpStm(A_stm s, Table_ t) {
  IntAndTable_ it;

  switch (s->kind) {
  case A_compoundStm:
    t = interpStm(s->u.compound.stm1, t);
    if (s->u.compound.stm2) {
      t = interpStm(s->u.compound.stm2, t);
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
  default:
    /* This should not happen! */
    assert(!"Wrong kind-value for A_stm!");
  }

  return t;
}

void interp(A_stm stm) { interpStm(stm, NULL); }

Table_ ctx = NULL;
void interp_context(A_stm stm) { ctx = interpStm(stm, ctx); }
