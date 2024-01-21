add_library(Mesh STATIC Mesh.cpp MeshEdge.cpp MeshHalfEdge.cpp MeshTriangle.cpp MeshVertex.cpp)

target_include_directories(Mesh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Mesh)