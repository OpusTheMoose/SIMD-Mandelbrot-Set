cmake_minimum_required(VERSION 3.5.0)
project(Fractal VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)

FetchContent_Declare(SDL3 
        URL https://github.com/libsdl-org/SDL/releases/download/release-3.2.20/SDL3-3.2.20.tar.gz
        
)
FetchContent_MakeAvailable(SDL3)


find_package(GLEW REQUIRED)

add_executable(Fractal src/GLRender.cpp src/fractalV2.cpp src/main.cpp src/gl.c)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -fopenmp -pthread -O3")
target_include_directories(Fractal PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(Fractal PRIVATE SDL3 GL)


include(CTest)
enable_testing()

