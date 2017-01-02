/*
 * Commonly used utility functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

// #define DEBUG
#ifdef DEBUG
#define debug_printf(fmt, args...) printf(fmt, ##args)
#else
#define debug_printf(fmt, args...) /* Don't do anything in release builds */
#endif

/* helper function */
char *_strdup(const char *s) {
  char *d = checked_malloc(strlen(s) + 1); // Space for length plus nul
  if (d == NULL) {
    return NULL; // No memory
  }
  strcpy(d, s);  // Copy the characters
  return d;      // Return the new string
}

/* helper function */
void _exit(int v) { exit(v); }

void *checked_malloc(int len) {
  void *p = calloc(len, 1);
  if (!p) {
    printf("Ran out of memory!\n");
    _exit(1);
  } else {
    debug_printf("malloc ptr %x, size: %d\n", (unsigned int)p, len);
  }
  return p;
}

void checked_free(void *p) {
  if (p == 0) {
    printf("checked_free error\n");
    _exit(1);
  } else {
    debug_printf("free ptr %x\n", (unsigned int)p);
    free(p);
  }
}

string String(char *s) {
  string p = checked_malloc(strlen(s) + 1);
  strcpy(p, s);
  return p;
}

U_boolList U_BoolList(bool head, U_boolList tail) {
  U_boolList list = checked_malloc(sizeof(*list));
  list->head = head;
  list->tail = tail;
  return list;
}
