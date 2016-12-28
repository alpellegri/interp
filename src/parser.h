#ifndef PARSER_H
#define PARSER_H

#include "slp.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void parse_init(char *ptr);
extern A_stm_t parse(void);

#ifdef __cplusplus
}
#endif

#endif /* PARSER_H */
