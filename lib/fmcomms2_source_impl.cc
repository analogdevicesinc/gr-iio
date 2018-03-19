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

#include <gnuradio/blocks/short_to_float.h>
#include <gnuradio/blocks/float_to_complex.h>

#include <ad9361.h>

using namespace gr::blocks;

namespace gr {
  namespace iio {

    fmcomms2_source_f32c::fmcomms2_source_f32c(bool rx1_en, bool rx2_en,
                    fmcomms2_source::sptr src_block) :
            hier_block2("fmcomms2_f32c",
                io_signature::make(0, 0, 0),
                io_signature::make(
			(int) rx1_en + (int) rx2_en,
			(int) rx1_en + (int) rx2_en,
			sizeof(gr_complex))),
            fmcomms2_block(src_block)
    {
      basic_block_sptr hier = shared_from_this();
      unsigned int num_streams = (int) rx1_en + (int) rx2_en;

      for (unsigned int i = 0; i < num_streams; i++) {
        short_to_float::sptr s2f1 = short_to_float::make(1, 2048.0);
        short_to_float::sptr s2f2 = short_to_float::make(1, 2048.0);
        float_to_complex::sptr f2c = float_to_complex::make(1);

        connect(src_block, i * 2, s2f1, 0);
        connect(src_block, i * 2 + 1, s2f2, 0);
        connect(s2f1, 0, f2c, 0);
        connect(s2f2, 0, f2c, 1);
        connect(f2c, 0, hier, i);
      }
    }

    fmcomms2_source::sptr
    fmcomms2_source::make(const std::string &uri, unsigned long long frequency,
		    unsigned long samplerate,
		    unsigned long bandwidth,
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    unsigned long buffer_size, bool quadrature, bool rfdc,
		    bool bbdc, const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value,
		    const char *port_select, const char *filter,
		    bool auto_filter)
    {
      return gnuradio::get_initial_sptr
        (new fmcomms2_source_impl(device_source_impl::get_context(uri), true,
				  frequency, samplerate, bandwidth,
				  ch1_en, ch2_en, ch3_en, ch4_en, buffer_size,
				  quadrature, rfdc, bbdc, gain1, gain1_value,
				  gain2, gain2_value, port_select, filter,
				  auto_filter));
    }

    fmcomms2_source::sptr
    fmcomms2_source::make_from(struct iio_context *ctx,
		    unsigned long long frequency, unsigned long samplerate,
		    unsigned long bandwidth,
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    unsigned long buffer_size, bool quadrature, bool rfdc,
		    bool bbdc, const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value,
		    const char *port_select, const char *filter,
		    bool auto_filter)
    {
      return gnuradio::get_initial_sptr
        (new fmcomms2_source_impl(ctx, false, frequency, samplerate,
				  bandwidth,
				  ch1_en, ch2_en, ch3_en, ch4_en, buffer_size,
				  quadrature, rfdc, bbdc, gain1, gain1_value,
				  gain2, gain2_value, port_select, filter,
				  auto_filter));
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

    fmcomms2_source_impl::fmcomms2_source_impl(struct iio_context *ctx,
		    bool destroy_ctx,
		    unsigned long long frequency, unsigned long samplerate,
		    unsigned long bandwidth,
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    unsigned long buffer_size, bool quadrature, bool rfdc,
		    bool bbdc, const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value,
		    const char *port_select, const char *filter,
		    bool auto_filter)
      : gr::sync_block("fmcomms2_source",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, -1, sizeof(short)))
      , device_source_impl(ctx, destroy_ctx, "cf-ad9361-lpc",
		      get_channels_vector(ch1_en, ch2_en, ch3_en, ch4_en),
		      "ad9361-phy", std::vector<std::string>(),
		      buffer_size, 0)
    {
	    set_params(frequency, samplerate, bandwidth, quadrature, rfdc, bbdc,
			    gain1, gain1_value, gain2, gain2_value,
			    port_select, filter, auto_filter);
    }

    void fmcomms2_source_impl::set_params(unsigned long long frequency,
		    unsigned long samplerate, unsigned long bandwidth,
		    bool quadrature, bool rfdc, bool bbdc,
		    const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value,
		    const char *port_select, const char *filter,
		    bool auto_filter)
    {
	    bool is_fmcomms4 = !iio_device_find_channel(phy, "voltage1", false);
	    std::vector<std::string> params;

	    if (filter && filter[0])
		    auto_filter = false;

	    params.push_back("out_altvoltage0_RX_LO_frequency=" +
			    boost::to_string(frequency));
	    if (!auto_filter) {
		    params.push_back("in_voltage_sampling_frequency=" +
				    boost::to_string(samplerate));
	    }
	    params.push_back("in_voltage_rf_bandwidth=" +
			    boost::to_string(bandwidth));
	    params.push_back("in_voltage_quadrature_tracking_en=" +
			    boost::to_string(quadrature));
	    params.push_back("in_voltage_rf_dc_offset_tracking_en=" +
			    boost::to_string(rfdc));
	    params.push_back("in_voltage_bb_dc_offset_tracking_en=" +
			    boost::to_string(bbdc));
	    std::string gain1_str = boost::to_string(gain1);
	    params.push_back("in_voltage0_gain_control_mode=" +
			    gain1_str);
	    if (gain1_str.compare("manual") == 0) {
	            params.push_back("in_voltage0_hardwaregain=" +
			    boost::to_string(gain1_value));
	    }
	    if (!is_fmcomms4) {
		    std::string gain2_str = boost::to_string(gain2);
		    params.push_back("in_voltage1_gain_control_mode=" +
				    gain2_str);
		    if (gain2_str.compare("manual") == 0) {
			    params.push_back("in_voltage1_hardwaregain=" +
					    boost::to_string(gain2_value));
	            }
	    }

	    params.push_back("in_voltage0_rf_port_select=" +
			    boost::to_string(port_select));

	    device_source_impl::set_params(params);

	    if (auto_filter) {
		    int ret = ad9361_set_bb_rate(phy, samplerate);
		    if (ret) {
			    throw std::runtime_error("Unable to set BB rate");
		    }
	    } else if (filter && filter[0]) {
		    std::string filt(filter);
		    if (!load_fir_filter(filt, phy))
			    throw std::runtime_error("Unable to load filter file");
	    }
    }

  } /* namespace iio */
} /* namespace gr */
