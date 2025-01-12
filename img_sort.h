#ifndef IMG_SORT_H
#define IMG_SORT_H


#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define LUMINANCE_COEFFICIENT_R 0.2126
#define LUMINANCE_COEFFICIENT_G 0.7152
#define LUMINANCE_COEFFICIENT_B 0.0722
#define MAX_FILENAME_LENGTH 256

typedef struct {
	unsigned char r, g, b;
} Pixel;

void load_pixel_array(unsigned char *data, Pixel *pixel_array, int pixel_num);
void save_pixel_array(Pixel* pixel_array, unsigned char* data, int pixel_num);

void sanitise_out_filename(char* out_name, char* filename);

typedef int (*compare_image_t)(const void *a, const void *b);
int cb_compare_luminance(const void *a, const void *b);
int cb_compare_hex(const void *a, const void *b);

#endif