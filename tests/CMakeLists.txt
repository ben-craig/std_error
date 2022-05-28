# CMakeList.txt : CMake project for std_error, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20)


find_package(GTest CONFIG REQUIRED) 
add_executable (std_error_test)

target_link_libraries(std_error_test PRIVATE std_error GTest::gtest_main)

target_include_directories(std_error_test PUBLIC "../include")

if (MSVC)
    cmake_policy(SET CMP0092 NEW) # Do not add /W3 to CMAKE_<LANG>_FLAGS by default
    target_compile_options(std_error_test PRIVATE /W4 /WX /std:c++latest)
else() # todo name other compilers, don't else them
    target_compile_options(std_error_test PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

include(GoogleTest)
gtest_discover_tests(std_error_test)
add_test(check_std_error std_error_test)

target_sources(std_error_test PRIVATE "std_error/std_error.test.cpp" "std_error_next/std_error2.test.cpp")
