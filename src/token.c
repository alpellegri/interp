#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "slp.h"
#include "token.h"
#include "util.h"

// #define DEBUG
#ifdef DEBUG
#define debug_printf(fmt, args...) printf(fmt, ##args)
#else
#define debug_printf(fmt, args...) /* Don't do anything in release builds */
#endif

char *input_input = NULL;
int input_pos = 0;
int input_line = 1;
int input_col = 0;

// const char keywords[] = " if then else lambda λ true false ";
const char keywords[] = " print if else ";
const char digit[] = "0123456789";
const char id_start[] = "abcdefghikjlmnopqrstuvzwxyλ_";
const char id[] = "?!-<>=0123456789";
const char op_char[] = "+-*/%=&|<>!";
const char punc[] = ",;(){}[]";
const char whitespace[] = " \t\n";

token_t current;

void input_init(char *ptr) { input_input = ptr; };

char input_charAt(int pos) {
  // debug_printf("InputStream->charAt %d\n", pos);
  return input_input[pos];
};

char input_next() {
  // debug_printf("InputStream->next %d\n", pos);
  char ch = input_charAt(input_pos++);
  if (ch == '\n') {
    input_line++, input_col = 0;
  } else {
    input_col++;
  }
  return ch;
};

char input_peek(void) {
  // debug_printf("InputStream->peek %d\n", pos);
  return input_charAt(input_pos);
};

int input_eof(void) {
  // debug_printf("InputStream->eof %d\n", input_pos);
  return input_peek() == '\0';
};

void input_croak(char *str) { printf("croak char: %s \n", str); };

/* tokenizer */
static int is_keyword(char *str) {
  int ret;
  ret = (strstr(keywords, str) != NULL);
  // debug_printf("TokenStream->is_keyword %d\n", ret);
  return ret;
};

static int is_digit(char ch) {
  int ret;
  char str[2] = "\0"; /* gives {\0, \0} */
  str[0] = ch;
  ret = (strstr(digit, str) != NULL);
  // debug_printf("TokenStream->is_digit %d\n", ret);
  return ret;
}

static int is_id_start(char ch) {
  int ret;
  // 1return is_id_start(ch) || "?!-<>=0123456789".indexOf(ch) >= 0;
  char str[2] = "\0"; /* gives {\0, \0} */
  str[0] = ch;
  ret = (strstr(id_start, str) != NULL);
  // debug_printf("TokenStream->is_id_start %d\n", ret);
  return ret;
}

static int is_id(char ch) {
  int ret;
  // 1return is_id_start(ch) || "?!-<>=0123456789".indexOf(ch) >= 0;
  char str[2] = "\0"; /* gives {\0, \0} */
  str[0] = ch;
  ret = (strstr(id, str) != NULL);
  // debug_printf("TokenStream->is_id %d\n", ret);
  return ret;
}

static int is_op_char(char ch) {
  int ret;
  // return "+-*/%=&|<>!".indexOf(ch) >= 0;
  char str[2] = "\0"; /* gives {\0, \0} */
  str[0] = ch;
  ret = (strstr(op_char, str) != NULL);
  // debug_printf("TokenStream->is_op_char %d\n", ret);
  return ret;
}

static int is_punc(char ch) {
  int ret;
  // return ",;(){}[]".indexOf(ch) >= 0;
  char str[2] = "\0"; /* gives {\0, \0} */
  str[0] = ch;
  ret = (strstr(punc, str) != NULL);
  // debug_printf("TokenStream->is_punc %d\n", ret);
  return ret;
}

static int is_whitespace(char ch) {
  int ret;
  char str[2] = "\0"; /* gives {\0, \0} */
  str[0] = ch;
  ret = (strstr(whitespace, str) != NULL);
  // debug_printf("TokenStream->is_whitespace %d\n", ret);
  return ret;
}

static int is_not_eol(char ch) {
  int ret;
  char str[2] = "\0"; /* gives {\0, \0} */
  str[0] = ch;
  ret = (strstr("\n", str) == NULL);
  // debug_printf("TokenStream->is_eol %d\n", ret);
  return ret;
}

static int is_not_eos(char ch) {
  int ret;
  char str[2] = "\0"; /* gives {\0, \0} */
  str[0] = ch;
  ret = (strstr("\"", str) == NULL);
  // debug_printf("TokenStream->is_eos %d\n", ret);
  return ret;
}

void read_while(char *str, int (*predicate)(char ch)) {
  // debug_printf("TokenStream->read_while\n");
  int i = 0;
  while (!input_eof() && predicate(input_peek())) {
    str[i++] = input_next();
  }
  str[i] = '\0'; // append string terminator
}

void read_once(char *str, int (*predicate)(char ch)) {
  // debug_printf("TokenStream->read_while\n");
  int i = 0;
  if (!input_eof() && predicate(input_peek())) {
    str[i++] = input_next();
  }
  str[i] = '\0'; // append string terminator
}

int read_number(token_t *token) {
  read_while(token->value, is_digit);
  // return { type: "num", value: parseFloat(number) };
  token->type = token_num;
  debug_printf("read_number is: _%s_\n", token->value);
  return 1;
}

int read_ident(token_t *token) {
  read_while(token->value, is_id_start);
  // return { type: is_keyword(id) ? "kw" : "var", value: id };
  token->type = is_keyword(token->value) ? (token_kw) : (token_var);
  debug_printf("read_ident is: _%s_\n", token->value);
  return 1;
}

int read_punc(token_t *token) {
  read_once(token->value, is_punc);
  // return { type: is_keyword(id) ? "kw" : "var", value: id };
  token->type = token_punc;
  debug_printf("read_punc is:  _%s_\n", token->value);
  return 1;
}

int read_op_char(token_t *token) {
  read_while(token->value, is_op_char);
  // return { type: is_keyword(id) ? "kw" : "var", value: id };
  token->type = token_op;
  debug_printf("read_op_char is: _%s_\n", token->value);
  return 1;
}

int read_string(token_t *token) {
  read_while(token->value, is_not_eos);
  // return { type: "str", value: read_escaped('"') };
  token->type = token_str;
  debug_printf("read_string is:  _%s_\n", token->value);
  return 1;
}

int read_comment(token_t *token) {
  // read_while(function(ch){ return ch != "\n" });
  read_while(token->value, is_not_eol);
  debug_printf("skip_comment is: _%s_\n", token->value);
  // input.next();
  return 1;
}

int read_next(token_t *token) {
  // debug_printf("TokenStream->read_next\n");
  int ret = 1;
  read_while(token->value, is_whitespace);
  if (input_eof()) {
    ret = 0;
  } else {
    current.unempty = 1;
    char ch = input_peek();
    if (ch == '#') {
      input_next(); // consume first #
      read_comment(token);
      read_next(token);
      return 1;
    }
    if (ch == '\"') {
      input_next(); // consume first "
      read_string(token);
      input_next(); // consume last "
      return 1;
    }
    if (is_digit(ch)) {
      read_number(token);
      return 1;
    }
    if (is_id_start(ch)) {
      read_ident(token);
      return 1;
    }
    if (is_punc(ch)) {
      // return { type: "punc", value: input.next() };
      read_punc(token);
      return 1;
    }
    if (is_op_char(ch)) {
      // return { type: "op", value: read_while(is_op_char)};
      read_op_char(token);
      return 1;
    }
    char str[2] = "\0"; /* gives {\0, \0} */
    str[0] = ch;
    input_croak(str);
    ret = 0;
  }
  return ret;
}

void token_init(char *ptr) { input_init(ptr); };

int token_peek(token_t *token) {
  // return current || (current = read_next());
  if (current.unempty == 0) {
    read_next(&current);
  }
  memcpy(token, &current, sizeof(token_t));
  return 1;
}

int token_next() {
  memset(&current, 0x00, sizeof(token_t));
  read_next(&current);
  return 1;
}

int token_eof(void) {
  debug_printf("token_eof: %d\n", current.unempty);
  return (current.unempty == 0);
}

void token_croak(char *str) {
  input_croak(str);
  _exit(0);
}

int token_is_punc(char *ch) {
  token_t tok;
  int ret = 0;
  token_peek(&tok);
  if ((tok.type == token_punc) && (strcmp(ch, tok.value) == 0)) {
    ret = 1;
  }
  return ret;
}

int token_is_var(token_t *tok) {
  int ret = 0;
  token_peek(tok);
  if (tok->type == token_var) {
    ret = 1;
  }
  return ret;
}

int token_is_kw(char *kw) {
  token_t tok;
  int ret = 0;
  token_peek(&tok);
  if ((tok.type == token_kw) && (strcmp(kw, tok.value) == 0)) {
    ret = 1;
  }
  return ret;
}

int token_is_op(char *op) {
  token_t tok;
  int ret = 0;
  token_peek(&tok);
  if ((tok.type == token_op) && (strcmp(op, tok.value) == 0)) {
    ret = 1;
  }
  return ret;
}

int token_is_op_tok(void) {
  token_t tok;
  int ret = 0;
  token_peek(&tok);
  if (tok.type == token_op) {
    ret = 1;
  }
  return ret;
}

int token_is_num(void) {
  token_t tok;
  int ret = 0;
  token_peek(&tok);
  // return tok && tok.type == "op" && (!op || tok.value == op) && tok;
  if (tok.type == token_num) {
    ret = 1;
  }
  return ret;
}

void token_skip_punc(char *ch) {
  if (token_is_punc(ch)) {
    token_next();
  } else {
    // input.croak("Expecting punctuation: \"" + ch + "\"");
    token_croak("Expecting punctuation:");
  }
}

void token_skip_kw(char *kw) {
  if (token_is_kw(kw)) {
    token_next();
  } else {
    // input.croak("Expecting keyword: \"" + kw + "\"");
    token_croak("Expecting keyword:");
  }
}

void token_skip_op(char *op) {
  if (token_is_op(op)) {
    token_next();
  } else {
    // input.croak("Expecting operator: \"" + op + "\"");
    token_croak("Expecting operator:");
  }
}
