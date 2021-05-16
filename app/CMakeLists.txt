add_executable(app main.c)

target_link_libraries(app PRIVATE rle)

file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_files)
file(COPY ${RLE_Naive_SOURCE_DIR}/test_files/input.bin DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_files)