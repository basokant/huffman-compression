#include "generate_huffman_nodes.h"
#include "libpnm.h"

#ifndef STORE_HUFFMAN_ENCODED_DATA
#define STORE_HUFFMAN_ENCODED_DATA

struct Image_Header {
    int image_width;
    int image_height;
    int max_gray_value;
    int num_huffman_nodes;
    long int length_of_encoded_image_array;
};

void store_huffman_encoded_data(char *compressed_file_name_ptr, int image_width,
                                int image_height, int max_gray_value,
                                int number_of_nodes, struct node *huffman_node,
                                long int length_of_encoded_image_array,
                                unsigned char *encoded_image);

#endif
