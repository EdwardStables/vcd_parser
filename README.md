# VCDP

A VCD parser and utility library using Antlr4.

## Building

You must download the antlr complete jar file from https://www.antlr.org/download.html and place it in the root of the repository. The cmake file expects it to be named `antlr-<version>-complete.jar`. `<version>` is set in `CMakeLists.txt`, and defaults to 4.13.0.

CMake will attempt to download the source zip file from github for the specified version. If you need an offline build then download the zip yourself and modify the `ExternalProject_Add` command in `Antlr4Cpp.cmake`.