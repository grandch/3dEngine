add_library(Scene Scene.cpp BezierManager.cpp LightManager.cpp MeshManager.cpp)

add_subdirectory(Axis)
link_libraries(Scene PUBLIC Axis)
include_directories(Scene PUBLIC
                    "${PROJECT_BINARY_DIR}"
                    "${PROJECT_SOURCE_DIR}/Axis")