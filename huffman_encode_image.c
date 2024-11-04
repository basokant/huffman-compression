#include <stdint.h>
#include <stdlib.h>

#include "generate_huffman_nodes.h"
#include "libpnm.h"

unsigned char *generate_huffman_codes(struct node *huffman_node,
                                      int number_of_nodes, int max_gray_value) {
    unsigned char *huffman_codes =
        (unsigned char *)calloc(max_gray_value + 1, sizeof(*huffman_codes));

    for (int i = number_of_nodes - 1; i >= 0; i--) {
        struct node current_node = huffman_node[i];
        int first_color = current_node.first_value;
        int second_color = current_node.second_value;

        int first_code = huffman_codes[first_color] << 1;
        int second_code = (huffman_codes[first_color] << 1) + 1;
        huffman_codes[first_color] = first_code;
        huffman_codes[second_color] = second_code;
    }

    printf("\nhuffman codes:\n");
    for (int i = 0; i < max_gray_value + 1; i++) {
        printf("%d: %d\n", i, huffman_codes[i]);
    }
    printf("\n");

    return huffman_codes;
}

unsigned char *huffman_encode_image(struct PGM_Image *input_pgm_image,
                                    struct node *huffman_node,
                                    int number_of_nodes,
                                    long int *length_of_encoded_image_array) {
    uint8_t *huffman_codes = generate_huffman_codes(
        huffman_node, number_of_nodes, input_pgm_image->maxGrayValue);

    size_t image_length = input_pgm_image->height * input_pgm_image->width;
    uint8_t *encoded_image =
        (uint8_t *)calloc(image_length, sizeof(*huffman_codes));

    uint8_t *image = (uint8_t *)calloc(image_length, sizeof(*huffman_codes));
    for (int row = 0; row < input_pgm_image->height; row++) {
        for (int col = 0; col < input_pgm_image->width; col++) {
            size_t pixel_idx = (row * input_pgm_image->width) + col;
            uint8_t pixel_value = input_pgm_image->image[row][col];

            image[pixel_idx] = pixel_value;
        }
    }

    size_t encoded_img_idx = 0;
    int8_t encoded_img_bit_pos = 7;
    for (size_t i = 0; i < image_length; i++) {
        uint8_t pixel_value = image[i];
        uint8_t encoded_pixel = huffman_codes[pixel_value];
        printf("i=%zu byte=%zu bit=%d pixel_value=%d encoded_pixel=%d ", i,
               encoded_img_idx, encoded_img_bit_pos, pixel_value,
               encoded_pixel);

        int8_t leading_one_bit_pos;
        for (leading_one_bit_pos = 7; leading_one_bit_pos >= 0;
             leading_one_bit_pos--) {
            bool is_leading_one = encoded_pixel & (1 << leading_one_bit_pos);
            if (is_leading_one) {
                break;
            }
        }
        printf("leading_one_bit_pos=%hhd\n", leading_one_bit_pos);

        bool is_encoded_pixel_zero = leading_one_bit_pos < 0;
        if (is_encoded_pixel_zero) {
            encoded_img_bit_pos--;
            bool byte_boundary_crossed = encoded_img_bit_pos < 0;
            if (byte_boundary_crossed) {
                encoded_img_idx++;        // go to next byte
                encoded_img_bit_pos = 7;  // start bit position at 7
            }
            continue;
        }

        // from found leading one to end of encoded pixel
        for (int8_t encoded_pixel_bit_pos = leading_one_bit_pos;
             encoded_pixel_bit_pos >= 0; encoded_pixel_bit_pos--) {
            // set output at bit_pos based on bits after leading one in
            uint8_t encoded_bit = (encoded_pixel >> encoded_pixel_bit_pos) & 1;
            printf("i=%zu encoded_bit=%d  encoded_pixel_bit_pos=%d\n", i,
                   encoded_bit, encoded_pixel_bit_pos);
            encoded_image[encoded_img_idx] |= encoded_bit
                                              << encoded_img_bit_pos;

            encoded_img_bit_pos--;
            bool byte_boundary_crossed = encoded_img_bit_pos < 0;
            if (byte_boundary_crossed) {
                encoded_img_idx++;        // go to next byte
                encoded_img_bit_pos = 7;  // start bit position at 7
            }
        }

        printf("\n");
    }

    *length_of_encoded_image_array = encoded_img_idx + 1;
    free(image);
    free(huffman_codes);

    return encoded_image;
}
