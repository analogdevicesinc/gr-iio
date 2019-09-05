/* -*- c++ -*- */
/*
 * Copyright 2018 Analog Devices Inc.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_IIO_ADF4350_SINK_IMPL_H
#define INCLUDED_IIO_ADF4350_SINK_IMPL_H

#include <gnuradio/iio/adf4350_sink.h>
#include <pmt/pmt.h>
#include "drivers/adf4350/adf4350.h"

namespace gr {
    namespace iio {
        class adf4350_sink_impl : public adf4350_sink
        {
        private:
            std::string uri;
            adf4350_dev *adf4350_device;

        public:
            adf4350_sink_impl(const std::string &uri);
            ~adf4350_sink_impl();

            void write_attribute(pmt::pmt_t pdu);

        };
    }
}


#endif //INCLUDED_IIO_ADF4350_SINK_IMPL_H
