# Adapted from cmake file provided by blorente: https://github.com/blorente/antlr-4.7-cpp-cmake-base

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12.2)
PROJECT(antlr4cpp_fetcher CXX)
INCLUDE(ExternalProject)

FIND_PACKAGE(Git REQUIRED)
FIND_PACKAGE(Java COMPONENTS Runtime REQUIRED)

# external repository
set(ANTLR4CPP_EXTERNAL_REPO "https://github.com/antlr/antlr4")
set(ANTLR4CPP_EXTERNAL_TAG ${ANTLR4_VERSION})
set(ANTLR4CPP_EXTERNAL_ZIP ${ANTLR4CPP_EXTERNAL_REPO}/archive/refs/tags/${ANTLR4CPP_EXTERNAL_TAG}.zip)

if(NOT EXISTS "${ANTLR4CPP_JAR_LOCATION}")
  message(FATAL_ERROR "Unable to find antlr tool. ANTLR4CPP_JAR_LOCATION:${ANTLR4CPP_JAR_LOCATION}")
endif()

# default path for source files
if (NOT ANTLR4CPP_GENERATED_SRC_DIR)
  set(ANTLR4CPP_GENERATED_SRC_DIR ${CMAKE_BINARY_DIR}/antlr4cpp_generated_src)
endif()

# download runtime environment
ExternalProject_Add(
  antlr4cpp
  #For automatically fetching the zip
  URL                ${ANTLR4CPP_EXTERNAL_ZIP}
  #For offline builds
  #URL                my_local_download_path.zip
  TIMEOUT            10
  LOG_DOWNLOAD       ON
  CONFIGURE_COMMAND  ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release -DANTLR4CPP_JAR_LOCATION=${ANTLR4CPP_JAR_LOCATION} -DBUILD_SHARED_LIBS=ON -DBUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DCMAKE_SOURCE_DIR:PATH=<SOURCE_DIR>/runtime/Cpp <SOURCE_DIR>/runtime/Cpp
  LOG_CONFIGURE ON
  LOG_BUILD ON
  DOWNLOAD_EXTRACT_TIMESTAMP false
)

ExternalProject_Get_Property(antlr4cpp INSTALL_DIR)

list(APPEND ANTLR4CPP_INCLUDE_DIRS ${INSTALL_DIR}/include/antlr4-runtime)
foreach(src_path misc atn dfa tree support)
  list(APPEND ANTLR4CPP_INCLUDE_DIRS ${INSTALL_DIR}/include/antlr4-runtime/${src_path})
endforeach(src_path)

set(ANTLR4CPP_LIBS "${INSTALL_DIR}/lib")

############ Generate runtime #################
# macro to add dependencies to target
#
# Param 1 project name
# Param 1 namespace (postfix for dependencies)
# Param 2 Lexer file (full path)
# Param 3 Parser File (full path)
#
# output
#
# antlr4cpp_src_files_{namespace} - src files for add_executable
# antlr4cpp_include_dirs_{namespace} - include dir for generated headers
# antlr4cpp_generation_{namespace} - for add_dependencies tracking

macro(
  antlr4cpp_two_file_process
  antlr4cpp_project
  antlr4cpp_project_namespace
  antlr4cpp_grammar_lexer
  antlr4cpp_grammar_parser
  lexer_name
  parser_name
)
  set(ANTLRGEN ${ANTLR4CPP_GENERATED_SRC_DIR}/${antlr4cpp_project_namespace})

  if(EXISTS "${ANTLR4CPP_JAR_LOCATION}")
    message(STATUS "Found antlr tool: ${ANTLR4CPP_JAR_LOCATION}")
  else()
    message(FATAL_ERROR "Unable to find antlr tool. ANTLR4CPP_JAR_LOCATION:${ANTLR4CPP_JAR_LOCATION}")
  endif()

  #modify file list as appropriate
  set(ANTLRFILES
    ${ANTLRGEN}/${lexer_name}.cpp;
    ${ANTLRGEN}/${lexer_name}.h;
    ${ANTLRGEN}/${lexer_name}.interp;
    ${ANTLRGEN}/${lexer_name}.tokens;
    ${ANTLRGEN}/${parser_name}.cpp;
    ${ANTLRGEN}/${parser_name}.h;
    ${ANTLRGEN}/${parser_name}.interp;
    ${ANTLRGEN}/${parser_name}.tokens;
    ${ANTLRGEN}/${parser_name}BaseListener.cpp;
    ${ANTLRGEN}/${parser_name}BaseListener.h;
    ${ANTLRGEN}/${parser_name}BaseVisitor.cpp;
    ${ANTLRGEN}/${parser_name}BaseVisitor.h;
    ${ANTLRGEN}/${parser_name}Listener.cpp;
    ${ANTLRGEN}/${parser_name}Listener.h;
    ${ANTLRGEN}/${parser_name}Visitor.cpp;
    ${ANTLRGEN}/${parser_name}Visitor.h
  )

  add_custom_command(
    OUTPUT ${ANTLRFILES}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${ANTLR4CPP_GENERATED_SRC_DIR}
    COMMAND "${Java_JAVA_EXECUTABLE}" -jar "${ANTLR4CPP_JAR_LOCATION}" -Werror -Dlanguage=Cpp -listener -visitor -o "${ANTLRGEN}" -package ${antlr4cpp_project_namespace} "${antlr4cpp_grammar_lexer}" "${antlr4cpp_grammar_parser}"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    DEPENDS "${antlr4cpp_grammar_lexer}" "${antlr4cpp_grammar_parser}"
  )

  add_custom_target("antlr4cpp_generation_${antlr4cpp_project_namespace}"
    DEPENDS ${ANTLRFILES}
  )

  # Find all the input files
  FILE(GLOB generated_files ${ANTLRGEN}/*.cpp)

  # export generated cpp files into list
  foreach(generated_file ${generated_files})
    list(APPEND antlr4cpp_src_files_${antlr4cpp_project_namespace} ${generated_file})
    set_source_files_properties(
      ${generated_file}
      PROPERTIES
      COMPILE_FLAGS -Wno-overloaded-virtual
      )
  endforeach(generated_file)
  message(STATUS "Antlr4Cpp  ${antlr4cpp_project_namespace} Generated: ${generated_files}")

  # export generated include directory
  set(antlr4cpp_include_dirs_${antlr4cpp_project_namespace} ${ANTLR4CPP_GENERATED_SRC_DIR}/${antlr4cpp_project_namespace})
  message(STATUS "Antlr4Cpp ${antlr4cpp_project_namespace} include: ${ANTLR4CPP_GENERATED_SRC_DIR}/${antlr4cpp_project_namespace}")

endmacro()

macro(
  antlr4cpp_one_file_process
  antlr4cpp_project
  antlr4cpp_project_namespace
  antlr4cpp_grammar
  grammar_name
)
  set(ANTLRGEN ${ANTLR4CPP_GENERATED_SRC_DIR}/${antlr4cpp_project_namespace})

  if(EXISTS "${ANTLR4CPP_JAR_LOCATION}")
    message(STATUS "Found antlr tool: ${ANTLR4CPP_JAR_LOCATION}")
  else()
    message(FATAL_ERROR "Unable to find antlr tool. ANTLR4CPP_JAR_LOCATION:${ANTLR4CPP_JAR_LOCATION}")
  endif()

  #modify file list as appropriate
  set(ANTLRFILES
    ${ANTLRGEN}/${grammar_name}Parser.cpp;
    ${ANTLRGEN}/${grammar_name}Parser.h;
    ${ANTLRGEN}/${grammar_name}Lexer.cpp;
    ${ANTLRGEN}/${grammar_name}Lexer.h;
    ${ANTLRGEN}/${grammar_name}Lexer.interp;
    ${ANTLRGEN}/${grammar_name}Lexer.tokens;
    ${ANTLRGEN}/${grammar_name}BaseListener.cpp;
    ${ANTLRGEN}/${grammar_name}BaseListener.h;
    ${ANTLRGEN}/${grammar_name}BaseVisitor.cpp;
    ${ANTLRGEN}/${grammar_name}BaseVisitor.h;
    ${ANTLRGEN}/${grammar_name}Listener.cpp;
    ${ANTLRGEN}/${grammar_name}Listener.h;
    ${ANTLRGEN}/${grammar_name}Visitor.cpp;
    ${ANTLRGEN}/${grammar_name}Visitor.h
  )

  add_custom_command(
    OUTPUT ${ANTLRFILES}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${ANTLR4CPP_GENERATED_SRC_DIR}
    COMMAND "${Java_JAVA_EXECUTABLE}" -jar "${ANTLR4CPP_JAR_LOCATION}" -Werror -Dlanguage=Cpp -listener -visitor -o "${ANTLRGEN}" -package ${antlr4cpp_project_namespace} "${antlr4cpp_grammar}"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    DEPENDS "${antlr4cpp_grammar}"
  )

  add_custom_target("antlr4cpp_generation_${antlr4cpp_project_namespace}"
    DEPENDS ${ANTLRFILES}
  )

  # Find all the input files
  FILE(GLOB generated_files ${ANTLRGEN}/*.cpp)

  # export generated cpp files into list
  foreach(generated_file ${generated_files})
    list(APPEND antlr4cpp_src_files_${antlr4cpp_project_namespace} ${generated_file})
    set_source_files_properties(
      ${generated_file}
      PROPERTIES
      COMPILE_FLAGS -Wno-overloaded-virtual
      )
  endforeach(generated_file)
  message(STATUS "Antlr4Cpp  ${antlr4cpp_project_namespace} Generated: ${generated_files}")

  # export generated include directory
  set(antlr4cpp_include_dirs_${antlr4cpp_project_namespace} ${ANTLR4CPP_GENERATED_SRC_DIR}/${antlr4cpp_project_namespace})
  message(STATUS "Antlr4Cpp ${antlr4cpp_project_namespace} include: ${ANTLR4CPP_GENERATED_SRC_DIR}/${antlr4cpp_project_namespace}")

endmacro()