cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR};${CMAKE_MODULE_PATH};")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(cmake/vcpkgdependencies.cmake)
set(VCPKG_ROOT ${CMAKE_BINARY_DIR}/vcpkg)
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
vcpkg_install(4474aba1e77be2d643eb684298b7d5479cad9a1f)

project(cl-renderer VERSION 0.1.0)

vcpkg_add_dependencies(
    boolinq
    jsoncpp
    libpng
    zstd
    )

find_path(BOOLINQ_INCLUDE_DIRS "boolinq/boolinq.h")
find_package(jsoncpp CONFIG REQUIRED)
find_package(libpng CONFIG REQUIRED)
find_package(OpenCL REQUIRED)
find_package(zstd CONFIG REQUIRED)

set(${PROJECT_NAME}_source
    ./main.cpp
    ./app.cpp
    ./openclcommandqueue.cpp
    ./openclcontext.cpp
    ./opencldevice.cpp
    ./openclkernel.cpp
    ./openclmem.cpp
    ./openclplatform.cpp
    ./openclprogram.cpp
    ./pngimagewriter.cpp
    ./tile.cpp
    ./tilemap.cpp
    )

set(${PROJECT_NAME}_include
    ./app.hpp
    ./log.hpp
    ./openclcommandqueue.hpp
    ./openclcontext.hpp
    ./opencldevice.hpp
    ./openclkernel.hpp
    ./openclmem.hpp
    ./openclplatform.hpp
    ./openclprogram.hpp
    ./pngimagewriter.hpp
    ./serializable.hpp
    ./tile.hpp
    ./tilemap.hpp
    )

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_include}
    ${${PROJECT_NAME}_source}
    )

#set_property(TARGET ${PROJECT_NAME}
#    PROPERTY CXX_STANDARD 17
#    )

target_include_directories(${PROJECT_NAME}
    PRIVATE ${OpenCL_INCLUDE_DIRS}
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE jsoncpp_object jsoncpp_static
    PRIVATE png_static
    PRIVATE ${OpenCL_LIBRARIES}
    PRIVATE zstd::libzstd_static
    )

