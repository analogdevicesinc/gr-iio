/*
 * Copyright 2021 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(fmcomms5_sink.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(fcff2d8db704e2781c85c54c3f3d6094)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <iio/fmcomms5_sink.h>
// pydoc.h is automatically generated in the build directory
#include <fmcomms5_sink_f32c_pydoc.h>

void bind_fmcomms5_sink_f32c(py::module& m)
{

    using fmcomms5_sink_f32c = ::gr::iio::fmcomms5_sink_f32c;


    py::class_<fmcomms5_sink_f32c,
               gr::hier_block2,
               gr::basic_block,
               std::shared_ptr<fmcomms5_sink_f32c>>(m, "fmcomms5_sink_f32c", D(fmcomms5_sink_f32c))

        .def(py::init(&fmcomms5_sink_f32c::make),
             py::arg("uri"),
             py::arg("longfrequency1"),
             py::arg("longfrequency2"),
             py::arg("samplerate"),
             py::arg("bandwidth"),
             py::arg("tx1_en"),
             py::arg("tx2_en"),
             py::arg("tx3_en"),
             py::arg("tx4_en"),
             py::arg("buffer_size"),
             py::arg("cyclic"),
             py::arg("rf_port_select"),
             py::arg("attenuation1"),
             py::arg("attenuation2"),
             py::arg("attenuation3"),
             py::arg("attenuation4"),
             py::arg("filter_source") = "",
             py::arg("filter_filename") = "",
             py::arg("Fpass") = 0.0,
             py::arg("Fstop") = 0.0,
             D(fmcomms5_sink_f32c, make))

        .def("set_params",
             &fmcomms5_sink_f32c::set_params,
             py::arg("longfrequency1"),
             py::arg("longfrequency2"),
             py::arg("samplerate"),
             py::arg("bandwidth"),
             py::arg("rf_port_select"),
             py::arg("attenuation1"),
             py::arg("attenuation2"),
             py::arg("attenuation3"),
             py::arg("attenuation4"),
             py::arg("filter_source") = "",
             py::arg("filter_filename") = "",
             py::arg("Fpass") = 0.0,
             py::arg("Fstop") = 0.0,
             D(fmcomms5_sink_f32c, set_params))

        ;
}
