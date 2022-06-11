cmake_minimum_required(VERSION 3.16)

project(Boids VERSION 1.0)

include(CTest)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")

string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address -fno-omit-frame-pointer")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address -fno-omit-frame-pointer")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(simulation main.cpp simulation.cpp)

target_link_libraries(simulation PRIVATE sfml-graphics)



if (BUILD_TESTING)
add_executable(simulation.t simulation.test.cpp)
target_link_libraries(simulation.t PRIVATE sfml-graphics)
add_test(NAME simulation.t COMMAND simulation.t)

endif()