#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interp.h"
#include "parser.h"

int main(void) {
  A_prog_p prog;

  // char code[] = "a=1;\n"
  //               "b=2;\n"
  //               "a=3;\n"
  //               "print(1,2+1,a);\n";

  char code[] = "a = 5+(3==3);\n"
                "b = 10*a;\n"
                "print(b);\n";

  // char code[] = "a = 1;\n"
  //               "print(a);\n"
  //               "a = a+1;\n"
  //               "print(a);\n"
  //               "a = a+1;\n"
  //               "print(a);\n"
  //               "a = a+1;\n"
  //               "print(a);\n"
  //               "a = a+1;\n"
  //               "print(a);\n"
  //               "a = a+1;\n"
  //               "print(a);\n";

  printf("load program\n");
  parse_init(code);

  printf("parse program\n");
  prog = parse();

  printf("display program\n");
  display_stm(prog);

  printf("interp program\n");
  // the evaluation function
  interp(prog);

  return 0;
}
