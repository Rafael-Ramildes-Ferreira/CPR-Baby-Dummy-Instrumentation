# Using R2U2 on ESP32
The code used is provided in [![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.7889284.svg)](https://doi.org/10.5281/zenodo.7889284). However it is a demostration made to run in Linux. In order to run it in ESP32, some changes had to be done in the source code, such as the following:

### Padding
Paddings ware added to structures to combat differences in memory alignment. These structures are:
- `r2u2_mltl_instruction_t`
- `r2u2_boxq_t`     
- `r2u2_instruction_t`

Beyond adding the paddings in the struct definition it is also necessary to add it when the structrue is created with {**values}
Tests ware only made in past time formulas, some other structures may also need padding when different formulas are used

### Memory bound sizes
Standard monitor implementation is to big for the internal memory of most microcontrollers, so alternatives sizes ware added to implement the monitor and are know used in the library code in place of the old larger values.
These values ware placed as defines in `internals/bounds.h`


----

This is still not a complete list, more changes may be necessary.