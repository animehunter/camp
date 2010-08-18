###############################################################################
##
## Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
## Contact: Technogerma Systems France Information (contact@technogerma.fr)
##
## This file is part of the CAMP library.
##
## CAMP is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
## 
## CAMP is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public License
## along with CAMP.  If not, see <http://www.gnu.org/licenses/>.
##
###############################################################################


cmake_minimum_required(VERSION 2.6)

# set project's name
project(CAMP_LUA)

# all source files
set(CAMP_LUA_SRCS
    include/camp-lua/valuetoluavisitor.hpp
    src/valuetoluavisitor.cpp
    include/camp-lua/conversion.hpp
    src/conversion.cpp
    include/camp-lua/object.hpp
    src/object.cpp
    include/camp-lua/context.hpp
    src/context.cpp
)

# find Lua
find_package(Lua51 REQUIRED)

# include files search paths
include_directories(
    ${CAMP_LUA_SOURCE_DIR}/include
    ${LUA_INCLUDE_DIR}
)

# instruct CMake to build a shared library from all of the source files
add_library(camp-lua ${CAMP_LUA_SRCS})

# define the export macro
if(BUILD_SHARED_LIBS)
    set_target_properties(camp-lua PROPERTIES DEFINE_SYMBOL CAMP_EXPORTS)
else()
    add_definitions(-DCAMP_STATIC)
endif()

# define d suffix on windows
if(WIN32)
    set_target_properties(camp-lua PROPERTIES DEBUG_POSTFIX d)
endif()

# gcc 4.x on Windows requires the -static-libgcc linker option to get rid of an extra DLL
if (WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    if(${GCC_VERSION} MATCHES "4\\..*")
        set_target_properties(camp-lua PROPERTIES LINK_FLAGS -static-libgcc)
    endif()
endif()

# link
target_link_libraries(camp-lua camp ${LUA_LIBRARIES})

# add the test subdirectory, but do not build it by default
if(BUILD_TEST_LUA)
    add_subdirectory(test)
endif()

