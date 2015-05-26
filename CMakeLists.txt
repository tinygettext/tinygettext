#
# TinyGetText build script
# Copyright (C) 2006 Christoph Sommer <christoph.sommer@2006.expires.deltadevelopment.de>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#


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


cmake_policy(SET CMP0005 NEW)

## Project name to use as command prefix

project(tinygettext)
set(VERSION "0.1")

### CMake configuration

cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${tinygettext_SOURCE_DIR})

# move some config clutter to the advanced section
mark_as_advanced(
	CMAKE_BACKWARDS_COMPATIBILITY
	CMAKE_BUILD_TYPE
	CMAKE_INSTALL_PREFIX
	EXECUTABLE_OUTPUT_PATH
	CMAKE_OSX_ARCHITECTURES
	CMAKE_OSX_SYSROOT
)

## Reveal library type choice to users
option(BUILD_SHARED_LIBS "Produce dynamic library instead of static archive" ON)

## Add iconv to include directories

find_package(ICONV REQUIRED)
include_directories(${ICONV_INCLUDE_DIR})

## Check iconv_const

include(CheckCXXSourceCompiles)

set(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${ICONV_INCLUDE_DIR})
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

## build list of source files

file(GLOB TINYGETTEXT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
file(GLOB TINYGETTEXT_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/tinygettext/*.hpp)

## define a target for building the library

add_library(tinygettext ${TINYGETTEXT_SOURCES})

## Add tinygettext dir to search path

include_directories(include/)

## Debug options

option(WERROR "Stops on first compiler warning in debug mode" OFF)
if(CMAKE_COMPILER_IS_GNUCC)
  add_definitions(-std=c++0x -O3 -Wall -Wextra -Weffc++ -pedantic)
  # -ansi fails in MinGW
  option(WARNINGS "Enable long list of warnings for compiler to check" ON)
  if(WARNINGS)
    add_definitions(
          -Wabi  -Wctor-dtor-privacy
          -Wstrict-null-sentinel
          -Wold-style-cast
          -Woverloaded-virtual
          -Wsign-promo -Wswitch-enum
          -Wcast-align  -Wcast-qual
          -Wdisabled-optimization
          -Wfloat-equal
          -Wformat=2
          -Winit-self
          -Winvalid-pch  -Wunsafe-loop-optimizations
          -Wlogical-op
          -Wmissing-format-attribute  -Wmissing-include-dirs -Wmissing-noreturn
          -Wpacked
          -Wredundant-decls
          -Wshadow
          -Wsign-conversion  -Wstack-protector
          -Wstrict-overflow=5
          -Wswitch-default  -Wswitch-enum
          -Wundef
          -Wconversion)
        # Still left:
        # -Wpadded      (DictionaryManager has a bool that sticks out)
  endif(WARNINGS)
  if(WERROR)
    add_definitions(-Werror)
  endif(WERROR)
endif(CMAKE_COMPILER_IS_GNUCC)

## Extra definitions

add_definitions(-DVERSION=${VERSION})

## Generate test executables in the right place

set(EXECUTABLE_OUTPUT_PATH ${tinygettext_BINARY_DIR}/test)

## Build tinygettext tests

foreach(TEST tinygettext_test po_parser_test)
  ## Add target for tinygettext test
  add_executable(${TEST} test/${TEST}.cpp)
  ## Link with tinygettext library
  target_link_libraries(${TEST} tinygettext)
  TARGET_LINK_LIBRARIES(${TEST} ${ICONV_LIBRARY})
endforeach(TEST)

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
