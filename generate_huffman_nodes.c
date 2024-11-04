#include "generate_huffman_nodes.h"

#include <math.h>

#include "libpnm.h"

struct node *generate_huffman_nodes(
    long int *pixel_frequency, int max_gray_value,
    int number_of_non_zero_values_in_the_frequency_array) {
    int num_huffman_nodes =
        number_of_non_zero_values_in_the_frequency_array - 1;
    struct node *huffman_nodes =
        malloc(num_huffman_nodes * sizeof(*huffman_nodes));

    long int *frequencies =
        (long int *)calloc(max_gray_value + 1, sizeof(*frequencies));

    for (int i = 0; i < max_gray_value + 1; i++) {
        frequencies[i] = pixel_frequency[i];
    }

    printf("joint pairs (%d pairs):\n", num_huffman_nodes);
    // get joint pairs in order
    for (int i = 0; i < num_huffman_nodes; i++) {
        int min_color = -1;
        long int min_freq = INFINITY;

        int second_min_color = -1;
        long int second_min_freq = INFINITY;

        // find two lowest indexes
        for (int color_value = 0; color_value < max_gray_value + 1;
             color_value++) {
            long int color_freq = frequencies[color_value];
            if (color_freq == 0) {
                continue;
            }

            if (color_freq >= min_freq && color_freq < second_min_freq) {
                second_min_color = color_value;
                second_min_freq = color_freq;
            }

            if (color_freq < min_freq) {
                second_min_color = min_color;
                second_min_freq = min_freq;

                min_color = color_value;
                min_freq = color_freq;
            }
        }

        // find first and second values for node
        int first_color =
            min_color < second_min_color ? min_color : second_min_color;
        int second_color =
            min_color < second_min_color ? second_min_color : min_color;
        printf("(%d, %d) min_color: %d %ld second_min_color: %d %ld\n",
               first_color, second_color, min_color, min_freq, second_min_color,
               second_min_freq);

        huffman_nodes[i] = (struct node){first_color, second_color};

        frequencies[first_color] = min_freq + second_min_freq;
        frequencies[second_color] = 0;
    }
    printf("\n");

    return huffman_nodes;
}
