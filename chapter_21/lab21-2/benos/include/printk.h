#ifndef __PRINTK__
#define __PRINTK__

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

int printk(const char *fmt, ...);
int vprintk(const char *fmt, va_list args);

#ifdef __cplusplus
}
#endif

#endif
