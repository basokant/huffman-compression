#include <stdint.h>

#include "generate_huffman_nodes.h"
#include "libpnm.h"

struct tree_node {
    int left;
    int value;
    int right;
};

#define INTERNAL -1

struct PGM_Image *huffman_decode_image(int image_width, int image_height,
                                       int max_gray_value, int number_of_nodes,
                                       struct node *huffman_node,
                                       long int length_of_encoded_image_array,
                                       unsigned char *encoded_image) {
    size_t huffman_tree_length = number_of_nodes * 2 + 1;
    struct tree_node huffman_tree[huffman_tree_length];
    int leaves[max_gray_value + 1];

    for (int i = 0; i < max_gray_value + 1; i++) {
        leaves[i] = 0;
    }

    struct tree_node root = {-1, INTERNAL, -1};
    huffman_tree[0] = root;

    for (int i = number_of_nodes - 1; i >= 0; i--) {
        /* printf("i=%d\n", i); */
        struct node current_node = huffman_node[i];

        struct tree_node left_node = {-1, current_node.first_value, -1};
        int left_node_idx = (number_of_nodes - i - 1) * 2 + 1;
        huffman_tree[left_node_idx] = left_node;

        /* printf("left_node: idx:%d | %d %d %d  ", left_node_idx,
         * left_node.left, */
        /*        left_node.value, left_node.right); */

        struct tree_node right_node = {-1, current_node.second_value, -1};
        int right_node_idx = (number_of_nodes - i - 1) * 2 + 2;
        huffman_tree[right_node_idx] = right_node;

        /* printf("right_node: idx:%d | %d %d %d  ", right_node_idx, */
        /*        right_node.left, right_node.value, right_node.right); */

        int prev_left_idx = leaves[current_node.first_value];
        if (prev_left_idx >= 0) {
            huffman_tree[prev_left_idx].left = left_node_idx;
            huffman_tree[prev_left_idx].value = INTERNAL;
            huffman_tree[prev_left_idx].right = right_node_idx;

            /* printf("prev_left: idx:%d | %d %d %d  ", prev_left_idx, */
            /*        huffman_tree[prev_left_idx].left, */
            /*        huffman_tree[prev_left_idx].value, */
            /*        huffman_tree[prev_left_idx].right); */
        }
        /* printf("\n"); */

        leaves[current_node.first_value] = left_node_idx;
        leaves[current_node.second_value] = right_node_idx;
    }

    /* printf("huffman tree\n"); */
    /* for (int i = 0; i < huffman_tree_length; i++) { */
    /*     struct tree_node current_node = huffman_tree[i]; */
    /*     printf("(%d, %d, %d)\n", current_node.left, current_node.value, */
    /*            current_node.right); */
    /* } */
    /* printf("\n"); */

    struct PGM_Image *decoded_image = malloc(sizeof(*decoded_image));
    create_PGM_Image(decoded_image, image_width, image_height, max_gray_value);

    // decode image using huffman tree
    size_t row = 0;
    size_t col = 0;
    struct tree_node current_node = huffman_tree[0];

    /* printf("current_node=(%d, %d, %d)\n", current_node.left,
     * current_node.value, */
    /*        current_node.right); */

    for (size_t encoded_idx = 0; encoded_idx < length_of_encoded_image_array;
         encoded_idx++) {
        uint8_t encoded_byte = encoded_image[encoded_idx];
        for (int8_t encoded_bit_pos = 7; encoded_bit_pos >= 0;
             encoded_bit_pos--) {
            uint8_t encoded_bit = (encoded_byte >> encoded_bit_pos) & 1;

            /* printf("bit=%d ", encoded_bit); */
            if (encoded_bit == 0) {
                current_node = huffman_tree[current_node.left];
            } else {
                current_node = huffman_tree[current_node.right];
            }
            /* printf("current_node=(%d, %d, %d)\n", current_node.left, */
            /*        current_node.value, current_node.right); */

            if (current_node.value < 0) {
                continue;
            }

            /* printf("DECODED %d\n\n", current_node.value); */
            decoded_image->image[row][col] = current_node.value;

            col += 1;
            if (col == image_width) {
                row += 1;
                col = 0;
            }

            if (row >= image_height) {
                return decoded_image;
            }

            current_node = huffman_tree[0];
            /* printf("current_node=(%d, %d, %d)\n", current_node.left, */
            /*        current_node.value, current_node.right); */
        }
    }

    /* printf("finished decoding image"); */
    return decoded_image;
}
