#include "img_sort.h"

int main(int argc, char ** argv) {
	if (argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	char in_filename[MAX_FILENAME_LENGTH];
	strncpy(in_filename, argv[1], sizeof(in_filename) - 1);
	in_filename[sizeof(in_filename) - 1] = '\0';

	int width, height, channels;
	unsigned char* data = stbi_load(in_filename, &width, &height, &channels, 3);
	if (data == NULL) {
		printf("File could not be opened or memory allocation error\n");
		stbi_image_free(data);
		return 1;
	}
	printf("Image loaded: %dx%d, %d channels\n", width, height, channels);

	int pixel_num = width * height;
	Pixel* pixels = malloc(pixel_num * sizeof(Pixel));
	if (pixels == NULL) {
		printf("Error allocating pixel array\n");
		stbi_image_free(data);
		return 1;
	}

	load_pixel_array(data, pixels, pixel_num);

	compare_image_t comparer = cb_compare_luminance;
	qsort(pixels, pixel_num, sizeof(Pixel), comparer);

	unsigned char* sorted_pixels = malloc(pixel_num * 3); // 3 channels
	if (sorted_pixels == NULL) {
		printf("Error allocating pixel array\n");
		free(pixels);
		stbi_image_free(data);
		return 1;
	}
	save_pixel_array(pixels, sorted_pixels, pixel_num);

	char out_filename[MAX_FILENAME_LENGTH];
	sanitise_out_filename(out_filename, in_filename);
	out_filename[MAX_FILENAME_LENGTH - 1] = '\0';

	if (stbi_write_png(out_filename, width, height, 3, sorted_pixels, width * 3)) {
		printf("Image stored as %s\n", out_filename);
	}
	else {
		printf("Error while storing output image\n");
	}

	free(sorted_pixels);
	free(pixels);
	stbi_image_free(data);
	return 0;
}


void load_pixel_array(unsigned char* data, Pixel* pixel_array, int pixel_num) {
	for (int i = 0; i < pixel_num; i++) {
		pixel_array[i].r = data[i * 3 + 0];
		pixel_array[i].g = data[i * 3 + 1];
		pixel_array[i].b = data[i * 3 + 2];
	}
}

void save_pixel_array(Pixel* pixel_array, unsigned char* data, int pixel_num) {
	for (int i = 0; i < pixel_num; i++) {
		data[i * 3 + 0] = pixel_array[i].r;
		data[i * 3 + 1] = pixel_array[i].g;
		data[i * 3 + 2] = pixel_array[i].b;
	}
}

int cb_compare_luminance(const void* a, const void* b) {
	const Pixel* pixelA = (const Pixel*)a;
	const Pixel* pixelB = (const Pixel*)b;

	int luminanceA = LUMINANCE_COEFFICIENT_R * pixelA->r + LUMINANCE_COEFFICIENT_G * pixelA->g + LUMINANCE_COEFFICIENT_B * pixelA->b;
	int luminanceB = LUMINANCE_COEFFICIENT_R * pixelB->r + LUMINANCE_COEFFICIENT_G * pixelB->g + LUMINANCE_COEFFICIENT_B * pixelB->b;
	
	return luminanceA - luminanceB;
}

int cb_compare_hex(const void* a, const void* b) {
	const Pixel* pixelA = (const Pixel*)a;
	const Pixel* pixelB = (const Pixel*)b;
	int totalA = 0;
	int totalB = 0;

	totalA += (pixelA->r << 16);
	totalA += (pixelA->g << 8);
	totalA += (pixelA->b << 0);

	totalB += (pixelB->r << 16);
	totalB += (pixelB->g << 8);
	totalB += (pixelB->b << 0);

	return totalA - totalB;
}

void sanitise_out_filename(char* out_name, char* filename) {
	char* sanatised_filename = strrchr(filename, '\\'); // if windows filepath
	if (!sanatised_filename) {
		sanatised_filename = strrchr(filename, '/');  // if (anything else)
	}
	sanatised_filename = sanatised_filename ? sanatised_filename + 1 : filename; // + 1 to go after slashes, if none use input

	snprintf(out_name, MAX_FILENAME_LENGTH, "sorted_%s", sanatised_filename);
}