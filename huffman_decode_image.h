#include "generate_huffman_nodes.h"
#include "libpnm.h"

#ifndef HUFFMAN_DECODE_IMAGE
#define HUFFMAN_DECODE_IMAGE

struct PGM_Image *huffman_decode_image(int image_width, int image_height,
                                       int max_gray_value, int number_of_nodes,
                                       struct node *huffman_node,
                                       long int length_of_encoded_image_array,
                                       unsigned char *encoded_image);

#endif
