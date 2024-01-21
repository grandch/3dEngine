add_library(Image STATIC stb_image.cpp)

target_include_directories(Image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Image)