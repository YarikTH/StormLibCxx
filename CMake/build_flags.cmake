# ==========================================
# This cmake is renamed ucm.cmake 
# that purpose is only work with build flags
# original copyright is below
# ==========================================
#
# ucm.cmake - useful cmake macros
#
# Copyright (c) 2016 Viktor Kirilov
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# https://opensource.org/licenses/MIT
#
# The documentation can be found at the library's page:
# https://github.com/onqtam/ucm

cmake_minimum_required(VERSION 2.8.12)

include(CMakeParseArguments)

# bf_add_flags
# Adds compiler flags to CMAKE_<LANG>_FLAGS or to a specific config
macro(bf_add_flags)
    cmake_parse_arguments(ARG "C;CXX;CLEAR_OLD" "" "CONFIG" ${ARGN})

    if(NOT ARG_CONFIG)
        set(ARG_CONFIG " ")
    endif()

    foreach(CONFIG ${ARG_CONFIG})
        # determine to which flags to add
        if(NOT ${CONFIG} STREQUAL " ")
            string(TOUPPER ${CONFIG} CONFIG)
            set(CXX_FLAGS CMAKE_CXX_FLAGS_${CONFIG})
            set(C_FLAGS CMAKE_C_FLAGS_${CONFIG})
        else()
            set(CXX_FLAGS CMAKE_CXX_FLAGS)
            set(C_FLAGS CMAKE_C_FLAGS)
        endif()

        # clear the old flags
        if(${ARG_CLEAR_OLD})
            if("${ARG_CXX}" OR NOT "${ARG_C}")
                set(${CXX_FLAGS} "")
            endif()
            if("${ARG_C}" OR NOT "${ARG_CXX}")
                set(${C_FLAGS} "")
            endif()
        endif()

        # add all the passed flags
        foreach(flag ${ARG_UNPARSED_ARGUMENTS})
            if("${ARG_CXX}" OR NOT "${ARG_C}")
                set(${CXX_FLAGS} "${${CXX_FLAGS}} ${flag}")
            endif()
            if("${ARG_C}" OR NOT "${ARG_CXX}")
                set(${C_FLAGS} "${${C_FLAGS}} ${flag}")
            endif()
        endforeach()
    endforeach()

endmacro()

# bf_set_flags
# Sets the CMAKE_<LANG>_FLAGS compiler flags or for a specific config
macro(bf_set_flags)
    bf_add_flags(CLEAR_OLD ${ARGN})
endmacro()

# bf_add_linker_flags
# Adds linker flags to CMAKE_<TYPE>_LINKER_FLAGS or to a specific config
macro(bf_add_linker_flags)
    cmake_parse_arguments(ARG "CLEAR_OLD;EXE;MODULE;SHARED;STATIC" "" "CONFIG" ${ARGN})

    if(NOT ARG_CONFIG)
        set(ARG_CONFIG " ")
    endif()

    foreach(CONFIG ${ARG_CONFIG})
        string(TOUPPER "${CONFIG}" CONFIG)
    
        if(NOT ${ARG_EXE} AND NOT ${ARG_MODULE} AND NOT ${ARG_SHARED} AND NOT ${ARG_STATIC})
            set(ARG_EXE 1)
            set(ARG_MODULE 1)
            set(ARG_SHARED 1)
            set(ARG_STATIC 1)
        endif()
    
        set(flags_configs "")
        if(${ARG_EXE})
            if(NOT "${CONFIG}" STREQUAL " ")
                list(APPEND flags_configs CMAKE_EXE_LINKER_FLAGS_${CONFIG})
            else()
                list(APPEND flags_configs CMAKE_EXE_LINKER_FLAGS)
            endif()
        endif()
        if(${ARG_MODULE})
            if(NOT "${CONFIG}" STREQUAL " ")
                list(APPEND flags_configs CMAKE_MODULE_LINKER_FLAGS_${CONFIG})
            else()
                list(APPEND flags_configs CMAKE_MODULE_LINKER_FLAGS)
            endif()
        endif()
        if(${ARG_SHARED})
            if(NOT "${CONFIG}" STREQUAL " ")
                list(APPEND flags_configs CMAKE_SHARED_LINKER_FLAGS_${CONFIG})
            else()
                list(APPEND flags_configs CMAKE_SHARED_LINKER_FLAGS)
            endif()
        endif()
        if(${ARG_STATIC})
            if(NOT "${CONFIG}" STREQUAL " ")
                list(APPEND flags_configs CMAKE_STATIC_LINKER_FLAGS_${CONFIG})
            else()
                list(APPEND flags_configs CMAKE_STATIC_LINKER_FLAGS)
            endif()
        endif()
    
        # clear the old flags
        if(${ARG_CLEAR_OLD})
            foreach(flags ${flags_configs})
                set(${flags} "")
            endforeach()
        endif()

        # add all the passed flags
        foreach(flag ${ARG_UNPARSED_ARGUMENTS})
            foreach(flags ${flags_configs})
                set(${flags} "${${flags}} ${flag}")
            endforeach()
        endforeach()
    endforeach()
endmacro()

# bf_set_linker_flags
# Sets the CMAKE_<TYPE>_LINKER_FLAGS linker flags or for a specific config
macro(bf_set_linker_flags)
    bf_add_linker_flags(CLEAR_OLD ${ARGN})
endmacro()

# bf_gather_flags
# Gathers all lists of flags for printing or manipulation
macro(bf_gather_flags with_linker result)
    set(${result} "")
    # add the main flags without a config
    list(APPEND ${result} CMAKE_C_FLAGS)
    list(APPEND ${result} CMAKE_CXX_FLAGS)
    if(${with_linker})
        list(APPEND ${result} CMAKE_EXE_LINKER_FLAGS)
        list(APPEND ${result} CMAKE_MODULE_LINKER_FLAGS)
        list(APPEND ${result} CMAKE_SHARED_LINKER_FLAGS)
        list(APPEND ${result} CMAKE_STATIC_LINKER_FLAGS)
    endif()
    
    if("${CMAKE_CONFIGURATION_TYPES}" STREQUAL "" AND NOT "${CMAKE_BUILD_TYPE}" STREQUAL "")
        # handle single config generators - like makefiles/ninja - when CMAKE_BUILD_TYPE is set
        string(TOUPPER ${CMAKE_BUILD_TYPE} config)
        list(APPEND ${result} CMAKE_C_FLAGS_${config})
        list(APPEND ${result} CMAKE_CXX_FLAGS_${config})
        if(${with_linker})
            list(APPEND ${result} CMAKE_EXE_LINKER_FLAGS_${config})
            list(APPEND ${result} CMAKE_MODULE_LINKER_FLAGS_${config})
            list(APPEND ${result} CMAKE_SHARED_LINKER_FLAGS_${config})
            list(APPEND ${result} CMAKE_STATIC_LINKER_FLAGS_${config})
        endif()
    else()
        # handle multi config generators (like msvc, xcode)
        foreach(config ${CMAKE_CONFIGURATION_TYPES})
            string(TOUPPER ${config} config)
            list(APPEND ${result} CMAKE_C_FLAGS_${config})
            list(APPEND ${result} CMAKE_CXX_FLAGS_${config})
            if(${with_linker})
                list(APPEND ${result} CMAKE_EXE_LINKER_FLAGS_${config})
                list(APPEND ${result} CMAKE_MODULE_LINKER_FLAGS_${config})
                list(APPEND ${result} CMAKE_SHARED_LINKER_FLAGS_${config})
                list(APPEND ${result} CMAKE_STATIC_LINKER_FLAGS_${config})
            endif()
        endforeach()
    endif()
endmacro()

# bf_set_runtime
# Sets the runtime (static/dynamic) for msvc/gcc
macro(bf_set_runtime)
    cmake_parse_arguments(ARG "STATIC;DYNAMIC" "" "" ${ARGN})

    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "unrecognized arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()
    
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" STREQUAL "")
        message(AUTHOR_WARNING "bf_set_runtime() does not support clang yet!")
    endif()
    
    bf_gather_flags(0 flags_configs)
    
    # add/replace the flags
    # note that if the user has messed with the flags directly this function might fail
    # - for example if with MSVC and the user has removed the flags - here we just switch/replace them
    if("${ARG_STATIC}")
        foreach(flags ${flags_configs})
            if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
                if(NOT ${flags} MATCHES "-static-libstdc\\+\\+")
                    set(${flags} "${${flags}} -static-libstdc++")
                endif()
                if(NOT ${flags} MATCHES "-static-libgcc")
                    set(${flags} "${${flags}} -static-libgcc")
                endif()
            elseif(MSVC)
                if(${flags} MATCHES "/MD")
                    string(REGEX REPLACE "/MD" "/MT" ${flags} "${${flags}}")
                endif()
            endif()
        endforeach()
    elseif("${ARG_DYNAMIC}")
        foreach(flags ${flags_configs})
            if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
                if(${flags} MATCHES "-static-libstdc\\+\\+")
                    string(REGEX REPLACE "-static-libstdc\\+\\+" "" ${flags} "${${flags}}")
                endif()
                if(${flags} MATCHES "-static-libgcc")
                    string(REGEX REPLACE "-static-libgcc" "" ${flags} "${${flags}}")
                endif()
            elseif(MSVC)
                if(${flags} MATCHES "/MT")
                    string(REGEX REPLACE "/MT" "/MD" ${flags} "${${flags}}")
                endif()
            endif()
        endforeach()
    endif()
endmacro()

# bf_print_flags
# Prints all compiler flags for all configurations
macro(bf_print_flags)
    bf_gather_flags(1 flags_configs)
    message("")
    foreach(flags ${flags_configs})
        message("${flags}: ${${flags}}")
    endforeach()
    message("")
endmacro()