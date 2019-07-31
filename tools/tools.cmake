# benchmark
add_executable(benchmark ${CMAKE_CURRENT_LIST_DIR}/benchmark.cpp)
target_link_libraries(benchmark caffe)

add_executable(caffe_encrypt_tool ${CMAKE_CURRENT_LIST_DIR}/encrypt_tool.cpp)
target_link_libraries(caffe_encrypt_tool caffe)
