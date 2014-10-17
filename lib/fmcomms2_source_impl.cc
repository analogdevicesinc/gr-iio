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
#include "fmcomms2_source_impl.h"

namespace gr {
  namespace iio {

    fmcomms2_source::sptr
    fmcomms2_source::make(const std::string &host, unsigned long long frequency,
		    unsigned long samplerate, unsigned long bandwidth,
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    unsigned long buffer_size, bool quadrature, bool rfdc,
		    bool bbdc, const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value,
		    const char *port_select)
    {
      return gnuradio::get_initial_sptr
        (new fmcomms2_source_impl(host, frequency, samplerate, bandwidth,
				  ch1_en, ch2_en, ch3_en, ch4_en, buffer_size,
				  quadrature, rfdc, bbdc, gain1, gain1_value,
				  gain2, gain2_value, port_select));
    }

    std::vector<std::string> fmcomms2_source_impl::get_channels_vector(
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

    fmcomms2_source_impl::fmcomms2_source_impl(const std::string &host,
		    unsigned long long frequency,
		    unsigned long samplerate, unsigned long bandwidth,
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    unsigned long buffer_size, bool quadrature, bool rfdc,
		    bool bbdc, const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value,
		    const char *port_select)
      : gr::sync_block("fmcomms2_source",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, -1, sizeof(float)))
      , device_source_impl(host, "cf-ad9361-lpc",
		      get_channels_vector(ch1_en, ch2_en, ch3_en, ch4_en),
		      buffer_size)
    {
	    set_params(frequency, samplerate, bandwidth, quadrature, rfdc, bbdc,
			    gain1, gain1_value, gain2, gain2_value,
			    port_select);
    }

    void fmcomms2_source_impl::set_params(unsigned long long frequency,
		    unsigned long samplerate, unsigned long bandwidth,
		    bool quadrature, bool rfdc, bool bbdc,
		    const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value,
		    const char *port_select)
    {
	    struct iio_channel *ch, *ch2;
	    struct iio_device *dev = iio_context_find_device(ctx, "ad9361-phy");
	    if (!dev)
		    throw std::runtime_error("Device not found");

	    ch = iio_device_find_channel(dev, "altvoltage0", true);
	    iio_channel_attr_write_longlong(ch, "frequency", frequency);

	    ch = iio_device_find_channel(dev, "voltage0", false);
	    ch2 = iio_device_find_channel(dev, "voltage1", false);
	    iio_channel_attr_write_longlong(ch,
			    "sampling_frequency", (long long) samplerate);
	    iio_channel_attr_write_longlong(ch,
			    "rf_bandwidth", (long long) bandwidth);
	    iio_channel_attr_write_bool(ch,
			    "quadrature_tracking_en", quadrature);
	    iio_channel_attr_write_bool(ch,
			    "rf_dc_offset_tracking_en", rfdc);
	    iio_channel_attr_write_bool(ch,
			    "bb_dc_offset_tracking_en", bbdc);
	    iio_channel_attr_write_double(ch, "hardwaregain", gain1_value);
	    iio_channel_attr_write_double(ch2, "hardwaregain", gain2_value);
	    iio_channel_attr_write(ch, "gain_control_mode", gain1);
	    iio_channel_attr_write(ch2, "gain_control_mode", gain2);
	    iio_channel_attr_write(ch, "rf_port_select", port_select);
    }

  } /* namespace iio */
} /* namespace gr */
