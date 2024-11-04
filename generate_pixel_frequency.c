#include "libpnm.h"

long int *generate_pixel_frequency(
    struct PGM_Image *input_pgm_image,
    int *number_of_non_zero_values_in_the_frequency_array) {
    long int *pixel_frequency = (long int *)calloc(
        input_pgm_image->maxGrayValue + 1, sizeof(*pixel_frequency));

    *number_of_non_zero_values_in_the_frequency_array = 0;

    for (int row = 0; row < input_pgm_image->height; row++) {
        for (int col = 0; col < input_pgm_image->width; col++) {
            int color_value = input_pgm_image->image[row][col];
            pixel_frequency[color_value] += 1;

            if (pixel_frequency[color_value] == 1) {
                *number_of_non_zero_values_in_the_frequency_array += 1;
            }
        }
    }

    printf("pixel frequency:\n");
    for (int i = 0; i < input_pgm_image->maxGrayValue + 1; i++) {
        printf("%ld ", pixel_frequency[i]);
    }
    printf("\n\n");

    return pixel_frequency;
}
