cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR};${CMAKE_MODULE_PATH};")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(cl-renderer VERSION 0.1.0)

vcpkg_add_dependencies(
    boolinq
    jsoncpp
    libpng
    )

find_path(BOOLINQ_INCLUDE_DIRS "boolinq/boolinq.h")
find_package(jsoncpp CONFIG REQUIRED)
find_package(libpng CONFIG REQUIRED)
find_package(OpenCL REQUIRED)

set(${PROJECT_NAME}_source
    ./main.cpp
    ./app.cpp
    )

set(${PROJECT_NAME}_include
    ./app.hpp
    ./log.hpp
    )

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_include}
    ${${PROJECT_NAME}_source}
    )

target_include_directories(${PROJECT_NAME}
    PRIVATE ${OpenCL_INCLUDE_DIRS}
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE jsoncpp_object jsoncpp_static
    PRIVATE png_static
    PRIVATE ${OpenCL_LIBRARIES}
    )

