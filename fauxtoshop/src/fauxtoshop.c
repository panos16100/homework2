#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WID 18 //width
#define HI 22 //high
#define FILES 2 //file_size
#define PHOTOS 34 //photo_size
#define PIX_START 10 //pixels_start

int main(void) {
    char headers[54];
    char *otherdata;
    char *pixel_data;
    
    // Read the BMP header
    int diabasa = fread(headers, sizeof(char), 54, stdin);
    
    // Check for valid BMP file
    if ((diabasa < 54) || (headers[0] != 'B') || (headers[1] != 'M')) {
        fprintf(stderr, "Not enough headers in the file or not a valid BMP file.\n");
        exit(1);
    }

    int width, height;
    width = *((int *)(headers + WID));
    height = *((int *)(headers + HI));

    // Swap width and height for 90-degree rotation
    *((int *)(headers + WID)) = height;
    *((int *)(headers + HI)) = width;

    int file_size;
    file_size = *((int *)(headers + FILES));
    int photo_size;
    photo_size = *((int *)(headers + PHOTOS));
    int pixels_start;
    pixels_start = *((int *)(headers + PIX_START));
    int diafora = pixels_start - 54;

    // Read and write the rest of the header
    otherdata = malloc(diafora * sizeof(char));
    fread(otherdata, sizeof(char), diafora, stdin);
    fwrite(headers, sizeof(char), 54, stdout);
    fwrite(otherdata, sizeof(char), diafora, stdout);

    // Calculate padding for the original image
    int padding = (4 - (width * 3) % 4) % 4;

    // Allocate memory for the pixel data
    pixel_data = malloc((width * height * 3 + height * padding) * sizeof(char));
    fread(pixel_data, sizeof(char), width * height * 3 + height * padding, stdin);

    // Allocate memory for the rotated pixel data
    char *rotated_array = malloc((width * height * 3 + width * padding) * sizeof(char));

    // Perform the 90-degree rotation
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int original_index = (i * width + j) * 3 + i * padding;
            int rotated_index = ((width - j - 1) * height + i) * 3 + (width - j - 1) * padding;
            memcpy(rotated_array + rotated_index, pixel_data + original_index, 3);
        }
    }

    // Write the rotated pixel data to stdout
    fwrite(rotated_array, sizeof(char), width * height * 3 + width * padding, stdout);

    // Free allocated memory
    free(otherdata);
    free(pixel_data);
    free(rotated_array);

    return 0;
}
