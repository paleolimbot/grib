
#' Read grib meta information
#'
#' @param path File path to a gridded binary file
#' @param which_message,which_field Which message/field within the file to read.
#' @param max_messages The number of messages to look for in a file. Typically
#'   there is one message per file.
#' @param expand Use `FALSE` to read meta information; `TRUE` will unpack and
#'   expand the contents of the file into memory.
#'
#' @export
#'
grib2_info <- function(path, max_messages = 100L) {
  result <- .Call(
    grib_c_g2_info,
    normalizePath(path),
    as.integer(max_messages)
  )

  result[!vapply(result, is.null, logical(1))]
}

#' @rdname grib2_info
#' @export
grib2_field <- function(path, which_field = 1L, which_message = 1L, expand = FALSE) {
  result <- .Call(
    grib_c_g2_field,
    normalizePath(path),
    as.integer(which_message) - 1L,
    as.integer(which_field) - 1L,
    as.logical(expand)
  )

  result
}
