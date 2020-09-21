
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "grib2.h"
#include <Rinternals.h>

#define INT_FIELD(index_, value_)                              \
  SEXP field##index_ = PROTECT(Rf_allocVector(INTSXP, 1));     \
  INTEGER(field##index_)[0] = value_;                          \
  SET_VECTOR_ELT(lst, index_, field##index_);                  \
  UNPROTECT(1)                                                 \

#define INT_ARRAY_FIELD(index_, value_, len_)                  \
  SEXP field##index_ = PROTECT(Rf_allocVector(INTSXP, len_));  \
  int* pField##index_ = INTEGER(field##index_);                \
  for (g2int i = 0; i < len_; i++) {                           \
    pField##index_[i] = value_[i];                             \
  }                                                            \
  SET_VECTOR_ELT(lst, index_, field##index_);                  \
  UNPROTECT(1)                                                 \

#define REAL_ARRAY_FIELD(index_, value_, len_)                  \
  SEXP field##index_ = PROTECT(Rf_allocVector(REALSXP, len_));  \
  double* pField##index_ = REAL(field##index_);                \
  for (g2int i = 0; i < len_; i++) {                           \
    pField##index_[i] = value_[i];                             \
  }                                                            \
  SET_VECTOR_ELT(lst, index_, field##index_);                  \
  UNPROTECT(1)

#define RAW_ARRAY_FIELD(index_, value_, len_)                  \
  SEXP field##index_ = PROTECT(Rf_allocVector(RAWSXP, len_));  \
  memcpy(RAW(field##index_), value_, len_);                    \
  SET_VECTOR_ELT(lst, index_, field##index_);                  \
  UNPROTECT(1)

SEXP gribfield_to_list(gribfield* gfld) {
  const char* names[] = {
    "version", "discipline",
    "idsectlen", "idsect",
    "locallen", "local",
    "ifldnum",
    "griddef",
    "numoct_opt", "interp_opt",
    "num_opt", "list_opt",
    "igdtnum", "igdtlen", "igdtmpl",
    "ipdtnum", "ipdtlen", "ipdtmpl",
    "num_coord", "coord_list",
    "idrtnum", "idrtlen", "idrtmpl",
    "unpacked", "expanded",
    "ibmp", "ngrdpts", "bmp",
    "ndpts", "fld",
    ""
  };

  SEXP lst = PROTECT(Rf_mkNamed(VECSXP, names));

  INT_FIELD(0, gfld->version);
  INT_FIELD(1, gfld->discipline);
  INT_FIELD(2, gfld->idsectlen);
  INT_ARRAY_FIELD(3, gfld->idsect, gfld->idsectlen);
  INT_FIELD(4, gfld->locallen);
  RAW_ARRAY_FIELD(5, gfld->local, gfld->locallen);
  INT_FIELD(6, gfld->ifldnum);
  INT_FIELD(7, gfld->griddef);
  INT_FIELD(8, gfld->numoct_opt);
  INT_FIELD(9, gfld->interp_opt);
  INT_FIELD(10, gfld->num_opt);
  INT_ARRAY_FIELD(11, gfld->list_opt, gfld->num_opt);
  INT_FIELD(12, gfld->igdtnum);
  INT_FIELD(13, gfld->igdtlen);
  INT_ARRAY_FIELD(14, gfld->igdtmpl, gfld->igdtlen);
  INT_FIELD(15, gfld->ipdtnum);
  INT_FIELD(16, gfld->ipdtlen);
  INT_ARRAY_FIELD(17, gfld->ipdtmpl, gfld->ipdtlen);
  INT_FIELD(18, gfld->num_coord);
  REAL_ARRAY_FIELD(19, gfld->coord_list, gfld->num_coord);
  INT_FIELD(20, gfld->idrtnum);
  INT_FIELD(21, gfld->idrtlen);
  INT_ARRAY_FIELD(22, gfld->idrtmpl, gfld->idrtlen);
  INT_FIELD(23, gfld->unpacked);
  INT_FIELD(24, gfld->expanded);
  INT_FIELD(25, gfld->ibmap);

  INT_FIELD(26, gfld->ngrdpts);
  if (gfld->bmap == NULL) {
    SET_VECTOR_ELT(lst, 27, R_NilValue);
  } else {
    INT_ARRAY_FIELD(27, gfld->bmap, gfld->ngrdpts);
  }

  INT_FIELD(28, gfld->ndpts);
  if (gfld->fld == NULL) {
    SET_VECTOR_ELT(lst, 29, R_NilValue);
  } else {
    REAL_ARRAY_FIELD(29, gfld->fld, gfld->ndpts);
  }

  UNPROTECT(1);
  return lst;
}

SEXP g2_info_to_list(g2int* listsec0, g2int* listsec1, g2int numfields, g2int numlocal) {
  const char* names[] = {"listsec0", "listsec1", "numfields", "numlocal", ""};
  SEXP lst = PROTECT(Rf_mkNamed(VECSXP, names));

  INT_ARRAY_FIELD(0, listsec0, 3);
  INT_ARRAY_FIELD(1, listsec1, 13);
  INT_FIELD(2, numfields);
  INT_FIELD(3, numlocal);

  UNPROTECT(1);
  return lst;
}

SEXP grib_c_g2_info(SEXP path, SEXP maxMessages) {
  const char* fpath = CHAR(STRING_ELT(path, 0));
  int maxMessagesInt = INTEGER(maxMessages)[0];

  SEXP out = PROTECT(Rf_allocVector(VECSXP, maxMessagesInt));
  for (int i = 0; i < maxMessagesInt; i++) {
    SET_VECTOR_ELT(out, i, R_NilValue);
  }

  unsigned char *cgrib;
  g2int listsec0[3], listsec1[13], numlocal, numfields;
  long lskip, lgrib, iseek;
  int ret, ierr;
  FILE *fptr;
  size_t lengrib;

  iseek = 0;
  fptr = fopen(fpath, "r");

  for (int i = 0; i < maxMessagesInt; i++) {
    seekgb(fptr, iseek, 32000, &lskip, &lgrib);
    if (lgrib == 0) {
      break;    // end loop at EOF or problem
    }

    cgrib = (unsigned char *) malloc(lgrib);
    ret = fseek(fptr, lskip, SEEK_SET);
    lengrib = fread(cgrib, sizeof(unsigned char), lgrib, fptr);
    iseek = lskip + lgrib;

    ierr = g2_info(cgrib, listsec0, listsec1, &numfields, &numlocal);
    SET_VECTOR_ELT(out, i, g2_info_to_list(listsec0, listsec1, numfields, numlocal));
    free(cgrib);
  }

  fclose(fptr);

  if (lgrib != 0) {
    Rf_warning(
      "Reached maximum number of messages in grib2_info(). Increase `max_messages` > %d.",
      maxMessagesInt
    );
  }

  UNPROTECT(1);
  return out;
}

SEXP grib_c_g2_field(SEXP path, SEXP whichMessage, SEXP whichField, SEXP expand) {
  const char* fpath = CHAR(STRING_ELT(path, 0));
  int  whichMessageInt = INTEGER(whichMessage)[0];
  int whichFieldInt = INTEGER(whichField)[0];
  int expandInt = LOGICAL(expand)[0];
  int unpackInt = expandInt;

  SEXP out = R_NilValue;

  unsigned char *cgrib;
  g2int listsec0[3], listsec1[13], numlocal, numfields;
  long lskip, n, lgrib, iseek;
  int ret, ierr;
  gribfield *gfld;
  FILE *fptr;
  size_t lengrib;

  iseek = 0;
  fptr = fopen(fpath, "r");
  int nMessage = -1;

  for ( ; ; ) {
    nMessage++;
    seekgb(fptr, iseek, 32000, &lskip, &lgrib);
    if (lgrib == 0) {
      break;    // end loop at EOF or problem
    }

    cgrib = (unsigned char *) malloc(lgrib);
    ret = fseek(fptr, lskip, SEEK_SET);
    iseek = lskip + lgrib;

    if (nMessage != whichMessageInt) {
      continue;
    }

    lengrib = fread(cgrib, sizeof(unsigned char), lgrib, fptr);
    ierr = g2_info(cgrib, listsec0, listsec1, &numfields, &numlocal);

    for (n = 0; n < numfields; n++) {
      if (n != whichFieldInt) {
        continue;
      }

      ierr = g2_getfld(cgrib, n + 1, unpackInt, expandInt, &gfld);

      out = PROTECT(gribfield_to_list(gfld));

      g2_free(gfld);

      if (out != R_NilValue) {
        break;
      }
    }

    free(cgrib);

    if (out != R_NilValue) {
      break;
    }
  }

  fclose(fptr);

  if (out != R_NilValue) {
    UNPROTECT(1);
  }

  return out;
}
