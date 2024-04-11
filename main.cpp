#include <stdio.h>
#include <jpeglib.h>
#include <vector>
#include <thread>
#include <iostream>
#include <chrono>
#include "picsaveload.h"
#include "blurthreads.h"

const int num_threads = 1;

int main() {
    auto begin = std::chrono::steady_clock::now();
    const char *input_file = "test.jpg";
    const char *output_file = "output.jpg";
    int blur_radius = 50;
    
    Image image = loadPicture(input_file);
    if(image.data == nullptr) {
        return 1;
    }
    int rows_per_thread = image.height/num_threads;
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        int start_row = i*rows_per_thread;
        int end_row = 0;
        if (i==num_threads-1) {
            end_row = image.height;
        }
            else {
                end_row = (i+1)*rows_per_thread;
            }
        threads.emplace_back(blurThread, std::ref(image), start_row, end_row, blur_radius);
    }

    for(auto &thread:threads){
        thread.join();
    }

    savePicture(output_file, image);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin);
    std::cout<<"Complete\n";
    std::cout<<"Duration: "<<(double)duration.count()/1000<<" s\n";
    return 0;
}