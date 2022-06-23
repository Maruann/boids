cmake_minimum_required(VERSION 3.16)

project(Boids VERSION 1.0)

include(CTest)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -pthread")

string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address -fno-omit-frame-pointer")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address -fno-omit-frame-pointer")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
find_package(SFML 2.5 COMPONENTS audio REQUIRED)

add_executable(simulation main.cpp simulation.cpp velocity.cpp graphics.cpp statistics.cpp)

target_link_libraries(simulation PRIVATE sfml-graphics sfml-audio)



if (BUILD_TESTING)
add_executable(simulation.t simulation.test.cpp simulation.cpp velocity.cpp statistics.cpp graphics.cpp)
target_link_libraries(simulation.t PRIVATE sfml-graphics sfml-audio)

add_test(NAME simulation.t COMMAND simulation.t)

endif()