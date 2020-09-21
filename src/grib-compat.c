
#include "grib-compat.h"
#include <stdarg.h>
#include <Rinternals.h>

void grib_compat_abort() {
  Rf_error("Abort called");
}

void grib_compat_printf(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  Rprintf(fmt, args);
  va_end(args);
}

void grib_compat_eprintf(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  REprintf(fmt, args);
  va_end(args);
}
