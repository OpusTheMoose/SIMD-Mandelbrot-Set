cmake_minimum_required(VERSION 3.5.0)
project(Fractal VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)


add_executable(Fractal GLRender.cpp fractalV2.cpp glad.c main.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -fopenmp -pthread -Ofast")
target_link_libraries(Fractal PRIVATE sfml-graphics)


include(CTest)
enable_testing()

