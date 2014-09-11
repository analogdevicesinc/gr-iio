/* -*- c++ -*- */
/* 
 * Copyright 2014 Analog Devices Inc.
 * Author: Paul Cercueil <paul.cercueil@analog.com>
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


#ifndef INCLUDED_IIO_FMCOMMS2_SINK_H
#define INCLUDED_IIO_FMCOMMS2_SINK_H

#include <gnuradio/iio/api.h>
#include <gnuradio/sync_block.h>

#include "device_sink.h"

namespace gr {
  namespace iio {

    class IIO_API fmcomms2_sink : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<fmcomms2_sink> sptr;

      static sptr make(const std::string &host,
		    double frequency, double samplerate, double bandwidth,
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    unsigned int buffer_size, bool cyclic,
		    const char *rf_port_select,
		    double attenuation1, double attenuation2);
    };

  } // namespace iio
} // namespace gr

#endif /* INCLUDED_IIO_FMCOMMS2_SINK_H */

