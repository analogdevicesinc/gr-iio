/*
 * Copyright 2020 Free Software Foundation, Inc.
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
/* BINDTOOL_HEADER_FILE_HASH(0b4c045412e9e78eb7c9b6c5b161091d)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <iio/fmcomms5_sink.h>
// pydoc.h is automatically generated in the build directory
#include <fmcomms5_sink_pydoc.h>

void bind_fmcomms5_sink(py::module& m)
{

    using fmcomms5_sink    = ::gr::iio::fmcomms5_sink;
    using fmcomms5_sink_f32c    = ::gr::iio::fmcomms5_sink_f32c;


    py::class_<fmcomms5_sink, gr::sync_block, gr::block, gr::basic_block,
        std::shared_ptr<fmcomms5_sink>>(m, "fmcomms5_sink", D(fmcomms5_sink))

        .def(py::init(&fmcomms5_sink::make),
           py::arg("uri"),
           py::arg("frequency1"),
           py::arg("frequency2"),
           py::arg("samplerate"),
           py::arg("bandwidth"),
           py::arg("ch1_en"),
           py::arg("ch2_en"),
           py::arg("ch3_en"),
           py::arg("ch4_en"),
           py::arg("ch5_en"),
           py::arg("ch6_en"),
           py::arg("ch7_en"),
           py::arg("ch8_en"),
           py::arg("buffer_size"),
           py::arg("cyclic"),
           py::arg("rf_port_select"),
           py::arg("attenuation1"),
           py::arg("attenuation2"),
           py::arg("attenuation3"),
           py::arg("attenuation4"),
           py::arg("filter") = "",
           D(fmcomms5_sink,make)
        )
        




        .def_static("make_from",&fmcomms5_sink::make_from,
            py::arg("ctx"),
            py::arg("frequency1"),
            py::arg("frequency2"),
            py::arg("samplerate"),
            py::arg("bandwidth"),
            py::arg("ch1_en"),
            py::arg("ch2_en"),
            py::arg("ch3_en"),
            py::arg("ch4_en"),
            py::arg("ch5_en"),
            py::arg("ch6_en"),
            py::arg("ch7_en"),
            py::arg("ch8_en"),
            py::arg("buffer_size"),
            py::arg("cyclic"),
            py::arg("rf_port_select"),
            py::arg("attenuation1"),
            py::arg("attenuation2"),
            py::arg("attenuation3"),
            py::arg("attenuation4"),
            py::arg("filter") = "",
            D(fmcomms5_sink,make_from)
        )


        .def("set_params",&fmcomms5_sink::set_params,
            py::arg("frequency1"),
            py::arg("frequency2"),
            py::arg("samplerate"),
            py::arg("bandwidth"),
            py::arg("rf_port_select"),
            py::arg("attenuation1"),
            py::arg("attenuation2"),
            py::arg("attenuation3"),
            py::arg("attenuation4"),
            D(fmcomms5_sink,set_params)
        )

        ;


    py::class_<fmcomms5_sink_f32c, gr::hier_block2,
        std::shared_ptr<fmcomms5_sink_f32c>>(m, "fmcomms5_sink_f32c", D(fmcomms5_sink_f32c))

        .def(py::init(&fmcomms5_sink_f32c::make),
           py::arg("uri"),
           py::arg("frequency1"),
           py::arg("frequency2"),
           py::arg("samplerate"),
           py::arg("bandwidth"),
           py::arg("rx1_en"),
           py::arg("rx2_en"),
           py::arg("rx3_en"),
           py::arg("rx4_en"),
           py::arg("buffer_size"),
           py::arg("cyclic"),
           py::arg("rf_port_select"),
           py::arg("attenuation1"),
           py::arg("attenuation2"),
           py::arg("attenuation3"),
           py::arg("attenuation4"),
           py::arg("filter") = "",
           D(fmcomms5_sink_f32c,make)
        )
        




        .def("set_params",&fmcomms5_sink_f32c::set_params,
            py::arg("frequency1"),
            py::arg("frequency2"),
            py::arg("samplerate"),
            py::arg("bandwidth"),
            py::arg("rf_port_select"),
            py::arg("attenuation1"),
            py::arg("attenuation2"),
            py::arg("attenuation3"),
            py::arg("attenuation4"),
            D(fmcomms5_sink_f32c,set_params)
        )

        ;




}







