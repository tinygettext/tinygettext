#
# TinyGetText - root build script
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


## Project name to use as command prefix

PROJECT(TINYGETTEXT)


### CMake configuration

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

# move some config clutter to the advanced section
MARK_AS_ADVANCED(
	CMAKE_BACKWARDS_COMPATIBILITY
	CMAKE_BUILD_TYPE
	CMAKE_INSTALL_PREFIX
	EXECUTABLE_OUTPUT_PATH
	LIBRARY_OUTPUT_PATH
	CMAKE_OSX_ARCHITECTURES
	CMAKE_OSX_SYSROOT
)

## Check endianess

INCLUDE(TestBigEndian)
TEST_BIG_ENDIAN(WORDS_BIGENDIAN)

## Check platform-dependent build options

INCLUDE(CheckCXXSourceCompiles)
INCLUDE(CheckTypeSize)

check_cxx_source_compiles(
	"
	#include <iconv.h>
	// this declaration will fail when there already exists a non const char** version which returns size_t
	double iconv(iconv_t cd,  char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft);
	int main() { return 0; }
	"
	HAVE_ICONV_CONST
)

CHECK_TYPE_SIZE("void*" SIZEOF_VOID_P)

MESSAGE(STATUS "Size of void* is ${SIZEOF_VOID_P}")

## TinyGetText library compilation

ADD_SUBDIRECTORY(src)

## Add tinygettext dir to search path

LINK_DIRECTORIES(src)
INCLUDE_DIRECTORIES(${TINYGETTEXT_SOURCE_DIR}/src)

## Debug options

OPTION(WERROR "Stops on first compiler warning in debug mode" ON)
IF(CMAKE_COMPILER_IS_GNUCC)
  ADD_DEFINITIONS(-Wall -Wextra)
  IF(WERROR)
    ADD_DEFINITIONS(-Werror)
  ENDIF(WERROR)
  ADD_DEFINITIONS(-fno-strict-aliasing)
ENDIF(CMAKE_COMPILER_IS_GNUCC)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  ADD_DEFINITIONS(-DMACOSX)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


## Generate executable in the right place

SET(EXECUTABLE_OUTPUT_PATH ${TINYGETTEXT_SOURCE_DIR}/test)

## Build list of sources for tinygettext test

FILE(GLOB TINYTEST_SOURCES RELATIVE ${TINYGETTEXT_SOURCE_DIR} test/*.cpp)

## Add target for tinygettext test

ADD_EXECUTABLE(tinytest ${TINYTEST_SOURCES})

## Link tinygettext binary with tinygettext and other libraries

TARGET_LINK_LIBRARIES(tinytest tinygettext)
