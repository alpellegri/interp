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

typedef struct node_s *node_p;
struct node_s {
  unsigned int key;
  unsigned int data;
  node_p next;
} node_t;

node_p node_head = NULL;
node_p node_curr = NULL;

// display the list
void display_nodes() {
  node_p ptr = node_head;
  unsigned int mem = 0;

  // printf("display_nodes:\n");
  // start from the beginning
  while (ptr != NULL) {
    mem += ptr->data;
    // printf("(%d,%d)\n", ptr->key, ptr->data);
    ptr = ptr->next;
  }
  printf("display_nodes memory: %d\n", mem);
}

// insert link at the first location
void mk_node(unsigned int key, unsigned int data) {
  // create a link
  node_p link = (node_p)malloc(sizeof(node_t));
  link->key = key;
  link->data = data;
  // point it to old first node
  link->next = node_head;
  // point first to new first node
  node_head = link;
}

// remove a link with given key
node_p rm_node(unsigned int key) {
  // start from the first link
  node_p curr = node_head;
  node_p prev = NULL;
  // if list is empty
  if (node_head == NULL) {
    return NULL;
  }
  // navigate through list
  while (curr->key != key) {
    // if it is last node
    if (curr->next == NULL) {
      return NULL;
    } else {
      // store reference to curr link
      prev = curr;
      // move to next link
      curr = curr->next;
    }
  }
  // found a match, update the link
  if (curr == node_head) {
    // change first to point to next link
    node_head = node_head->next;
  } else {
    // bypass the curr link
    prev->next = curr->next;
  }
  return curr;
}

/* helper function */
char *_strdup(const char *s) {
  char *d = checked_malloc(strlen(s) + 1); // Space for length plus nul
  if (d == NULL) {
    return NULL; // No memory
  }
  strcpy(d, s); // Copy the characters
  return d;     // Return the new string
}

/* helper function */
void _exit(int v) { exit(v); }

void *checked_malloc(unsigned int len) {
  void *p = calloc(len, 1);
  if (!p) {
    printf("Ran out of memory!\n");
    _exit(1);
  } else {
    // debug_printf("malloc ptr %x, size: %d\n", (unsigned int)p, len);
    mk_node((unsigned int)p, len);
    display_nodes();
  }
  return p;
}

void checked_free(void *p) {
  if (p == 0) {
    printf("checked_free error\n");
    _exit(1);
  } else {
    node_p node;
    // debug_printf("free ptr %x\n", (unsigned int)p);
    node = rm_node((unsigned int)p);
    free(node);
    free(p);
    display_nodes();
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
