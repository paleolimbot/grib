
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

SEXP grib_c_g2_info(SEXP path, SEXP maxMessages);
SEXP grib_c_g2_field(SEXP path, SEXP whichMessage, SEXP whichField, SEXP expand);

static const R_CallMethodDef CallEntries[] = {
  {"grib_c_g2_info", (DL_FUNC) &grib_c_g2_info, 2},
  {"grib_c_g2_field", (DL_FUNC) &grib_c_g2_field, 4},
  {NULL, NULL, 0}
};


void R_init_grib(DllInfo *dll) {
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}
