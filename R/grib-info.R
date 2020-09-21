
#' Read grib meta information
#'
#' @param path File path to a gridded binary file
#' @param max_length The maximum number of meta records to read. Typically
#'   this will return a single record.
#'
#' @export
#'
grib2_info <- function(path, max_length = 100L) {
  result <- .Call(grib_c_info, normalizePath(path), max_length)
  result[!vapply(result, is.null, logical(1))]
}
