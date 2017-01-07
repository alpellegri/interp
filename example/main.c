#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interp.h"
#include "parser.h"

int main(void) {
  A_stmList_p prog;

  // char code[] = "print(2);\n";

  // char code[] = "print(1);\n"
  //               "print(2);\n"
  //               "print(3);\n";

  // char code[] = "print(\"Hello World!\n\");\n";

  // char code[] = "a = 5;\n";

  // char code[] = "a = 5;\n"
  //               "print(a);\n";

  // char code[] = "a = 1;\n"
  //               "b = 2;\n"
  //               "c = 10;\n"
  //               "a = 3;\n"
  //               "b = 4;\n"
  //               "print(c);\n";

  // char code[] = "a=1;\n"
  //               "b=2;\n"
  //               "a=3;\n"
  //               "print(1,2+1,a);\n";

  // char code[] = "a = 5+(3==3);\n"
  //               "b = 10*a;\n"
  //               "print(b);\n";

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

  // char code[] = "a = 1;\n"
  //               "if (a > 2) {\n"
  //               "  print(1);\n"
  //               "} else {\n"
  //               "  print(2);\n"
  //               "};\n";

  // char code[] = "print(\"Hello World!\n\");\n"
  //               "a = 1;\n"
  //               "if (a > 1) {\n"
  //               "  print(1);\n"
  //               "  print(3);\n"
  //               "} else {\n"
  //               "  print(2);\n"
  //               "  print(4);\n"
  //               "};\n";

  // char code[] = "print(\"Hello World!\n\");\n"
  //               "b = 2;\n"
  //               "a = 1;\n"
  //               "if (a < b) {\n"
  //               "  print(\"then\n\");\n"
  //               "  print(a);\n"
  //               "  print(\"\n\");\n"
  //               "} else {\n"
  //               "  print(\"else\n\");\n"
  //               "  print(b);\n"
  //               "  print(\"\n\");\n"
  //               "};\n"
  //               "print(b);\n"
  //               "print(\"\n\");\n";

  char code[] = "print(\"Hello World!\n\");\n"
                "a = 0;\n"
                "while (a < 5) {\n"
                "  print(\"iteration \");\n"
                "  print(a);\n"
                "  print(\"\n\");\n"
                "  a = a + 1;\n"
                "};\n"
                "print(a);\n"
                "print(\"\n\");\n";

  printf("*\n");
  printf("load program\n");
  printf("*\n");
  parse_init(code);

  printf("*\n");
  printf("parse program\n");
  printf("*\n");
  prog = parse();

  // printf("*\n");
  // printf("display program\n");
  // printf("*\n");
  // parse_display(prog);

  printf("*\n");
  printf("interp program\n");
  printf("*\n");
  interp(prog);

  // printf("*\n");
  // printf("interp_Stm program\n");
  // printf("*\n");
  // interp_Stm_init(prog);
  // while (interp_Stm() == 1)
  //   ;
  // interp_Stm_destroy();

  printf("*\n");
  printf("destroy prog\n");
  printf("*\n");
  parse_destroy(prog);

  return 0;
}
