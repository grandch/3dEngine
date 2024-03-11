cmake_minimum_required(VERSION 3.10)

project(3dEngine)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(Anim Inputs Bezier Camera Importer ext/stb Light Mesh Scene Scene/Axis Shaders ${OPENGL_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})

add_library(Anim SHARED Anim/Bone.cpp)
add_library(Inputs SHARED Inputs/Input.cpp)
add_library(Bezier SHARED Bezier/BezierCurve.cpp Bezier/BezierSurface.cpp)
add_library(Camera SHARED Camera/CameraDualQuaternion.cpp Camera/CameraRotateAround.cpp)
add_library(Importer SHARED Importer/Importer.cpp)
add_library(Light SHARED Light/PointLight.cpp)
add_library(Mesh SHARED Mesh/Mesh.cpp Mesh/MeshEdge.cpp Mesh/MeshHalfEdge.cpp Mesh/MeshTriangle.cpp Mesh/MeshVertex.cpp)
add_library(Scene SHARED Scene/Scene.cpp Scene/BezierManager.cpp Scene/LightManager.cpp Scene/MeshManager.cpp Scene/Axis/Axis.cpp)
add_library(Shaders SHARED Shaders/Shader.cpp)
add_library(ext SHARED ext/stb/stb_image.cpp)

add_executable(ref ref.cpp)
add_executable(skinningCPU skinningCPU.cpp)
add_executable(skinningGPU skinningGPU.cpp)

target_link_libraries(ref Anim Inputs Bezier Camera Importer Light Mesh Scene Shaders ext ${OPENGL_LIBRARIES} ${GLM_LIBRARY_DIRS} ${SDL2_LIBRARIES})
target_link_libraries(skinningCPU Anim Inputs Bezier Camera Importer Light Mesh Scene Shaders ext ${OPENGL_LIBRARIES} ${GLM_LIBRARY_DIRS} ${SDL2_LIBRARIES})
target_link_libraries(skinningGPU Anim Inputs Bezier Camera Importer Light Mesh Scene Shaders ext ${OPENGL_LIBRARIES} ${GLM_LIBRARY_DIRS} ${SDL2_LIBRARIES})

file(GLOB VERTEXSHADERS
  "Shaders/*.vert"
)

file(GLOB FRAGMENTSHADERS
  "Shaders/*.frag"
)

file(GLOB TEXTURES
  "Shaders/*.jpg"
)

file(GLOB MODELS
  "Models/*.obj"
)

file(COPY ${VERTEXSHADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Shaders/)
file(COPY ${FRAGMENTSHADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Shaders/)
file(COPY ${TEXTURES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Shaders/)
file(COPY ${MODELS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Models/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/convert.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})