add_library(Camera STATIC Camera.cpp)

target_include_directories(Camera PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Camera)