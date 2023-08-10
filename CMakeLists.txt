cmake_minimum_required(VERSION 3.16)

include(CMakePrintHelpers)
cmake_print_variables(PROJECT_SOURCE_DIR)
cmake_print_variables($ENV{PATH})

# project name
project(f446re)

# select linker script
set(LINKER_SCRIPT "ld/LinkerScript.ld")

# define CPU OPTIONS
set(CPU_OPTIONS -mcpu=cortex-m4 -mthumb -mfloat-abi=soft)

# optimizations (-O0 -O1 -O2 -O3 -Os -Ofast -Og -flto)
set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 -DDEBUG")
#set(CMAKE_CXX_FLAGS_DEBUG "-Og -g")
set(CMAKE_C_FLAGS_RELEASE "-Os")#-flto")
#set(CMAKE_CXX_FLAGS_RELEASE "-Os -flto")

add_compile_options(
    ${CPU_OPTIONS}
    $<$<COMPILE_LANGUAGE:C>:-std=gnu11>
    $<$<COMPILE_LANGUAGE:CXX>:-std=c++17>
    $<$<COMPILE_LANGUAGE:CXX>:-fms-extensions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-use-cxa-atexit>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>
    -fstrict-volatile-bitfields
    -ffunction-sections
    #-fno-threadsafe-statics

    --specs=nano.specs
    
    #-Werror
    -Wall
    #-Wextra
    #-Wcast-align
    #-Wconversion
    #-Wsign-conversion
    #-Wold-style-cast
    #-Wshadow
    #-Wlogical-op
    #-Wsuggest-override
    #-Wsuggest-final-types
    #-Wsuggest-final-methods
    #-pedantic
)

include_directories(
    ${CMAKE_SOURCE_DIR}
)

link_directories(
    ${CMAKE_SOURCE_DIR}
)

# list of modules to build final firmware
add_executable(${PROJECT_NAME}
    src/main.c
)

target_link_libraries(${PROJECT_NAME}
    ${CPU_OPTIONS}
    -T${LINKER_SCRIPT}
    -Wl,-Map=${PROJECT_NAME}.map
    --specs=nosys.specs
    -nostartfiles  
    m
    # -nostdlib
)

set_property(TARGET ${PROJECT_NAME} PROPERTY LINK_DEPENDS ${CMAKE_SOURCE_DIR}/${LINKER_SCRIPT})

include("cmake/flash.cmake")
