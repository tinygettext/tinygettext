#
# TinyGetText - library build script
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

## Add to include directories

FIND_PACKAGE(ICONV REQUIRED)
INCLUDE_DIRECTORIES(${ICONV_INCLUDE_DIR})

## build list of source files

FILE(GLOB TINYGETTEXT_SOURCES *.cpp)

## define a target for building the library

ADD_LIBRARY(tinygettext ${TINYGETTEXT_SOURCES})

TARGET_LINK_LIBRARIES(tinygettext ${ICONV_LIBRARY})
