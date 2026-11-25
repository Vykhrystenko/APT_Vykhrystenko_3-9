cmake_minimum_required(VERSION 3.25)

project(Pr11_FunctionalProgramming LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Part1 part1_lambda_average.cpp)
add_executable(Part2 part2_greater_than.cpp)
add_executable(Part3 part3_immutable_prepend.cpp)

add_executable(Part4_Classic part4_classic_pipeline.cpp)
add_executable(Part4_Ranges part4_ranges_pipeline.cpp)

add_executable(Part5_1 part5_1_remove_if.cpp)
add_executable(Part5_2 part5_2_lambda_capture.cpp)
add_executable(Part5_3 part5_3_dangling.cpp)
add_executable(Part5_4 part5_4_ranges_average.cpp)