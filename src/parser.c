#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "slp.h"
#include "token.h"
#include "util.h"

// #define DEBUG
#ifdef DEBUG
#define debug_printf(fmt, args...) printf(fmt, ##args)
#else
#define debug_printf(fmt, args...) /* Don't do anything in release builds */
#endif

A_stmList_p parseStmList(void);
A_stm_p parseStm(void);
A_exp_p parseExp(void);
A_expList_p parseExpList(void);

char *parse_varname() {
  token_t tok;

  token_peek(&tok);
  debug_printf("parse_varname: token_peek %s\n", tok.value);
  if (tok.type != token_var) {
    printf("Expecting variable name: %d <-> %d\n", tok.type, token_var);
    token_croak("Expecting variable name");
  }
  return _strdup(tok.value);
}

A_exp_p parse_atom(void) {
  A_exp_p exp;
  token_t tok;

  token_peek(&tok);
  debug_printf("parse_atom: token_peek %s\n", tok.value);
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
  } else if (token_is_string() == 1) {
    exp = A_StrExp(_strdup(tok.value));
    token_next();
    return exp;
  }

  printf("error parseExp\n");
  _exit(0);
  return NULL;
}

A_exp_p maybeBinary(A_exp_p left, int prec) {
  A_exp_p right;
  token_t tok;
  token_peek(&tok);
  debug_printf("maybeBinary: token_peek %s\n", tok.value);
  if (token_is_op_tok() == 1) {
    A_binop oper;

    debug_printf("maybeBinary: token_is_op_tok\n");
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

A_exp_p parseExp(void) {
  token_t tok;
  token_peek(&tok);
  debug_printf("parseExp: token_peek %s\n", tok.value);
  return maybeBinary(parse_atom(), 0);
}

A_expList_p parseExpList(void) {
  A_expList_p explist;
  token_t tok;

  token_peek(&tok);
  debug_printf("parseExpList: token_peek %s\n", tok.value);
  explist = A_ExpList(parseExp(), NULL);
  if (token_is_punc(",") == 1) {
    token_next();
    explist->tail = parseExpList();
    return explist;
  } else if (token_is_punc(")") == 1) {
    return explist;
  }

  printf("error parseExpList\n");
  _exit(0);
  return NULL;
}

A_stm_p parseStm(void) {
  token_t tok;
  A_stm_p stm;

  token_peek(&tok);
  debug_printf("parseStm: token_peek %s\n", tok.value);
  if (token_is_var(&tok) == 1) {
    char *varname = parse_varname();
    token_next();
    debug_printf("parseStm: =1\n");
    token_skip_op("=");
    debug_printf("parseStm: =2\n");
    stm = A_AssignStm(varname, parseExp());
  } else if (token_is_kw("print") == 1) {
    token_next();
    token_skip_punc("(");
    stm = A_PrintStm(parseExpList());
    token_skip_punc(")");
  } else if (token_is_kw("if") == 1) {
    A_exp_p cond;
    A_stmList_p then;
    A_stmList_p otherwise = NULL;
    token_next();
    token_skip_punc("(");
    cond = parseExp();
    token_skip_punc(")");
    token_skip_punc("{");
    then = parseStmList();
    token_skip_punc("}");
    if (token_is_kw("else") == 1) {
      token_next();
      token_skip_punc("{");
      otherwise = parseStmList();
      token_skip_punc("}");
    }
    stm = A_IfStm(cond, then, otherwise);
  }

  /* end of Stm */
  if (token_is_punc(";") == 1) {
    return stm;
  }

  printf("error parseProg\n");
  _exit(0);
  return NULL;
}

A_stmList_p parseStmList(void) {
  int i = 0;
  int run = 1;
  token_t tok;
  A_stmList_p head;
  A_stmList_p stmList;

  token_peek(&tok);
  debug_printf("\nparseProg [%d]\n", i++);
  stmList = A_StmList(parseStm(), NULL);
  token_skip_punc(";");
  head = stmList;
  if (token_is_punc("}") == 1) {
    debug_printf("parseProg } found\n");
    run = 0;
  }
  if (token_is_eof() == 1) {
    run = 0;
  }

  while (run == 1) {
    debug_printf("\nparseProg [%d]\n", i++);
    stmList->tail = A_StmList(parseStm(), NULL);
    token_skip_punc(";");

    if (token_is_punc("}") == 1) {
      debug_printf("parseProg } found\n");
      run = 0;
    }
    if (token_is_eof() == 1) {
      run = 0;
    }
    stmList = stmList->tail;
  }

  return head;
}

void parse_init(char *ptr) {
  /* init token */
  token_init(ptr);
};

A_stmList_p parse(void) { return parseStmList(); }
