cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR};${CMAKE_MODULE_PATH};")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(vcpkgdependencies.cmake)
set(VCPKG_ROOT ${CMAKE_BINARY_DIR}/vcpkg)
if (DEFINED ENV{VCPKG_ROOT})
    set(VCPKG_ROOT "$ENV{VCPKG_ROOT}")
endif()
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
vcpkg_install()

project(cl-renderer VERSION 0.1.0)

vcpkg_add_dependencies(
    "boolinq"
    "jsoncpp"
    "libpng"
    # https://stackoverflow.com/questions/57745122/how-to-compress-a-c-string-using-zstd
    "zstd"
    "libsodium"
    "zeromq[sodium]"
    "cppzmq"
    "pngpp"
    )

find_package(OpenCL REQUIRED)
find_package(zstd CONFIG REQUIRED)
find_package(ZeroMQ CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(libpng CONFIG REQUIRED)
find_package(unofficial-sodium CONFIG REQUIRED)
find_path(PNGPP_INCLUDE_DIRS "png++/color.hpp")
find_path(BOOLINQ_INCLUDE_DIRS "boolinq/boolinq.h")

add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)
