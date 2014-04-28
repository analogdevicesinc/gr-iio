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
# Setup swig generation
########################################################################
include(GrPython)
include(GrSwig)

set(GR_SWIG_INCLUDE_DIRS
	${GR_IIO_INCLUDE_DIRS}
    ${GNURADIO_RUNTIME_SWIG_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

if(ENABLE_GR_CTRLPORT)
  list(APPEND GR_SWIG_FLAGS "-DGR_CTRLPORT")
  list(APPEND GR_SWIG_INCLUDE_DIRS ${ICE_INCLUDE_DIR})
endif(ENABLE_GR_CTRLPORT)

# Setup swig docs to depend on includes and pull in from build directory
set(GR_SWIG_TARGET_DEPS iio_generated_includes)

set(GR_SWIG_LIBRARIES gnuradio-iio)

GR_SWIG_MAKE(iio_swig iio_swig.i)

GR_SWIG_INSTALL(
    TARGETS iio_swig
    DESTINATION ${GR_PYTHON_DIR}/gnuradio/iio
    COMPONENT "iio_python"
)

install(
    FILES
    iio_swig.i
    DESTINATION ${GR_INCLUDE_DIR}/gnuradio/swig
    COMPONENT "iio_swig"
)
