cmake_minimum_required(VERSION 3.5.1)
project(lab5 C)

set(SRC src/main.c src/huffman.c src/huffman.h src/bitstream.c src/bitstream.h)

add_executable(${PROJECT_NAME} ${SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
