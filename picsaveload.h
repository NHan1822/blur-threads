#pragma ones

struct Image {
public:
    unsigned char *data;
    int width, height, num_components;
    Image(): data(nullptr), width(0), height(0), num_components(0){}
};
Image loadPicture(const char* file);
void savePicture(const char *file, Image &image);