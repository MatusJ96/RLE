set(SOURCES rle.c)

set(HEADER_LIST "${RLE_Naive_SOURCE_DIR}/include/rle.h")

add_library(rle ${SOURCES} ${HEADER_LIST})

target_include_directories(rle PUBLIC ../include)
