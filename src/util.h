#ifndef UTIL_H
#define UTIL_H

#include <assert.h>

typedef char *string;
typedef char bool;

#define TRUE 1
#define FALSE 0

extern void *checked_malloc(int len);
extern void checked_free(void *p);
extern string String(char *);

typedef struct U_boolList_ *U_boolList;

struct U_boolList_ {
  bool head;
  U_boolList tail;
};

extern U_boolList U_BoolList(bool head, U_boolList tail);

#endif /* UTIL_H */
