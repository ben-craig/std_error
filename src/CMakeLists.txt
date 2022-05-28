# CMakeList.txt : CMake project for std_error, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20)

# Add source to this project's executable.
add_library (std_error STATIC 
    "std_error/std_error.cpp"
    "std_error_next/std_error2.cpp"
    "../include/std_error/std_error.h"
    "../include/std_error_next/std_error2.h")

target_include_directories(std_error PUBLIC "../include")

if (MSVC)
    cmake_policy(SET CMP0092 NEW) # Do not add /W3 to CMAKE_<LANG>_FLAGS by default
    target_compile_options(std_error PRIVATE /W4 /WX /std:c++latest)
else() # todo name other compilers, don't else them
    target_compile_options(std_error PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

# TODO: Add tests and install targets if needed.


