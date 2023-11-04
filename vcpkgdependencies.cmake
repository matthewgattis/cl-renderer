
function(vcpkg_install)
    if(NOT EXISTS ${VCPKG_ROOT})
        message("vcpkg install git clone to ${VCPKG_ROOT}")
        execute_process(
            COMMAND git clone https://github.com/microsoft/vcpkg.git ${VCPKG_ROOT})
    endif()

    # if(DEFINED commit)
    #     message("vcpkg install git checkout ${commit}")
    #     execute_process(
    #         COMMAND git checkout ${commit}
    #         WORKING_DIRECTORY ${VCPKG_ROOT})
    # endif()

    # message("vcpkg install git pull")
    # execute_process(
    #     COMMAND git pull
    #     WORKING_DIRECTORY ${VCPKG_ROOT})

    if(WIN32)
        set(vcpkg_exec ${VCPKG_ROOT}/vcpkg.exe)
        set(VCPKG_EXEC ${vcpkg_exec} PARENT_SCOPE)
        set(vcpkg_bootstrap ${VCPKG_ROOT}/bootstrap-vcpkg.bat)
    else()
        set(vcpkg_exec ${VCPKG_ROOT}/vcpkg)
        set(VCPKG_EXEC ${vcpkg_exec} PARENT_SCOPE)
        set(vcpkg_bootstrap ${VCPKG_ROOT}/bootstrap-vcpkg.sh)
    endif()

    if (NOT EXISTS ${vcpkg_exec})
        message("vcpkg install bootstrap")
        execute_process(
            COMMAND ${vcpkg_bootstrap}
            WORKING_DIRECTORY ${VCPKG_ROOT})
    endif()
endfunction()

function(vcpkg_add_dependencies)
    if(NOT DEFINED VCPKG_ROOT)
        set(VCPKG_ROOT "${CMAKE_BINARY_DIR}/vcpkg")
        if (DEFINED ENV{VCPKG_ROOT})
            set(VCPKG_ROOT "$ENV{VCPKG_ROOT}")
        endif()
    endif()
    message(STATUS "vcpkg_add_dependencies VCPKG_ROOT (${VCPKG_ROOT})")

    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_TOOLCHAIN_FILE}" PARENT_SCOPE)
    endif()
    message(STATUS "vcpkg_add_dependencies CMAKE_TOOLCHAIN_FILE (${CMAKE_TOOLCHAIN_FILE})")

    if(NOT EXISTS "${VCPKG_ROOT}")
        message("vcpkg_add_dependencies git clone to ${VCPKG_ROOT}")
        execute_process(
            COMMAND git clone https://github.com/microsoft/vcpkg.git ${VCPKG_ROOT})
    endif()

    if(WIN32)
        set(vcpkg_exec "${VCPKG_ROOT}/vcpkg.exe")
        set(vcpkg_bootstrap "${VCPKG_ROOT}/bootstrap-vcpkg.bat")
    else()
        set(vcpkg_exec "${VCPKG_ROOT}/vcpkg")
        set(vcpkg_bootstrap "${VCPKG_ROOT}/bootstrap-vcpkg.sh")
    endif()
    message(STATUS "vcpkg_add_dependencies vcpkg_exec (${vcpkg_exec})")
    message(STATUS "vcpkg_add_dependencies vcpkg_bootstrap (${vcpkg_bootstrap})")

    if(NOT EXISTS ${vcpkg_exec})
        message(STATUS "vcpkg_add_dependencies bootstraping")
        if(WIN32)
            execute_process(
                COMMAND cmd /C ${vcpkg_bootstrap}
                WORKING_DIRECTORY ${VCPKG_ROOT})
        else()
            execute_process(
                COMMAND ${vcpkg_bootstrap}
                WORKING_DIRECTORY ${VCPKG_ROOT})
        endif()
    endif()

    message(STATUS "vcpkg_add_dependencies install (${ARGN})")
    execute_process(
        COMMAND ${vcpkg_exec} install ${ARGN} --recurse #--triplet ${VCPKG_TARGET_TRIPLET}
        WORKING_DIRECTORY ${VCPKG_ROOT}
        )
endfunction()

