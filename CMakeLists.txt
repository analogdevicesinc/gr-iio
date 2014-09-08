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
# Setup dependencies
########################################################################
include(GrBoost)

PKG_CHECK_MODULES(IIO libiio)

########################################################################
# Register component
########################################################################
include(GrComponent)
GR_REGISTER_COMPONENT("gr-iio" ENABLE_GR_IIO
    Boost_FOUND
    ENABLE_GNURADIO_RUNTIME
    ENABLE_GR_BLOCKS
	IIO_FOUND
)

GR_SET_GLOBAL(GR_IIO_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

GR_SET_GLOBAL(GR_IIO_SWIG_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/swig
)

########################################################################
# Begin conditional configuration
########################################################################
if (ENABLE_GR_IIO)

########################################################################
# Setup CPack components
########################################################################

include(GrPackage)
CPACK_SET(CPACK_COMPONENT_GROUP_IIO_DESCRIPTION "GNU Radio IIO Blocks")

CPACK_COMPONENT("iio_runtime"
	GROUP        "IIO"
    DISPLAY_NAME "Runtime"
    DESCRIPTION  "Runtime"
    DEPENDS      "runtime_runtime"
)

CPACK_COMPONENT("iio_devel"
	GROUP        "IIO"
    DISPLAY_NAME "Development"
    DESCRIPTION  "C++ headers, package config, import libraries"
    DEPENDS      "runtime_devel"
)

CPACK_COMPONENT("iio_python"
	GROUP        "IIO"
    DISPLAY_NAME "Python"
    DESCRIPTION  "Python modules for runtime; GRC xml files"
    DEPENDS      "runtime_python;iio_runtime"
)

CPACK_COMPONENT("iio_swig"
	GROUP        "IIO"
    DISPLAY_NAME "SWIG"
    DESCRIPTION  "SWIG development .i files"
    DEPENDS      "runtime_swig;iio_python;iio_devel"
)

########################################################################
# Add subdirectories
########################################################################
add_subdirectory(include/gnuradio/iio)
add_subdirectory(lib)
if(ENABLE_PYTHON)
    add_subdirectory(swig)
	add_subdirectory(python/iio)
    add_subdirectory(grc)
endif(ENABLE_PYTHON)

########################################################################
# Create Pkg Config File
########################################################################
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/gnuradio-iio.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/gnuradio-iio.pc
@ONLY)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/gnuradio-iio.pc
    DESTINATION ${GR_LIBRARY_DIR}/pkgconfig
    COMPONENT "iio_devel"
)

endif(ENABLE_GR_IIO)
