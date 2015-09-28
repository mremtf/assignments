project( sudoku_ocr)
cmake_minimum_required(VERSION 2.8)
add_definitions(-DHAVE_ALLOCA -DHAVE_ALLOCA_H -DHAVE_LIBPTHREAD -DHAVE_UNISTD_H)
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -stdlib=libc++ -std=c++0x -O3 -Wall" )
find_package( OpenCV REQUIRED )
add_executable( sudoku_ocr src/sudoku_ocr.cpp) 
target_link_libraries( sudoku_ocr ${OpenCV_LIBS} )

