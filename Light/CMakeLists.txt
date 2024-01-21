add_library(Lights STATIC PointLight.cpp)

target_include_directories(Lights PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Light)