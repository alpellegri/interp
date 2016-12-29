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

  // char code[] = "print(1);\n"
  //               "print(2);\n";

  // char code[] = "a = 1;\n"
  //               "if (a < 2) {\n"
  //               "  print(1);\n"
  //               "} else {\n"
  //               "  print(2);\n"
  //               "};\n";

  // char code[] = "a = 1;\n"
  //               "if (a < 2) {\n"
  //               "  print(1);\n"
  //               "  print(3);\n"
  //               "} else {\n"
  //               "  print(2);\n"
  //               "  print(4);\n"
  //               "};\n";

  printf("*\n");
  printf("load program\n");
  printf("*\n");
  parse_init(code);

  printf("*\n");
  printf("parse program\n");
  printf("*\n");
  prog = parse();

  printf("*\n");
  printf("display program\n");
  printf("*\n");
  display_stm(prog);

  printf("*\n");
  printf("interp program\n");
  printf("*\n");
  // the evaluation function
  interp(prog);

  return 0;
}
