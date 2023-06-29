cmake_minimum_required(VERSION 3.10)
project(vcd_parser)
set(CMAKE_CXX_STANDARD 17)

# ANTLR #
LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(ANTLR4CPP_JAR_LOCATION ${PROJECT_SOURCE_DIR}/thirdparty/antlr/antlr-4.13.0-complete.jar)
include( ExternalAntlr4Cpp )
include_directories( ${ANTLR4CPP_INCLUDE_DIRS} )
link_directories( ${ANTLR4CPP_LIBS} )
message(STATUS "Found antlr4cpp libs: ${ANTLR4CPP_LIBS} and includes: ${ANTLR4CPP_INCLUDE_DIRS} ")

antlr4cpp_process_grammar(demo antlrcpptest
  ${CMAKE_CURRENT_SOURCE_DIR}/src/VCDLexer.g4
  ${CMAKE_CURRENT_SOURCE_DIR}/src/VCDParser.g4)
# include generated files in project environment
include_directories(${antlr4cpp_include_dirs_antlrcpp})

add_executable(vcdp
    src/main.cpp
    ${antlr4cpp_include_dirs_antlrcpp}
)

target_include_directories(vcdp PRIVATE headers)
add_dependencies(vcdp antlr4cpp antlr4cpp_generation_antlrcpptest)
target_link_libraries(vcdp antlr4-runtime)

