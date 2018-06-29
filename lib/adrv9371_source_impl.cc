/* -*- c++ -*- */
/*
 * Copyright 2018 Analog Devices Inc.
 * Author: Travis Collins <travis.collins@analog.com>
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
#include "adrv9371_source_impl.h"

#include <gnuradio/blocks/short_to_float.h>
#include <gnuradio/blocks/float_to_complex.h>

using namespace gr::blocks;

namespace gr {
  namespace iio {


    adrv9371_source_f32c::adrv9371_source_f32c(bool rx1_en, bool rx2_en,
                    adrv9371_source::sptr src_block) :
            hier_block2("adrv9371_f32c",
                io_signature::make(0, 0, 0),
                io_signature::make(
			(int) rx1_en + (int) rx2_en,
			(int) rx1_en + (int) rx2_en,
			sizeof(gr_complex))),
            adrv9371_block(src_block)
    {
      basic_block_sptr hier = shared_from_this();
      unsigned int num_streams = (int) rx1_en + (int) rx2_en;

      for (unsigned int i = 0; i < num_streams; i++) {
        short_to_float::sptr s2f1 = short_to_float::make(1, 2048.0);//FIXME
        short_to_float::sptr s2f2 = short_to_float::make(1, 2048.0);//FIXME
        float_to_complex::sptr f2c = float_to_complex::make(1);

        connect(src_block, i * 2, s2f1, 0);
        connect(src_block, i * 2 + 1, s2f2, 0);
        connect(s2f1, 0, f2c, 0);
        connect(s2f2, 0, f2c, 1);
        connect(f2c, 0, hier, i);
      }
    }

    adrv9371_source::sptr
    adrv9371_source::make(const std::string &uri, unsigned long long frequency,
      unsigned long rf_bandwidth, const char *profile,
      bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
      unsigned long buffer_size,
      const char *gain1, double gain1_manual, const char *gain2, double gain2_manual)
    {
      return gnuradio::get_initial_sptr
        (new adrv9371_source_impl(device_source_impl::get_context(uri), true,
				  frequency, rf_bandwidth, profile,
				  ch1_en, ch2_en, ch3_en, ch4_en, buffer_size,
				  gain1, gain1_manual, gain2, gain2_manual));
    }

    adrv9371_source::sptr
    adrv9371_source::make_from(struct iio_context *ctx,
		    unsigned long long frequency,
		    unsigned long bandwidth,
        const char *profile,
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    unsigned long buffer_size,
		    const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value)
    {
      return gnuradio::get_initial_sptr
        (new adrv9371_source_impl(ctx, false, frequency,
				  bandwidth, profile,
				  ch1_en, ch2_en, ch3_en, ch4_en, buffer_size,
				  gain1, gain1_value,
				  gain2, gain2_value));
    }

    std::vector<std::string> adrv9371_source_impl::get_channels_vector(
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en)
    {
	    std::vector<std::string> channels;
	    if (ch1_en)
		    channels.push_back("voltage0_i");
	    if (ch2_en)
		    channels.push_back("voltage0_q");
	    if (ch3_en)
		    channels.push_back("voltage1_i");
	    if (ch4_en)
		    channels.push_back("voltage1_q");
	    return channels;
    }

    adrv9371_source_impl::~adrv9371_source_impl()
    {
    }

    adrv9371_source_impl::adrv9371_source_impl(struct iio_context *ctx,
		    bool destroy_ctx,
		    unsigned long long frequency,
		    unsigned long bandwidth, const char *profile,
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    unsigned long buffer_size, const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value)
      : gr::sync_block("adrv9371_source",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, -1, sizeof(short)))
      , device_source_impl(ctx, destroy_ctx, "axi-ad9371-rx-hpc",
		      get_channels_vector(ch1_en, ch2_en, ch3_en, ch4_en),
		      "ad9371-phy", std::vector<std::string>(),
		      buffer_size, 0)
    {
	    set_params(frequency, bandwidth, profile,
			    gain1, gain1_value, gain2, gain2_value);
    }

    void adrv9371_source_impl::set_params(unsigned long long frequency,
		    unsigned long bandwidth,
		    const char *profile,
		    const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value)
    {
	    std::vector<std::string> params;

	    params.push_back("out_altvoltage0_RX_LO_frequency=" +
			    boost::to_string(frequency));
	    params.push_back("in_voltage0_rf_bandwidth=" +
			    boost::to_string(bandwidth));
	    std::string gain1_str = boost::to_string(gain1);
	    params.push_back("in_voltage0_gain_control_mode=" +
			    gain1_str);
	    if (gain1_str.compare("manual") == 0) {
	            params.push_back("in_voltage0_hardwaregain=" +
			    boost::to_string(gain1_value));
	    }
	    std::string gain2_str = boost::to_string(gain2);
	    params.push_back("in_voltage1_gain_control_mode=" +
			    gain2_str);
	    if (gain2_str.compare("manual") == 0) {
		    params.push_back("in_voltage1_hardwaregain=" +
				    boost::to_string(gain2_value));
            }

	    device_source_impl::set_params(params);

      // Insert profile write here

    }

  } /* namespace iio */
} /* namespace gr */
