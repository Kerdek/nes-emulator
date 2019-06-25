cmake_minimum_required(VERSION 3.9)

project(nes-emulator VERSION 1.0.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

######################
#    Dependencies    #
######################

find_package(fmt REQUIRED)
find_package(SDL2 REQUIRED)

include_directories("include")
include_directories("lib/include")

######################
#    Source files    #
######################

file(GLOB SOURCE_FILES
    "src/*.cpp"
    "src/mappers/*.cpp"
)

######################
#     Executable     #
######################

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

######################
#   Compiler flags   #
######################

set(GNU_FLAGS
    -Werror
    -Wall
    -Wextra
    -pedantic
    -Wuseless-cast
    -Wformat=2
    -Wdisabled-optimization
)

set(MSVC_FLAGS
    /WX
    /W4
    /permissive-
    /diagnostics:caret
    /Zi
    -w
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_compile_options(${PROJECT_NAME} PRIVATE
    # This keeps enabling on Linux
    # $<$<BOOL:MSVC>:${MSVC_FLAGS}>
    $<$<CXX_COMPILER_ID:GNU>:${GNU_FLAGS}>
)

if (MSVC)
    string(REGEX REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif()

##################
# Link libraries #
##################

if(UNIX)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        # SDL freaks out when this is on
        # target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined -fno-sanitize=shift)
        # target_link_libraries(${PROJECT_NAME} asan ubsan)
    endif()
endif()

target_link_libraries(${PROJECT_NAME} fmt SDL2)
