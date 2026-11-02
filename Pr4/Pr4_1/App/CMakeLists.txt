add_executable(MyApp main.cpp)
target_link_libraries(MyApp PRIVATE MatrixLib)
target_compile_features(MyApp PRIVATE cxx_std_20)