
function(vcpkg_install commit)
    if(NOT EXISTS ${VCPKG_ROOT})
        message("vcpkg install git clone to ${VCPKG_ROOT}")
        execute_process(
            COMMAND git clone https://github.com/microsoft/vcpkg.git ${VCPKG_ROOT})
    endif()

    if(DEFINED commit)
        message("vcpkg install git checkout ${commit}")
        execute_process(
            COMMAND git checkout ${commit}
            WORKING_DIRECTORY ${VCPKG_ROOT})
    endif()

    message("vcpkg install git pull")
    execute_process(
        COMMAND git pull
        WORKING_DIRECTORY ${VCPKG_ROOT})

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
    message("vcpkg add dependencies ${ARGN}")
    execute_process(
        COMMAND ${VCPKG_EXEC} install ${ARGN} --triplet ${VCPKG_TARGET_TRIPLET}
        WORKING_DIRECTORY ${VCPKG_ROOT}
        )
endfunction()

