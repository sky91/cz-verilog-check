cmake_minimum_required(VERSION 3.14)
project(cz-verilog-check)

set(CMAKE_CXX_STANDARD 17)

add_executable(cz-verilog-check main.cpp)

if (${MINGW})
    target_link_options(cz-verilog-check
        PUBLIC $<$<CXX_COMPILER_ID:GNU>:-static-libgcc -static-libstdc++ -static>)
endif ()
target_link_options(cz-verilog-check PUBLIC $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU>>:-s>)