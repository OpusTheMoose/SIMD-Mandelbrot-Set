cmake_minimum_required(VERSION 3.5.0)
project(Fractal VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)


find_package(GLEW REQUIRED)

add_executable(Fractal src/GLRender.cpp src/fractalV2.cpp src/main.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -fopenmp -pthread -O3")
target_link_libraries(Fractal PRIVATE sfml-graphics GLEW GL)


include(CTest)
enable_testing()

