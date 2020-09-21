
#ifndef GRIB_COMPAT_H
#define GRIB_COMPAT_H

__attribute__((noreturn)) void grib_compat_abort();
void grib_compat_printf(const char* fmt, ...);
void grib_compat_eprintf(const char* fmt, ...);

#endif
