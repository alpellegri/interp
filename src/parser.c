#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "slp.h"
#include "token.h"
#include "util.h"

A_stm_t parseStm(void);
A_exp_t parseExp(void);
A_expList_t parseExpList(void);

char *parse_varname() {
  token_t tok;
  memset(&tok, 0x00, sizeof(token_t));
  token_peek(&tok);
  if (tok.type != token_var) {
    printf("Expecting variable name: %d <-> %d\n", tok.type, token_var);
    token_croak("Expecting variable name");
  }
  return _strdup(tok.value);
}

A_exp_t parse_atom(void) {
  A_exp_t exp;
  token_t tok;

  token_peek(&tok);
  printf("parse_atom: token_peek %s\n", tok.value);
  if (token_is_punc("(") == 1) {
    token_next();
    exp = parseExp();
    token_skip_punc(")");
    return exp;
  } else if (token_is_num() == 1) {
    exp = A_NumExp(atoi(tok.value));
    token_next();
    return exp;
  } else if (token_is_var(&tok) == 1) {
    exp = A_IdExp(_strdup(tok.value));
    token_next();
    return exp;
  }

  printf("error parseExp\n");
  _exit(0);
  return NULL;
}

A_exp_t maybeBinary(A_exp_t left, int prec) {
  A_exp_t right;
  token_t tok;
  token_peek(&tok);
  printf("maybeBinary: token_peek %s\n", tok.value);
  if (token_is_op_tok() == 1) {
    A_binop oper;

    token_next();
    right = maybeBinary(parse_atom(), 0);
    if ((strcmp("+", tok.value) == 0)) {
      oper = A_add;
    } else if ((strcmp("-", tok.value) == 0)) {
      oper = A_sub;
    } else if ((strcmp("*", tok.value) == 0)) {
      oper = A_mul;
    } else if ((strcmp("/", tok.value) == 0)) {
      oper = A_div;
    } else if ((strcmp("==", tok.value) == 0)) {
      oper = A_eq;
    } else if ((strcmp("!=", tok.value) == 0)) {
      oper = A_ne;
    } else if ((strcmp("<=", tok.value) == 0)) {
      oper = A_le;
    } else if ((strcmp("<", tok.value) == 0)) {
      oper = A_lt;
    } else if ((strcmp(">=", tok.value) == 0)) {
      oper = A_ge;
    } else if ((strcmp(">", tok.value) == 0)) {
      oper = A_gt;
    } else {
      printf("error maybeBinary\n");
      _exit(0);
    }
    return A_OpExp(left, oper, right);
  }
  return left;
}

A_exp_t parseExp(void) {
  token_t tok;
  token_peek(&tok);
  printf("parseExp: token_peek %s\n", tok.value);
  return maybeBinary(parse_atom(), 0);
}

A_expList_t parseExpList(void) {
  A_expList_t explist;
  token_t tok;

  token_peek(&tok);
  printf("parseExpList: token_peek %s\n", tok.value);
  explist = A_PairExpList(parseExp(), NULL);
  if (token_is_punc(",") == 1) {
    token_skip_punc(",");
    explist->tail = parseExpList();
    return explist;
  } else if (token_is_punc(")") == 1) {
    return explist;
  }

  printf("error parseExpList\n");
  _exit(0);
  return NULL;
}

A_stm_t parseStm(void) {
  token_t tok;
  A_stm_t stm;

  token_peek(&tok);
  printf("parseStm: token_peek %s\n", tok.value);
  if (token_is_var(&tok) == 1) {
    char *varname = parse_varname();
    token_next();
    token_skip_op("=");
    stm = A_AssignStm(varname, parseExp());
  } else if (token_is_kw("print") == 1) {
    token_skip_kw("print");
    token_skip_punc("(");
    stm = A_PrintStm(parseExpList());
    token_skip_punc(")");
  }
  if (token_is_punc(";")) {
    return stm;
  }

  printf("error parseStm\n");
  _exit(0);
  return NULL;
}

void parse_init(char *ptr) { token_init(ptr); };

A_stm_t parse(void) {
  int i = 0;
  token_t tok;
  A_stm_t code;
  A_stm_t stm = NULL;

  token_peek(&tok);
  printf("[%d]\n", i++);
  stm = A_CompoundStm(NULL, NULL);
  code = stm;
  stm->u.compound.stm1 = parseStm();
  while (!token_eof()) {
    printf("[%d]\n", i++);
    printf("token_skip_punc\n");
    token_skip_punc(";");
    if (!token_eof()) {
      stm->u.compound.stm2 = A_CompoundStm(NULL, NULL);
      stm = stm->u.compound.stm2;
      stm->u.compound.stm1 = parseStm();
    }
  }

  return code;
}
