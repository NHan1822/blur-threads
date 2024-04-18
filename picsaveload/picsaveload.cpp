#include "picsaveload.h"
#include <stdio.h>
#include <jpeglib.h>

Image loadPicture(const char* file){
    Image image;
    FILE* fl = fopen(file,"rb");
    if (!fl) {
        return image;
    }
    jpeg_decompress_struct info;
    jpeg_error_mgr error;

    info.err = jpeg_std_error(&error);
    jpeg_create_decompress(&info);

    jpeg_stdio_src(&info, fl);
    jpeg_read_header(&info, TRUE);

    jpeg_start_decompress(&info);

    image.width = info.output_width;
    image.height = info.output_height;
    image.num_components = info.output_components;
    image.data = new unsigned char[image.width*image.num_components*image.height];

    unsigned char *buffer[1];

    while (info.output_scanline < info.output_height) {
        buffer[0] = (unsigned char*)(&image.data[info.output_components*info.output_width*info.output_scanline]);
        jpeg_read_scanlines(&info, buffer, 1);
    }

    jpeg_finish_decompress(&info);
    jpeg_destroy_decompress(&info);
    fclose(fl);
    return image;
}

void savePicture(const char *file, Image &image){
    FILE* fl = fopen(file, "wb");
    if (!fl) {
        return;
    }
    jpeg_compress_struct info;
    jpeg_error_mgr error;
    info.err = jpeg_std_error(&error);
    jpeg_create_compress(&info);

    jpeg_stdio_dest(&info, fl);

    info.image_width = image.width;
    info.image_height = image.height;
    info.input_components = image.num_components;
    info.in_color_space = JCS_RGB;
    
    jpeg_set_defaults(&info);
    jpeg_start_compress(&info, TRUE);

    unsigned char *buffer[1];

    while(info.next_scanline<info.image_height) {
        buffer[0] = (unsigned char*)(&image.data[info.next_scanline*image.width*image.num_components]);
        jpeg_write_scanlines(&info, buffer, 1);
    }
    
    jpeg_finish_compress(&info);
    jpeg_destroy_compress(&info);
    fclose(fl);
}