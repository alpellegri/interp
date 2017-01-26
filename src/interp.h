#ifndef INTERP_H
#define INTERP_H

#include "slp.h"
#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

/* store value of variables */
typedef struct table_s *table_p;
typedef struct table_s {
  string id;
  int value;
  string str;
  table_p tail;
} table_t;
extern table_p Table(string id, int value, table_p tail);

/* store stm entry point of each function definition */
typedef struct table_function_s *table_function_p;
typedef struct table_function_s {
  string id;
  A_expList_p vars;
  A_stmList_p body;
  table_function_p tail;
} table_function_t;
extern table_function_p TableFunction(string id, A_expList_p vars,
                                      A_stmList_p body, table_function_p tail);

/* interp context */
typedef struct interp_ctx_s *interp_ctx_p;
typedef struct interp_ctx_s {
  table_p vars;
  table_function_p function;
} interp_ctx_t;

/*
 * Interpreting expressions is more complicated than interpreting statements,
 * because expressions return integer values AND have side effects.  We wish
 * to simulate the straight-line programming language's assignment statements
 * without doing any side effects in the interpreter itself. (The print
 * statements will be accomplished by interpreter side effects, however.)  Part
 * of the solution for this is to use this struct.
 */
typedef struct intAndTable_s *IntAndTable_p;
typedef struct intAndTable_s {
  int i;
  string str;
  table_p t;
} intAndTable_t;
extern IntAndTable_p IntAndTable(int i, table_p t);

/*
 * Produce a new table from the specified table.  The new table is just like
 * the original one except that some identifiers map to different integers
 * as
 * a result of the given statement.
 */
extern void interpStmList(interp_ctx_p context, A_stmList_p stm);
extern void interpStm(interp_ctx_p context, A_stm_p s);

/*
 * The result of interpreting an expression e1 with table t1 is an integer
 * value i and a new table t2.  When interpreting an expression with two
 * subexpressions (such as an OpExp), the table t2 resulting from the first
 * subexpression can be used in processing the second subexpression.
 */
extern IntAndTable_p interpExp(interp_ctx_p context, A_exp_p e);
extern IntAndTable_p interpExpList(interp_ctx_p context, A_expList_p expList);

/*
 * Put a new element at the head of the linked list.
 */
extern table_p update(table_p t, string id, int value);
extern int lookup(table_p t, string key);

/*
 * "Interpret" a program in this language.
 */
extern void interp(A_stmList_p stmList);

/*
 * "Interpret" a program stm by stm
 */
extern void interp_init(A_stmList_p stmList);
extern unsigned int interp_step(void);
extern void interp_destroy(void);

#ifdef __cplusplus
}
#endif

#endif /* INTERP_H */
