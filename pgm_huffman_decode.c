#include <stdio.h>
#include <stdlib.h>

#include "generate_huffman_nodes.h"
#include "huffman_decode_image.h"
#include "libpnm.h"
#include "read_huffman_encoded_data.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Expected 2 arguments. Received %d\n", argc - 1);
        exit(0);
    }

    char *input_image_name = argv[1];
    char *output_decoded_image_name = argv[2];

    int image_width, image_height, max_gray_value, number_of_nodes;
    struct node *huffman_node;
    long length_of_encoded_image_array;

    unsigned char *encoded_image = read_huffman_encoded_data(
        input_image_name, &image_width, &image_height, &max_gray_value,
        &number_of_nodes, &huffman_node, &length_of_encoded_image_array);

    struct PGM_Image *decoded_image = huffman_decode_image(
        image_width, image_height, max_gray_value, number_of_nodes,
        huffman_node, length_of_encoded_image_array, encoded_image);

    printf("%d %d", decoded_image->height, decoded_image->width);

    save_PGM_Image(decoded_image, output_decoded_image_name, false);
    free(encoded_image);
}
