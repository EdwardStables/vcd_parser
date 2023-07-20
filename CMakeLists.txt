cmake_minimum_required(VERSION 3.10)
project(vcd_parser)
set(CMAKE_CXX_STANDARD 17)

# ANTLR
set(ANTLR4_VERSION 4.13.0)

set(ANTLR4CPP_JAR_LOCATION ${PROJECT_SOURCE_DIR}/antlr-${ANTLR4_VERSION}-complete.jar)
include(Antlr4Cpp.cmake)
include_directories( ${ANTLR4CPP_INCLUDE_DIRS} )
link_directories( ${ANTLR4CPP_LIBS} )
message(STATUS "Found antlr4cpp libs: ${ANTLR4CPP_LIBS} and includes: ${ANTLR4CPP_INCLUDE_DIRS} ")

set(ANTLR4NAMESPACE antlrvcdp)

antlr4cpp_one_file_process(vcdp ${ANTLR4NAMESPACE}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/vcd.g4
  vcd
)

add_executable(vcdp
    src/main.cpp
    src/parser.cpp
    src/store.cpp
    src/util.cpp
    ${antlr4cpp_src_files_${ANTLR4NAMESPACE}}
)

target_include_directories( vcdp
  PRIVATE headers
  PRIVATE build/antlr4cpp_generated_src/${ANTLR4NAMESPACE}
)
add_dependencies(vcdp
  antlr4cpp
  antlr4cpp_generation_${ANTLR4NAMESPACE}
)
target_link_libraries(vcdp antlr4-runtime)

