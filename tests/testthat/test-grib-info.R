
test_that("grid2_info() works", {
  grib2_info("~/Desktop/CMC_hrdps_maritimes_PRATE_SFC_0_ps2.5km_2020091400_P001-00.grib2")
  grib2_field("~/Desktop/CMC_hrdps_maritimes_PRATE_SFC_0_ps2.5km_2020091400_P001-00.grib2", expand = FALSE)
  grib2_field("~/Desktop/CMC_hrdps_maritimes_PRATE_SFC_0_ps2.5km_2020091400_P001-00.grib2", expand = TRUE)
})
