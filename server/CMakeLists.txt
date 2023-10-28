
project(cl-renderer-server VERSION 0.1.0)

set(${PROJECT_NAME}_source
    ./main.cpp
    ./app.cpp
    )

set(${PROJECT_NAME}_include
    ./app.hpp
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
    PRIVATE ${PNGPP_INCLUDE_DIRS}
    PRIVATE ${cl-renderer-common_SOURCE_DIR}
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE cl-renderer-common
    PRIVATE jsoncpp_object jsoncpp_static
    PRIVATE ${OpenCL_LIBRARIES}
    PRIVATE zstd::libzstd_static
    PRIVATE $<IF:$<TARGET_EXISTS:zstd::libzstd_shared>,zstd::libzstd_shared,zstd::libzstd_static>
    )

