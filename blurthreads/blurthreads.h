#pragma ones
#include "../picsaveload/picsaveload.h"

void blur(unsigned char *row, int width, int radius, int num_components);

void blurThread (Image &image, int start_row, int end_row, int radius);