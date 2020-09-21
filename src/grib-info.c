
#include <stdio.h>
#include <stdlib.h>
#include "grib2.h"
#include <Rinternals.h>

// Copied from grib.h:
//   gribfield gfld;
//
//        gfld->version = GRIB edition number ( currently 2 )
//        gfld->discipline = Message Discipline ( see Code Table 0.0 )
//        gfld->idsect = Contains the entries in the Identification
//                        Section ( Section 1 )
//                        This element is a pointer to an array
//                        that holds the data.
//            gfld->idsect[0]  = Identification of originating Centre
//                                    ( see Common Code Table C-1 )
//                             7 - US National Weather Service
//            gfld->idsect[1]  = Identification of originating Sub-centre
//            gfld->idsect[2]  = GRIB Master Tables Version Number
//                                    ( see Code Table 1.0 )
//                             0 - Experimental
//                             1 - Initial operational version number
//            gfld->idsect[3]  = GRIB Local Tables Version Number
//                                    ( see Code Table 1.1 )
//                             0     - Local tables not used
//                             1-254 - Number of local tables version used
//            gfld->idsect[4]  = Significance of Reference Time (Code Table 1.2)
//                             0 - Analysis
//                             1 - Start of forecast
//                             2 - Verifying time of forecast
//                             3 - Observation time
//            gfld->idsect[5]  = Year ( 4 digits )
//            gfld->idsect[6]  = Month
//            gfld->idsect[7)  = Day
//            gfld->idsect[8]  = Hour
//            gfld->idsect[9]  = Minute
//            gfld->idsect[10]  = Second
//            gfld->idsect[11]  = Production status of processed data
//                                    ( see Code Table 1.3 )
//                              0 - Operational products
//                              1 - Operational test products
//                              2 - Research products
//                              3 - Re-analysis products
//            gfld->idsect[12]  = Type of processed data ( see Code Table 1.4 )
//                              0  - Analysis products
//                              1  - Forecast products
//                              2  - Analysis and forecast products
//                              3  - Control forecast products
//                              4  - Perturbed forecast products
//                              5  - Control and perturbed forecast products
//                              6  - Processed satellite observations
//                              7  - Processed radar observations
//        gfld->idsectlen = Number of elements in gfld->idsect[].
//        gfld->local   = Pointer to character array containing contents
//                       of Local Section 2, if included
//        gfld->locallen = length of array gfld->local[]
//        gfld->ifldnum = field number within GRIB message
//        gfld->griddef = Source of grid definition (see Code Table 3.0)
//                      0 - Specified in Code table 3.1
//                      1 - Predetermined grid Defined by originating centre
//        gfld->ngrdpts = Number of grid points in the defined grid.
//        gfld->numoct_opt = Number of octets needed for each
//                          additional grid points definition.
//                          Used to define number of
//                          points in each row ( or column ) for
//                          non-regular grids.
//                          = 0, if using regular grid.
//        gfld->interp_opt = Interpretation of list for optional points
//                          definition.  (Code Table 3.11)
//        gfld->igdtnum = Grid Definition Template Number (Code Table 3.1)
//        gfld->igdtmpl  = Contains the data values for the specified Grid
//                         Definition Template ( NN=gfld->igdtnum ).  Each
//                         element of this integer array contains an entry (in
//                         the order specified) of Grid Defintion Template 3.NN
//                         This element is a pointer to an array
//                         that holds the data.
//        gfld->igdtlen = Number of elements in gfld->igdtmpl[].  i.e. number of
//                       entries in Grid Defintion Template 3.NN
//                       ( NN=gfld->igdtnum ).
//        gfld->list_opt  = (Used if gfld->numoct_opt .ne. 0)  This array
//                          contains the number of grid points contained in
//                          each row ( or column ).  (part of Section 3)
//                          This element is a pointer to an array
//                          that holds the data.  This pointer is nullified
//                          if gfld->numoct_opt=0.
//        gfld->num_opt = (Used if gfld->numoct_opt .ne. 0)  The number of entries
//                       in array ideflist.  i.e. number of rows ( or columns )
//                       for which optional grid points are defined.  This value
//                       is set to zero, if gfld->numoct_opt=0.
//        gfdl->ipdtnum = Product Definition Template Number (see Code Table 4.0)
//        gfld->ipdtmpl  = Contains the data values for the specified Product
//                         Definition Template ( N=gfdl->ipdtnum ).  Each element
//                         of this integer array contains an entry (in the
//                         order specified) of Product Defintion Template 4.N.
//                         This element is a pointer to an array
//                         that holds the data.
//        gfld->ipdtlen = Number of elements in gfld->ipdtmpl[].  i.e. number of
//                       entries in Product Defintion Template 4.N
//                       ( N=gfdl->ipdtnum ).
//        gfld->coord_list  = Real array containing floating point values
//                            intended to document the vertical discretisation
//                            associated to model data on hybrid coordinate
//                            vertical levels.  (part of Section 4)
//                            This element is a pointer to an array
//                            that holds the data.
//        gfld->num_coord = number of values in array gfld->coord_list[].
//        gfld->ndpts = Number of data points unpacked and returned.
//        gfld->idrtnum = Data Representation Template Number
//                       ( see Code Table 5.0)
//        gfld->idrtmpl  = Contains the data values for the specified Data
//                         Representation Template ( N=gfld->idrtnum ).  Each
//                         element of this integer array contains an entry
//                         (in the order specified) of Product Defintion
//                         Template 5.N.
//                         This element is a pointer to an array
//                         that holds the data.
//        gfld->idrtlen = Number of elements in gfld->idrtmpl[].  i.e. number
//                       of entries in Data Representation Template 5.N
//                       ( N=gfld->idrtnum ).
//        gfld->unpacked = logical value indicating whether the bitmap and
//                        data values were unpacked.  If false,
//                        gfld->bmap and gfld->fld pointers are nullified.
//        gfld->expanded = Logical value indicating whether the data field
//                         was expanded to the grid in the case where a
//                         bit-map is present.  If true, the data points in
//                         gfld->fld match the grid points and zeros were
//                         inserted at grid points where data was bit-mapped
//                         out.  If false, the data values in gfld->fld were
//                         not expanded to the grid and are just a consecutive
//                         array of data points corresponding to each value of
//                         "1" in gfld->bmap.
//        gfld->ibmap = Bitmap indicator ( see Code Table 6.0 )
//                     0 = bitmap applies and is included in Section 6.
//                     1-253 = Predefined bitmap applies
//                     254 = Previously defined bitmap applies to this field
//                     255 = Bit map does not apply to this product.
//        gfld->bmap  = integer array containing decoded bitmap,
//                      if gfld->ibmap=0 or gfld->ibap=254.  Otherwise nullified.
//                      This element is a pointer to an array
//                      that holds the data.
//        gfld->fld  = Array of gfld->ndpts unpacked data points.
//                     This element is a pointer to an array
//                     that holds the data.

#define INT_FIELD(index_, value_)                              \
  SEXP field##index_ = PROTECT(Rf_allocVector(INTSXP, 1));     \
  INTEGER(field##index_)[0] = value_;                          \
  SET_VECTOR_ELT(lst, index_, field##index_);                  \
  UNPROTECT(1)

SEXP gribfield_to_list(gribfield* gfld) {
  const char* names[] = {
    "version",
    // "discipline", "idsect", "local", "ifldnum", "griddef",
    // "ngrdopts", "numoct_opt", "interp_opt",
    ""
  };

  SEXP lst = PROTECT(Rf_mkNamed(VECSXP, names));
  INT_FIELD(0, gfld->version);

  UNPROTECT(1);
  return lst;
}

SEXP grib_c_info(SEXP path, SEXP maxLength) {
  // extract file path as buffer and allocate output
  const char* fpath = CHAR(STRING_ELT(path, 0));

  int maxLengthInt = INTEGER(maxLength)[0];
  SEXP out = PROTECT(Rf_allocVector(VECSXP, maxLengthInt));
  for (int i = 0; i < maxLengthInt; i++) {
    SET_VECTOR_ELT(out, i, R_NilValue);
  }

  unsigned char *cgrib;
  g2int listsec0[3], listsec1[13], numlocal, numfields;
  long lskip, n, lgrib, iseek;
  int unpack, expand, ret, ierr;
  gribfield *gfld;
  FILE *fptr;
  size_t lengrib;

  iseek = 0;
  unpack = 1;
  expand = 1;

  fptr = fopen(fpath, "r");

  for (int i = 0; i < maxLengthInt; i++) {
    seekgb(fptr, iseek, 32000, &lskip, &lgrib);
    if (lgrib == 0) {
      break;    // end loop at EOF or problem
    }

    cgrib = (unsigned char *) malloc(lgrib);
    ret = fseek(fptr,lskip,SEEK_SET);
    lengrib = fread(cgrib,sizeof(unsigned char),lgrib,fptr);
    iseek = lskip + lgrib;
    ierr = g2_info(cgrib, listsec0, listsec1, &numfields, &numlocal);
    for (n = 0; n < numfields; n++) {
      ierr = g2_getfld(cgrib, n+1, unpack, expand, &gfld);

      SEXP item = PROTECT(gribfield_to_list(gfld));
      SET_VECTOR_ELT(out, i, item);
      UNPROTECT(1);

      g2_free(gfld);
    }

    free(cgrib);
  }

  UNPROTECT(1);
  return out;
}
