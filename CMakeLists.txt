cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR};${CMAKE_MODULE_PATH};")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(vcpkg_setup_toolchain.cmake)
vcpkg_setup_toolchain()

project(cl-renderer VERSION 0.1.0)

find_path(PNGPP_INCLUDE_DIRS "png++/color.hpp")
find_path(BOOLINQ_INCLUDE_DIRS "boolinq/boolinq.h")
#find_package(jsoncpp CONFIG REQUIRED)
find_package(libpng CONFIG REQUIRED)
find_package(OpenCL REQUIRED)
#find_package(zstd CONFIG REQUIRED)
find_package(PNG REQUIRED)
find_package(argparse CONFIG REQUIRED)

set(${PROJECT_NAME}_source
    "src/main.cpp"
    "src/app.cpp"
    "src/openclcommandqueue.cpp"
    "src/openclcontext.cpp"
    "src/opencldevice.cpp"
    "src/openclkernel.cpp"
    "src/openclmem.cpp"
    "src/openclplatform.cpp"
    "src/openclprogram.cpp"
    )

set(${PROJECT_NAME}_include
    "src/app.hpp"
    "src/log.hpp"
    "src/openclcommandqueue.hpp"
    "src/openclcontext.hpp"
    "src/opencldevice.hpp"
    "src/openclkernel.hpp"
    "src/openclmem.hpp"
    "src/openclplatform.hpp"
    "src/openclprogram.hpp"
    "src/serializable.hpp"
    "src/deserializable.hpp"
    "src/image.hpp"
    )

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_include}
    ${${PROJECT_NAME}_source}
    )

#set_property(TARGET ${PROJECT_NAME}
#    PROPERTY CXX_STANDARD 17
#    )

target_include_directories(${PROJECT_NAME}
    PRIVATE ${OpenCL_INCLUDE_DIR}
    PRIVATE ${PNGPP_INCLUDE_DIRS}
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${OpenCL_LIBRARIES}
    PRIVATE PNG::PNG
    PRIVATE argparse::argparse
    #PRIVATE JsonCpp::JsonCpp
    #PRIVATE $<IF:$<TARGET_EXISTS:zstd::libzstd_shared>,zstd::libzstd_shared,zstd::libzstd_static>
    )

