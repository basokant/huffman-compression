#include "store_huffman_encoded_data.h"

#include <stdio.h>

#include "generate_huffman_nodes.h"
#include "libpnm.h"

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

void store_huffman_encoded_data(char *compressed_file_name_ptr, int image_width,
                                int image_height, int max_gray_value,
                                int number_of_nodes, struct node *huffman_node,
                                long int length_of_encoded_image_array,
                                unsigned char *encoded_image) {
    struct Image_Header header = {image_width, image_height, max_gray_value,
                                  number_of_nodes,
                                  length_of_encoded_image_array};

    printf("encoded image (%ld bytes):\n", length_of_encoded_image_array);
    print_binary(encoded_image, length_of_encoded_image_array);

    FILE *compressed_file = fopen(compressed_file_name_ptr, "wb");
    fwrite(&header, sizeof(header), 1, compressed_file);
    fwrite(huffman_node, sizeof(struct node), number_of_nodes, compressed_file);
    fwrite(encoded_image, sizeof(unsigned char), length_of_encoded_image_array,
           compressed_file);
    fclose(compressed_file);
}
