/*
 * Each grammar symbol (Stm, Exp, ExpList, Binop) corresponds to a typedef in
 * the data structures.  Each typedef defines a pointer to a corresponding
 * struct.  The struct name, which ends in an underscore, is never used
 * anywhere except in the declaration of the typedef and the definition of the
 * struct itself.
 *
 * Each struct contains a kind field, which is an enum showing different
 * variants, one for each grammar rule; and a u field, which is a union.  Each
 * grammar rule has right-hand-side components that must be represented in the
 * data structures.  Each grammar symbol's struct contains a union to carry
 * these values, and a kind field to indicate which variant of the union is
 * valid.
 *
 * If there is more than one nontrivial (value-carrying) symbol in the
 * right-hand side of a rule (example: the rule CompoundStm), the union will
 * have a component that is itself a struct comprising these values (example:
 * the compound element of the A_stm_s union).
 *
 * If there is only one nontrivial symbol in the right-hand side of a rule, the
 * union will have a component that is the value (example: the num field of the
 * A_exp_p union).
 *
 * For each variant (CompoundStm, AssignStm, etc.) we make a constructor
 * function to malloc and initialize the data structure.  For each grammar
 * rule, there is one constructor that belongs to the union for its
 * left-hand-side symbol.  This constructor function initializes all fields.
 * The malloc function shall never be called directly, except in these
 * constructor functions.
 *
 * For BinOp, we do not make a Binop struct because this would be overkill:
 * none of the variants would carry any data.  Instead, we make an enum type
 * A_binop.
 *
 * Each module (header file) shall have a prefix unique to that module (example
 * A_ in this file).
 *
 * Typedef names (after the prefix) shall start with lowercase letters;
 * constructor functions (after the prefix) with uppercase; enumeration atoms
 * (after the prefix) with lowercase; and union variants (which have no prefix)
 * with lowercase.
 */

#ifndef SLP_H
#define SLP_H

#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  A_add,
  A_sub,
  A_mul,
  A_div,
  A_eq,
  A_ne,
  A_le,
  A_lt,
  A_ge,
  A_gt
} A_binop;

typedef struct A_stm_s *A_stm_p;
typedef struct A_exp_s *A_exp_p;
typedef struct A_expList_s *A_expList_p;

/*
 * Stm -> Stm; Stm       (CompoundStm)
 * Stm -> Stm; epsilon   (CompoundStm)
 * Stm -> id := Exp      (AssignStm)
 * Stm -> print(ExpList) (PrintStm)
 * Stm -> if(Exp)        (IfStm)
 */
struct A_stm_s {
  enum { A_compoundStm, A_assignStm, A_printStm, A_ifStm } kind;
  union {
    struct {
      A_stm_p stm1, stm2;
    } compound;
    struct {
      string id;
      A_exp_p exp;
    } assign;
    struct {
      A_expList_p exps;
    } print;
    struct {
      A_exp_p exp;
    } if_kw;
  } u;
} A_stm_t;
extern A_stm_p A_CompoundStm(A_stm_p stm1, A_stm_p stm2);
extern A_stm_p A_AssignStm(string id, A_exp_p exp);
extern A_stm_p A_PrintStm(A_expList_p exps);

/*
 * Exp -> id             (IdExp)
 * Exp -> num           (NumExp)
 * Exp -> Exp Binop Exp  (OpExp)
 * Exp -> (Stm, Exp)   (EseqExp)
 */
struct A_exp_s {
  enum { A_idExp, A_numExp, A_opExp, A_eseqExp } kind;
  union {
    string id;
    int num;
    struct {
      A_exp_p left;
      A_binop oper;
      A_exp_p right;
    } op;
    struct {
      A_stm_p stm;
      A_exp_p exp;
    } eseq;
  } u;
} A_exp_t;
extern A_exp_p A_IdExp(string id);
extern A_exp_p A_NumExp(int num);
extern A_exp_p A_OpExp(A_exp_p left, A_binop oper, A_exp_p right);
extern A_exp_p A_EseqExp(A_stm_p stm, A_exp_p exp);

/*
 * ExpList -> Exp, ExpList  (PairExpList)
 * ExpList -> Exp  epsilon  (PairExpList)
 */
struct A_expList_s {
  enum { A_pairExpList } kind;
  A_exp_p head;
  A_expList_p tail;
} A_expList_t;
extern A_expList_p A_PairExpList(A_exp_p head, A_expList_p tail);

extern void display_stm(A_stm_p stm);
extern void display_exp(A_exp_p exp);
extern void display_expList(A_expList_p expList);

#ifdef __cplusplus
}
#endif

#endif /* SLP_H */
