
grib_info <- function(path) {
  .Call(grib_c_info, normalizePath(path), 20L)
}
