
<!-- README.md is generated from README.Rmd. Please edit that file -->

# grib

<!-- badges: start -->

[![Lifecycle:
experimental](https://img.shields.io/badge/lifecycle-experimental-orange.svg)](https://www.tidyverse.org/lifecycle/#experimental)
[![R build
status](https://github.com/paleolimbot/grib/workflows/R-CMD-check/badge.svg)](https://github.com/paleolimbot/grib/actions)
<!-- badges: end -->

The goal of grib is to read GRIB and GRIB2 files. Right now it does
pretty much nothing and is proof-of-concept only.

## Installation

You can install the development version from
[GitHub](https://github.com/) with:

``` r
# install.packages("remotes")
remotes::install_github("paleolimbot/grib")
```

## Example

This is a basic example which shows you how to solve a common problem:

``` r
library(grib)
grib2_info("~/Desktop/CMC_hrdps_maritimes_PRATE_SFC_0_ps2.5km_2020091400_P001-00.grib2")
#> [[1]]
#> [[1]]$listsec0
#> [1]     0     2 22451
#> 
#> [[1]]$listsec1
#>  [1]   54    0    4    0    1 2020    9   14    0    0    0    0    2
#> 
#> [[1]]$numfields
#> [1] 1
#> 
#> [[1]]$numlocal
#> [1] 0
data <- grib2_field(
  "~/Desktop/CMC_hrdps_maritimes_PRATE_SFC_0_ps2.5km_2020091400_P001-00.grib2",
  expand = TRUE
)
summary(data$fld)
#>      Min.   1st Qu.    Median      Mean   3rd Qu.      Max. 
#> 0.000e+00 0.000e+00 0.000e+00 3.755e-05 0.000e+00 6.131e-03
```
