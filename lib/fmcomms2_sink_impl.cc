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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cstdio>

#include <gnuradio/io_signature.h>
#include "fmcomms2_sink_impl.h"

namespace gr {
  namespace iio {

    fmcomms2_sink::sptr
    fmcomms2_sink::make(const std::string &host,
		    double frequency, double samplerate, double bandwidth,
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    unsigned int buffer_size, bool cyclic,
		    const char *rf_port_select,
		    double attenuation1, double attenuation2)
    {
      return gnuradio::get_initial_sptr(
	    new fmcomms2_sink_impl(host, frequency,
		    samplerate, bandwidth, ch1_en, ch2_en, ch3_en, ch4_en,
		    buffer_size, cyclic, rf_port_select,
		    attenuation1, attenuation2));
    }

    std::vector<std::string> fmcomms2_sink_impl::get_channels_vector(
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en)
    {
	    std::vector<std::string> channels;
	    if (ch1_en)
		    channels.push_back("voltage0");
	    if (ch2_en)
		    channels.push_back("voltage1");
	    if (ch3_en)
		    channels.push_back("voltage2");
	    if (ch4_en)
		    channels.push_back("voltage3");
	    return channels;
    }

    fmcomms2_sink_impl::fmcomms2_sink_impl(const std::string &host,
		    double frequency, double samplerate, double bandwidth,
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    unsigned int buffer_size, bool _cyclic,
		    const char *rf_port_select,
		    double attenuation1, double attenuation2)
	    : gr::sync_block("fmcomms2_sink",
			    gr::io_signature::make(1, -1, sizeof(float)),
			    gr::io_signature::make(0, 0, 0))
	    , device_sink_impl(host, "cf-ad9361-dds-core-lpc",
			    get_channels_vector(ch1_en, ch2_en, ch3_en, ch4_en),
			    buffer_size, _cyclic)
    {
	    struct iio_channel *ch, *ch2;
	    struct iio_device *dev = iio_context_find_device(ctx, "ad9361-phy");
	    if (!dev)
		    throw std::runtime_error("Device not found");

	    ch = iio_device_find_channel(dev, "altvoltage1", true);
	    iio_channel_attr_write_longlong(ch,
			    "frequency", (long long) frequency);

	    ch = iio_device_find_channel(dev, "voltage0", true);
	    ch2 = iio_device_find_channel(dev, "voltage1", true);
	    iio_channel_attr_write_longlong(ch,
			    "sampling_frequency", (long long) samplerate);
	    iio_channel_attr_write_longlong(ch,
			    "rf_bandwidth", (long long) bandwidth);
	    iio_channel_attr_write(ch, "rf_port_select", rf_port_select);
	    iio_channel_attr_write_double(ch, "hardwaregain", attenuation1);
	    iio_channel_attr_write_double(ch2, "hardwaregain", attenuation2);

	    cyclic = _cyclic;
    }

    int fmcomms2_sink_impl::work(int noutput_items,
		    gr_vector_const_void_star &input_items,
		    gr_vector_void_star &output_items)
    {
	    int ret = device_sink_impl::work(noutput_items, input_items,
			    output_items);
	    if (ret < 0 || !cyclic)
		    return ret;
	    else
		    return WORK_DONE;
    }
  } /* namespace iio */
} /* namespace gr */
