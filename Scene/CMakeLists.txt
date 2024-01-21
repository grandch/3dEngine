add_library(Scene STATIC Scene.cpp BezierManager.cpp LightManager.cpp MeshManager.cpp)

target_include_directories(Scene PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Scene)

add_subdirectory(Axis)
link_libraries(Scene PUBLIC Axis)
include_directories(Scene PUBLIC
                    "${PROJECT_BINARY_DIR}"
                    "${PROJECT_SOURCE_DIR}/Scene/Axis")