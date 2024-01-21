add_library(Importer STATIC Importer.cpp)

target_include_directories(Importer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Importer)