#ifndef TOKEN_H
#define TOKEN_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  token_punc = 1,
  token_num,
  token_str,
  token_kw,
  token_var,
  token_op,
} token_type_t;

typedef char token_value_t[50];

typedef struct token_s {
  int unempty;
  token_type_t type;
  token_value_t value;
} token_t;

extern void token_init(char *ptr);
extern void token_peek(token_t *token);
extern void token_next();
extern int token_eof(void);
extern void token_croak(char *str);
extern int token_is_punc(char *ch);
extern int token_is_var(token_t *tok);
extern int token_is_kw(char *kw);
extern int token_is_op(char *op);
extern int token_is_op_tok(void);
extern int token_is_num(void);
extern void token_skip_punc(char *ch);
extern void token_skip_kw(char *kw);
extern void token_skip_op(char *op);

#ifdef __cplusplus
}
#endif

#endif /* TOKEN_H */
