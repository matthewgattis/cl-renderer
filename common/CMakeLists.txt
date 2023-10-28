
project(cl-renderer-common VERSION 0.1.0)

set(${PROJECT_NAME}_source
    openclcommandqueue.cpp
    openclcontext.cpp
    opencldevice.cpp
    openclkernel.cpp
    openclmem.cpp
    openclplatform.cpp
    openclprogram.cpp
    )

set(${PROJECT_NAME}_include
    image.hpp
    log.hpp
    openclcommandqueue.hpp
    openclcontext.hpp
    opencldevice.hpp
    openclkernel.hpp
    openclmem.hpp
    openclplatform.hpp
    openclprogram.hpp
    serializable.hpp
    deserializable.hpp
    message.hpp
    config.hpp
    )

add_library(${PROJECT_NAME}
    ${${PROJECT_NAME}_include}
    ${${PROJECT_NAME}_source}
    )

#set_property(TARGET ${PROJECT_NAME}
#    PROPERTY CXX_STANDARD 17
#    )

target_include_directories(${PROJECT_NAME}
    PRIVATE ${OpenCL_INCLUDE_DIRS}
    PRIVATE ${BOOLINQ_INCLUDE_DIRS}
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE jsoncpp_object jsoncpp_static
    PRIVATE ${OpenCL_LIBRARIES}
    PRIVATE $<IF:$<TARGET_EXISTS:zstd::libzstd_shared>,zstd::libzstd_shared,zstd::libzstd_static>
    )

