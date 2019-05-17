# Copyright 2011 Free Software Foundation, Inc.
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
# Check if there is C++ code at all
########################################################################
if(NOT iio_sources)
	MESSAGE(STATUS "No C++ sources... skipping swig/")
	return()
endif(NOT iio_sources)

########################################################################
# Include swig generation macros
########################################################################
find_package(SWIG)
find_package(PythonLibs)
if(NOT SWIG_FOUND OR NOT PYTHONLIBS_FOUND)
    return()
endif()
include(GrSwig)
include(GrPython)

########################################################################
# Setup swig generation
########################################################################
set(GR_SWIG_INCLUDE_DIRS $<TARGET_PROPERTY:gnuradio::runtime_swig,INTERFACE_INCLUDE_DIRECTORIES>)
set(GR_SWIG_TARGET_DEPS gnuradio::runtime_swig)

set(GR_SWIG_LIBRARIES gnuradio-iio)

GR_SWIG_MAKE(iio_swig iio_swig.i)
GR_SWIG_MAKE(iio_pluto_source_swig iio_pluto_source_swig.i)
GR_SWIG_MAKE(iio_pluto_sink_swig iio_pluto_sink_swig.i)

########################################################################
# Install the build swig module
########################################################################
GR_SWIG_INSTALL(TARGETS iio_swig iio_pluto_source_swig iio_pluto_sink_swig
	DESTINATION ${GR_PYTHON_DIR}/iio)

########################################################################
# Install swig .i files for development
########################################################################
install(
    FILES
    iio_swig.i
    iio_pluto_source_swig.i
    iio_pluto_sink_swig.i
    DESTINATION ${GR_INCLUDE_DIR}/iio/swig
)
