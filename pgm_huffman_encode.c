#include <stdio.h>
#include <stdlib.h>

#include "generate_huffman_nodes.h"
#include "generate_pixel_frequency.h"
#include "huffman_encode_image.h"
#include "libpnm.h"
#include "store_huffman_encoded_data.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Expected 2 arguments. Received %d\n", argc - 1);
        exit(0);
    }

    char *input_image_name = argv[1];
    char *output_decompressed_image_name = argv[2];

    struct PGM_Image input_image;
    load_PGM_Image(&input_image, input_image_name);

    printf("original image:\n");
    for (int i = 0; i < input_image.height; i++) {
        for (int j = 0; j < input_image.width; j++) {
            printf("%d ", input_image.image[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    int number_of_non_zero_values_in_the_frequency_array;
    long int *pixel_frequency = generate_pixel_frequency(
        &input_image, &number_of_non_zero_values_in_the_frequency_array);

    struct node *huffman_nodes = generate_huffman_nodes(
        pixel_frequency, input_image.maxGrayValue,
        number_of_non_zero_values_in_the_frequency_array);

    int number_of_nodes = number_of_non_zero_values_in_the_frequency_array - 1;

    long int length_of_encoded_image_array;
    unsigned char *encoded_image =
        huffman_encode_image(&input_image, huffman_nodes, number_of_nodes,
                             &length_of_encoded_image_array);

    store_huffman_encoded_data(
        output_decompressed_image_name, input_image.width, input_image.height,
        input_image.maxGrayValue, number_of_nodes, huffman_nodes,
        length_of_encoded_image_array, encoded_image);

    free_PGM_Image(&input_image);
    free(pixel_frequency);
    free(huffman_nodes);
    free(encoded_image);
}
