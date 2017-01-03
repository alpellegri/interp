#ifndef UTIL_H
#define UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef char *string;
typedef char bool;

#define TRUE 1
#define FALSE 0

extern char *_strdup(const char *s);
extern void _exit(int v);
extern void *checked_malloc(unsigned int len);
extern void checked_free(void *p);
extern string String(char *);

typedef struct U_boolList_ *U_boolList;

struct U_boolList_ {
  bool head;
  U_boolList tail;
};

extern U_boolList U_BoolList(bool head, U_boolList tail);

#ifdef __cplusplus
}
#endif

#endif /* UTIL_H */
