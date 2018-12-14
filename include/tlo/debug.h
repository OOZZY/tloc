#ifndef TLO_DEBUG_H
#define TLO_DEBUG_H

#include <stdio.h>

#define TLO_PRINT_EXPR(_format_specifier, _expr)                       \
  printf("%s:%d: %s: %s: " _format_specifier "\n", __FILE__, __LINE__, \
         __func__, #_expr, (_expr))

#endif  // TLO_DEBUG_H
