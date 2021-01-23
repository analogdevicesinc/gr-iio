# Copyright 2011-2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Setup library
########################################################################
include(GrPlatform)

list(APPEND iio_sources
    attr_sink_impl.cc
    attr_source_impl.cc
    attr_updater_impl.cc
    dds_control.cc
    device_sink_impl.cc
    device_source_impl.cc
)

set(iio_sources "${iio_sources}" PARENT_SCOPE)
if(NOT iio_sources)
    MESSAGE(STATUS "No C++ sources... skipping lib/")
    return()
endif(NOT iio_sources)

add_library(gnuradio-iio
    SHARED
    ${iio_sources}
)

target_link_libraries(gnuradio-iio
    PUBLIC
    gnuradio::gnuradio-runtime
    gnuradio-analog
    gnuradio-fft
    gnuradio-filter
    gnuradio-pmt
    ${AD9361_LIBRARIES}
    ${GR_VOLK_LIB}
    ${LIBIIO_LIBRARIES}
    PRIVATE
    gnuradio-blocks
)

########################################################################
# Begin conditional configuration
########################################################################
if(LIBAD9361_FOUND)

    target_sources(gnuradio-iio
        PRIVATE
        fmcomms2_sink_impl.cc
        fmcomms2_source_impl.cc
        fmcomms5_sink_impl.cc
        fmcomms5_source_impl.cc
        pluto_sink_impl.cc
        pluto_source_impl.cc
    )

    target_include_directories(gnuradio-iio
        PUBLIC
        ${LIBAD9361_INCLUDE_DIRS}
    )

    target_link_libraries(gnuradio-iio
        PUBLIC
        ${LIBAD9361_LIBRARIES}
    )

endif(LIBAD9361_FOUND)

target_include_directories(gnuradio-iio
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/>
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    PUBLIC $<INSTALL_INTERFACE:include>
)

set_target_properties(gnuradio-iio
    PROPERTIES
    DEFINE_SYMBOL
    "gnuradio_iio_EXPORTS"
)

if(APPLE)
    set_target_properties(gnuradio-iio
        PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-iio)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)
