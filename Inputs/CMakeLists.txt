add_library(Inputs STATIC Input.cpp)

target_include_directories(Inputs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Inputs)