
# Call me before using `project()`
function(vcpkg_setup_toolchain)
    # You need to provide VCPKG_ROOT yourself!
    # cmake -D VCPKG_ROOT="path/to/vcpkg/root" ../path/to/source
    # or set VCPKG_ROOT environment variable.
    if(NOT DEFINED VCPKG_ROOT)
        # This is not a good place!
        #set(VCPKG_ROOT "${CMAKE_BINARY_DIR}/vcpkg")
        if(DEFINED ENV{VCPKG_ROOT})
            # Set from system environment variable.
            message(STATUS "vcpkg_setup_toolchain: setting VCPKG_ROOT from system environment variable")
            file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" VCPKG_ROOT)
        endif()
    endif()

    message(STATUS "vcpkg_setup_toolchain: VCPKG_ROOT (${VCPKG_ROOT})")

    if(NOT DEFINED VCPKG_ROOT)
        # We complain, and quit if `VCPKG_ROOT` is not set.
        message(FATAL_ERROR "vcpkg_setup_toolchain: VCPKG_ROOT not defined. VCPKG_ROOT must be set to vcpkg root directory. https://vcpkg.io/en/getting-started.html")
    endif()
    

    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_TOOLCHAIN_FILE}" PARENT_SCOPE)
    endif()

    message(STATUS "vcpkg_setup_toolchain: CMAKE_TOOLCHAIN_FILE (${CMAKE_TOOLCHAIN_FILE})")

    # Put this at the end. We really only want to cache this if no errors occur above.
    set(VCPKG_ROOT "${VCPKG_ROOT}" CACHE PATH "Path to vcpkg root directory.")
endfunction()

