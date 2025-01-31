#include "libpnm.h"

#ifndef GENERATE_PIXEL_FREQUENCY
#define GENERATE_PIXEL_FREQUENCY

long int *
generate_pixel_frequency(struct PGM_Image *input_pgm_image,
                         int *number_of_non_zero_values_in_the_frequency_array);

#endif
