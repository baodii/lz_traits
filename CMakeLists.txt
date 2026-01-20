cmake_minimum_required(VERSION 3.16)
project(ze_info LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LevelZero REQUIRED level-zero)

add_executable(ze_info ze_info.cpp)

target_include_directories(ze_info PRIVATE ${LevelZero_INCLUDE_DIRS})
target_link_directories(ze_info PRIVATE ${LevelZero_LIBRARY_DIRS})
target_link_libraries(ze_info PRIVATE ${LevelZero_LIBRARIES})
target_compile_options(ze_info PRIVATE ${LevelZero_CFLAGS_OTHER})

