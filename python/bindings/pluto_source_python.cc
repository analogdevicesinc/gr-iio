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
/* BINDTOOL_HEADER_FILE(pluto_source.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(c8c02874c3d470cd2bd0fd5cd427b77b)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <iio/pluto_source.h>
// pydoc.h is automatically generated in the build directory
#include <pluto_source_pydoc.h>

void bind_pluto_source(py::module& m)
{

    using pluto_source = ::gr::iio::pluto_source;


    py::class_<pluto_source,
        std::shared_ptr<pluto_source>>(m, "pluto_source", D(pluto_source))

        .def(py::init(&pluto_source::make),
             py::arg("uri"),
             py::arg("longfrequency"),
             py::arg("samplerate"),
             py::arg("bandwidth"),
             py::arg("buffer_size"),
             py::arg("quadrature"),
             py::arg("rfdc"),
             py::arg("bbdc"),
             py::arg("gain"),
             py::arg("gain_value"),
             py::arg("filter_source") = "",
             py::arg("filter_filename") = "",
             py::arg("Fpass") = 0.0,
             py::arg("Fstop") = 0.0,
             D(pluto_source, make))

        .def("set_params",
             &pluto_source::set_params,
             py::arg("longfrequency"),
             py::arg("samplerate"),
             py::arg("bandwidth"),
             py::arg("quadrature"),
             py::arg("rfdc"),
             py::arg("bbdc"),
             py::arg("gain"),
             py::arg("gain_value"),
             py::arg("filter_source") = "",
             py::arg("filter_filename") = "",
             py::arg("Fpass") = 0.0,
             py::arg("Fstop") = 0.0,
             D(pluto_source, set_params))

        ;
}
