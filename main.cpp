#include <stdio.h>
#include <jpeglib.h>
#include <vector>
#include <thread>
#include <iostream>
#include <chrono>
//#include "picsaveload.h"
#include "blurthreads/blurthreads.h"
#include <filesystem>
#include <fstream>

const std::vector<int> numbers_of_threads = {1,2,4,8};

int main() {
    std::vector<std::string> folders {"data/1280x720", "data/1920x1080","data/Above"};
    std::ofstream statisctic;
    statisctic.open("statistic.txt");
    for (auto num_threads : numbers_of_threads) {
        for (auto folder : folders) {
            std::vector<double> duratioan_for_each_file;
            double time_per_folder = 0;
            for (const auto &entry : std::filesystem::directory_iterator(folder)) {
                if (std::filesystem::is_regular_file(entry.path())) {
                    auto begin = std::chrono::steady_clock::now();
                    auto a = entry.path().filename().c_str();
                    const char *input_file = entry.path().c_str();
                    const char *output_file = entry.path().c_str();
                    int blur_radius = 50;

                    Image img = loadPicture(input_file);
                    if(img.data == nullptr) {
                        return 1;
                    }
                    int rows_per_thread = img.height/num_threads;
                    std::vector<std::thread> threads;

                    for (int i = 0; i < num_threads; ++i) {
                        int start_row = i*rows_per_thread;
                        int end_row = 0;
                        if (i==num_threads-1) {
                            end_row = img.height;
                        }
                            else {
                                end_row = (i+1)*rows_per_thread;
                            }
                        threads.emplace_back(blurThread, std::ref(img), start_row, end_row, blur_radius);
                    }

                    for(auto &thread:threads){
                        thread.join();
                    }

                    savePicture(output_file, img);
                    auto end = std::chrono::steady_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin);
                    duratioan_for_each_file.push_back((double)duration.count()/1000);
                    time_per_folder+=(double)duration.count()/1000;
                }
            }
             if(statisctic.is_open()) {
            statisctic<<"Average time for "<<folder<<" and "<<num_threads<<" threads is "<<time_per_folder/duratioan_for_each_file.size()<<" s\n";
            }
        }
    }
    std::cout<<"Complete\n";
    return 0;
}