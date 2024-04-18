cmake_minimum_required(VERSION 3.16)
project(MultithreadBlur)

set(MAIN_SRC main.cpp)

add_executable(main ${MAIN_SRC})

add_subdirectory(picsaveload)
add_subdirectory(blurthreads)

target_link_libraries(main picsaveload blurthreads jpeg)