cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_BUILD_TYPE Debug)

project(lab)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SRC CONFIGURE_DEPENDS "*.h" "*.cpp")

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC)

