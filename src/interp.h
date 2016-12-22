#ifndef INTERP_H
#define INTERP_H

#include <stdlib.h>

#include "slp.h"
#include "util.h"

/* store value of variables */
typedef struct table *Table_;
struct table {
  string id;
  int value;
  Table_ tail;
};
extern Table_ Table(string id, int value, Table_ tail);

/*
 * Interpreting expressions is more complicated than interpreting statements,
 * because expressions return integer values AND have side effects.  We wish
 * to simulate the straight-line programming language's assignment statements
 * without doing any side effects in the interpreter itself. (The print
 * statements will be accomplished by interpreter side effects, however.)  Part
 * of the solution for this is to use this struct.
 */
typedef struct intAndTable *IntAndTable_;
struct intAndTable {
  int i;
  Table_ t;
};
extern IntAndTable_ IntAndTable(int i, Table_ t);

/*
 * "Interpret" a program in this language.
 */
extern void interp(A_stm stm);
extern void interp_context(A_stm stm);
extern void interp_multi(A_stm *stm);

/*
 * Produce a new table from the specified table.  The new table is just like
 * the original one except that some identifiers map to different integers as
 * a result of the given statement.
 */
extern Table_ interpStm(A_stm s, Table_ t);

/*
 * The result of interpreting an expression e1 with table t1 is an integer
 * value i and a new table t2.  When interpreting an expression with two
 * subexpressions (such as an OpExp), the table t2 resulting from the first
 * subexpression can be used in processing the second subexpression.
 */
extern IntAndTable_ interpExp(A_exp e, Table_ t);
extern IntAndTable_ interpExpList(A_expList expList, Table_ t);

/*
 * Put a new element at the head of the linked list.
 */
extern Table_ update(Table_ t, string id, int value);
extern int lookup(Table_ t, string key);

#endif /* INTERP_H */
