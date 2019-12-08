cmake_minimum_required(VERSION 3.14)
project(A7)

set(CMAKE_CXX_STANDARD 11)

include_directories(Z:/CSCI441/include)
link_directories(Z:/CSCI441/lib)

add_executable(A7 main.cpp Enemy.cpp Enemy.h Model_Base.cpp Model_Base.h Light.cpp Light.h MyKart.cpp MyKart.h)
target_link_libraries(A7 a7 SOIL3 glew32.dll opengl32 glfw3 gdi32)