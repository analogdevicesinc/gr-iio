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
	${CMAKE_CURRENT_SOURCE_DIR}/iio_math_parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)

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

add_library(gnuradio-iio SHARED ${iio_sources})
target_link_libraries(gnuradio-iio
	gnuradio::gnuradio-runtime
	gnuradio::gnuradio-blocks
	gnuradio::gnuradio-analog
	gnuradio::gnuradio-pmt
	gnuradio::gnuradio-filter
	gnuradio::gnuradio-fft
	${IIO_LIBRARIES}
	${AD9361_LIBRARIES})
target_include_directories(gnuradio-iio
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/>
    PUBLIC $<INSTALL_INTERFACE:include>
  )
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
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-iio)
