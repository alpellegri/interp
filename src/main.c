#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "interp.h"

int main(void) {
  A_stm stm;

#if 1
  char code[] =
    "a = 5+(3==2);\n"
    "b = 10*a;\n"
    "print(b);\n";
#else
  char code[] =
    "a = 1;\n"
    "print(a);\n"
    "a = a+1;\n"
    "print(a);\n"
    "a = a+1;\n"
    "print(a);\n"
    "a = a+1;\n"
    "print(a);\n"
    "a = a+1;\n"
    "print(a);\n"
    "a = a+1;\n";
#endif

  printf("load program\n");
  parse_init(code);

  printf("parse program\n");
  stm = parse();

  printf("display program\n");
  display_stm(stm);

  printf("interp program\n");
  // the evaluation function
  interp(stm);

  return 0;
}
