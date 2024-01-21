add_library(Bezier STATIC BezierCurve.cpp BezierSurface.cpp)

target_include_directories(Bezier PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Bezier)