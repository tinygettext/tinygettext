# tinygettext - A gettext replacement that works directly on .po files
# Copyright (C) 2006 Christoph Sommer <christoph.sommer@2006.expires.deltadevelopment.de>
#
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgement in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.

#
# INSTRUCTIONS:
# -------------
#
# Create a directory build/ and change to it. Run
#
#   cmake ..
#
# This creates a set of Makefiles to build the project. Run
#
#   make
#

cmake_minimum_required(VERSION 3.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/external/cmake-modules/)

cmake_policy(SET CMP0005 NEW)

## Project name to use as command prefix

project(tinygettext)
set(VERSION "0.1.0")

include(MaximumWarnings)

### CMake configuration

if(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
SET(CMAKE_MACOSX_RPATH ON)

# move some config clutter to the advanced section
mark_as_advanced(
	CMAKE_BACKWARDS_COMPATIBILITY
	CMAKE_BUILD_TYPE
	CMAKE_INSTALL_PREFIX
	EXECUTABLE_OUTPUT_PATH
	CMAKE_OSX_ARCHITECTURES
	CMAKE_OSX_SYSROOT
)

## Add iconv to include directories

find_package(Iconv REQUIRED)

## Check iconv_const

include(CheckCXXSourceCompiles)

set(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${Iconv_INCLUDE_DIRS})
check_cxx_source_compiles(
	"
	#include <iconv.h>
	// this declaration will fail when there already exists a non const char** version which returns size_t
	double iconv(iconv_t cd,  char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft);
	int main() { return 0; }
	"
	HAVE_ICONV_CONST
)

# TODO: better way of config

if(HAVE_ICONV_CONST)
  add_definitions(-DHAVE_ICONV_CONST)
else(HAVE_ICONV_CONST)
  remove_definitions(-DHAVE_ICONV_CONST)
endif(HAVE_ICONV_CONST)

## TinyGetText library compilation
file(GLOB TINYGETTEXT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
file(GLOB TINYGETTEXT_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/tinygettext/*.hpp)

add_library(tinygettext STATIC ${TINYGETTEXT_SOURCES})
set_target_properties(tinygettext PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)
target_include_directories(tinygettext PUBLIC include/)
target_compile_options(tinygettext PRIVATE ${WARNINGS_CXX_FLAGS})
target_compile_definitions(tinygettext PRIVATE VERSION=${VERSION})
if(WIN32)
  target_compile_definitions(tinygettext PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
target_link_libraries(tinygettext PUBLIC Iconv::Iconv)

## Build tinygettext tests
if(BUILD_TESTS)
  foreach(TEST tinygettext_test po_parser_test)
    add_executable(${TEST} test/${TEST}.cpp)
    set_target_properties(${TEST} PROPERTIES
      CXX_STANDARD 17
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF)
    target_compile_options(${TEST} PRIVATE ${WARNINGS_CXX_FLAGS})
    if(WIN32)
      target_compile_definitions(${TEST} PRIVATE _CRT_SECURE_NO_WARNINGS)
    endif()
    target_link_libraries(${TEST} PRIVATE tinygettext)
  endforeach(TEST)
endif()

## Install tinygettext

# use standardized variable name
set(LIB_SUBDIR "lib${LIB_SUFFIX}"
	CACHE STRING "Subdirectory of prefix into which libraries are installed (e.g., lib32, lib64)")

## prepare tinygettext.pc
configure_file(tinygettext.pc.in tinygettext.pc @ONLY)

install(TARGETS tinygettext
	ARCHIVE DESTINATION ${LIB_SUBDIR}
	LIBRARY DESTINATION ${LIB_SUBDIR})
install(FILES ${TINYGETTEXT_HEADERS}
	DESTINATION include/tinygettext)
install(FILES ${tinygettext_BINARY_DIR}/tinygettext.pc
	DESTINATION ${LIB_SUBDIR}/pkgconfig)

# EOF #
