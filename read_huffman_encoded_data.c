#include <stdio.h>

#include "generate_huffman_nodes.h"
#include "libpnm.h"
#include "store_huffman_encoded_data.h"

void print_binary_byte(unsigned char num) {
    for (int j = 7; j >= 0; j--) {
        if (num & (1 << j)) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf(" ");
}

void print_binary(unsigned char *bytes, long int num_bytes) {
    for (int i = 0; i < num_bytes; i++) {
        print_binary_byte(bytes[i]);
    }
    printf("\n\n");
}

unsigned char *read_huffman_encoded_data(
    char *compressed_file_name_ptr, int *image_width, int *image_height,
    int *max_gray_value, int *number_of_nodes, struct node **huffman_node,
    long int *length_of_encoded_image_array) {
    struct Image_Header header;
    unsigned char *encoded_image =
        malloc(sizeof(*encoded_image) * *length_of_encoded_image_array);

    FILE *compressed_file = fopen(compressed_file_name_ptr, "rb");

    fread(&header, sizeof(header), 1, compressed_file);
    fread(*huffman_node, sizeof(struct node), header.num_huffman_nodes,
          compressed_file);
    fread(encoded_image, sizeof(unsigned char),
          header.length_of_encoded_image_array, compressed_file);
    fclose(compressed_file);

    *image_width = header.image_width;
    *image_height = header.image_height;
    *max_gray_value = header.max_gray_value;
    *number_of_nodes = header.num_huffman_nodes;
    *length_of_encoded_image_array = header.length_of_encoded_image_array;

    printf(
        "image_width=%d, image_height=%d, max_gray_value=%d, "
        "number_of_nodes=%d, length_of_encoded_image_array=%ld\n\n",
        *image_width, *image_height, *max_gray_value, *number_of_nodes,
        *length_of_encoded_image_array);

    /* printf("joint pairs (%d)\n", *number_of_nodes); */
    /* for (int i = 0; i < *number_of_nodes; i++) { */
    /*     struct node current_node = (*huffman_node)[i]; */
    /*     printf("(%d %d)\n", current_node.first_value, */
    /*            current_node.second_value); */
    /* } */
    /* printf("\n"); */

    /* printf("encoded image:\n"); */
    /* print_binary(encoded_image, *length_of_encoded_image_array); */
    /* printf("\n\n"); */

    return encoded_image;
}
