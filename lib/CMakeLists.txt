# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR} ${IIO_INCLUDE_DIRS} ${AD9361_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

include(FindFLEX REQUIRED)
find_package(BISON "3.0.2" REQUIRED)

flex_target(lexer
	${CMAKE_CURRENT_SOURCE_DIR}/iio_math_lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
bison_target(parser
	${CMAKE_CURRENT_SOURCE_DIR}/iio_math_parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)

list(APPEND iio_sources
	device_source_impl.cc
	device_sink_impl.cc
	fmcomms2_source_impl.cc
	fmcomms2_sink_impl.cc
	fmcomms5_source_impl.cc
	fmcomms5_sink_impl.cc
	pluto_source_impl.cc
	pluto_sink_impl.cc
	attr_sink_impl.cc
	attr_source_impl.cc
	converter_ss_impl.cc
	iio_math_impl.cc
	iio_math_gen_impl.cc
	iio_power_ff_impl.cc
	iio_modulo_ff_impl.cc
	iio_modulo_const_ff_impl.cc
	${FLEX_lexer_OUTPUTS}
	${BISON_parser_OUTPUTS}
)

set(iio_sources "${iio_sources}" PARENT_SCOPE)
if(NOT iio_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT iio_sources)

if ("${PC_GNURADIO_RUNTIME_VERSION}" VERSION_LESS 3.8.0)
	set(GR_IS_VERSION_3_7_OR_LESS ON)
endif()

option(COMPAT_GR_3_7_OR_LESS "Compile for GNU Radio version 3.7 or less" ${GR_IS_VERSION_3_7_OR_LESS})
if (COMPAT_GR_3_7_OR_LESS)
	add_definitions(-DGR_VERSION_3_7_OR_LESS)
endif()

add_library(gnuradio-iio SHARED ${iio_sources})
target_link_libraries(gnuradio-iio
	${Boost_LIBRARIES}
	${IIO_LIBRARIES}
	${AD9361_LIBRARIES}
	${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-iio PROPERTIES
	VERSION ${GR_IIO_VERSION}
	SOVERSION ${GR_IIO_VERSION_MAJOR}
	DEFINE_SYMBOL "gnuradio_iio_EXPORTS"
)

if(APPLE)
    set_target_properties(gnuradio-iio PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-iio
	LIBRARY DESTINATION ${GR_LIBRARY_DIR} # .so/.dylib file
	ARCHIVE DESTINATION ${GR_LIBRARY_DIR} # .lib file
	RUNTIME DESTINATION ${GR_RUNTIME_DIR} # .dll file
)


########################################################################
# Setup the include and linker paths
########################################################################
#include_directories(
#	${GR_IIO_INCLUDE_DIRS}
#   ${GNURADIO_RUNTIME_INCLUDE_DIRS}
#   ${Boost_INCLUDE_DIRS}
