add_library(Shader STATIC Shader.cpp)

target_include_directories(Shader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Shader)