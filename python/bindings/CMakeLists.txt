# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND iio_python_files
    attr_sink_python.cc
    attr_source_python.cc
    converter_ss_python.cc
    device_sink_python.cc
    device_source_python.cc
    fmcomms2_sink_python.cc
    fmcomms2_source_python.cc
    fmcomms5_sink_python.cc
    fmcomms5_source_python.cc
    math_python.cc
    modulo_const_ff_python.cc
    modulo_ff_python.cc
    pluto_sink_python.cc
    pluto_source_python.cc
    power_ff_python.cc
    python_bindings.cc)

GR_PYBIND_MAKE_OOT(iio 
   ../..
   gr::iio
   "${iio_python_files}")

install(TARGETS iio_python DESTINATION ${GR_PYTHON_DIR}/iio COMPONENT pythonapi)
