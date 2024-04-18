#include "blurthreads.h"
#include <vector>

void blur(unsigned char *row, int width, int radius, int num_components){
    std::vector <unsigned char> new_row(width*num_components);
    for (int i = 0; i < width; ++i) {
        int sR = 0; int sG = 0; int sB = 0; int cnt = 0;
        for (int di = -radius; di<=radius; ++di){
            int a = std::min(std::max(i+di, 0), width-1);
            sR+=row[a*num_components];
            sG+=row[a*num_components+1];
            sB+=row[a*num_components+2];
            cnt++;
        }
        new_row[i*3]=sR/cnt;
        new_row[i*3+1]=sG/cnt;
        new_row[i*3+2]=sB/cnt;
    }
    std::copy(new_row.begin(), new_row.end(), row);
}   

void blurThread (Image &image, int start_row, int end_row, int radius){
    for (int i=start_row; i<end_row; ++i){
        blur(&image.data[i*image.width*image.num_components], image.width, radius, image.num_components);
    }
}
