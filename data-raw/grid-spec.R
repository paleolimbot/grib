
library(tidyverse)

ec_guide <- "https://weather.gc.ca/grib/what_is_GRIB_e.html"
noaa_guide <- "https://www.nco.ncep.noaa.gov/pmb/codes/GRIB2/"
grid1_html <- "https://www.wmo.int/pages/prog/www/WDM/Guides/Guide-binary-2.html"
grid2_html <- "https://www.nco.ncep.noaa.gov/pmb/docs/grib2/grib2_doc/"


g2c_lib_src <- "https://www.nco.ncep.noaa.gov/pmb/codes/GRIB2/g2clib-1.6.0.tar"
curl::curl_download(
  "https://www.nco.ncep.noaa.gov/pmb/codes/GRIB2/g2clib-1.6.0.tar",
  "data-raw/g2clib-src.tar"
)
untar("data-raw/g2clib-src.tar", exdir = "data-raw/g2clib-src")

sources <- tibble(
  src = list.files("data-raw/g2clib-src/g2clib-1.6.0", pattern = "\\.(c|h)", full.names = TRUE) %>%
    str_subset("main(home)?.c", negate = TRUE),
  dst = file.path("src", basename(src))
)

stopifnot(all(file.copy(sources$src, sources$dst)))

