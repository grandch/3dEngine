cmake_minimum_required(VERSION 3.10)

project(3dEngine)

add_executable(3dEngine main.cpp)

add_subdirectory(Bezier)
add_subdirectory(Camera)
add_subdirectory(Importer)
add_subdirectory(Inputs)
add_subdirectory(Light)
add_subdirectory(Mesh)
add_subdirectory(Scene)
add_subdirectory(Shaders)
add_subdirectory(lib)

# target_link_directories(3dEngine PUBLIC
#                         Bezier
#                         Camera
#                         Importer
#                         Inputs
#                         Light
#                         Mesh
#                         Scene
#                         Shaders
#                         lib)



# target_include_directories(3dEngine PUBLIC
#                            "${PROJECT_BINARY_DIR}"
#                            "${PROJECT_SOURCE_DIR}/Bezier"
#                            "${PROJECT_SOURCE_DIR}/Camera"
#                            "${PROJECT_SOURCE_DIR}/Importer"
#                            "${PROJECT_SOURCE_DIR}/Inputs"
#                            "${PROJECT_SOURCE_DIR}/Light"
#                            "${PROJECT_SOURCE_DIR}/Mesh"
#                            "${PROJECT_SOURCE_DIR}/Scene"
#                            "${PROJECT_SOURCE_DIR}/Shaders"
#                            "${PROJECT_SOURCE_DIR}/lib")