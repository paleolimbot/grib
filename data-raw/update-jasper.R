
library(tidyverse)

curl::curl_download(
  "https://www.ece.uvic.ca/~frodo/jasper/software/jasper-2.0.14.tar.gz",
  "data-raw/jasper-src.tar.gz"
)

untar("data-raw/jasper-src.tar.gz", exdir = "data-raw/jasper-src")

sources <- tibble(
  src = list.files("src", pattern = ".c$", full.names = TRUE),
  obj = basename(src) %>% str_replace("\\.c", ".o")
)

list.files("src", pattern = ".c$", recursive = TRUE) %>%
  str_replace("\\.c$", ".o") %>%
  paste0(collapse = "\\\n    ") %>%
  paste0("OBJECTS = ", .) %>%
  clipr::write_clip()
