#ifndef PARSER_H
#define PARSER_H

#include "slp.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void parse_init(char *ptr);
extern A_stmList_p parse(void);
extern void parse_destroy(A_stmList_p stmList);
extern void parse_display(A_stmList_p stmList);

#ifdef __cplusplus
}
#endif

#endif /* PARSER_H */
