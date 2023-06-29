cmake_minimum_required(VERSION 3.10)
project(vcd_parser)

set(ANTLR4CPP_JAR_LOCATION ${PROJECT_SOURCE_DIR}/thirdparty/antlr-4.13.0-complete.jar)

add_executable(vcdp
    source/main.cpp
)

target_include_directories(vcdp
    PRIVATE headers
)

set(CMAKE_CXX_STANDARD 17)

