/* -*- c++ -*- */
/* 
 * Copyright 2015 Analog Devices Inc.
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
#include "fmcomms5_source_impl.h"

#include <ad9361.h>

namespace gr {
  namespace iio {

    fmcomms5_source::sptr
    fmcomms5_source::make(const std::string &uri,
		    unsigned long long frequency1,
		    unsigned long long frequency2, unsigned long samplerate,
		    unsigned long decimation, unsigned long bandwidth,
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    bool ch5_en, bool ch6_en, bool ch7_en, bool ch8_en,
		    unsigned long buffer_size, bool quadrature, bool rfdc,
		    bool bbdc, const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value,
		    const char *gain3, double gain3_value,
		    const char *gain4, double gain4_value,
		    const char *port_select, const char *filter)
    {
      return gnuradio::get_initial_sptr
        (new fmcomms5_source_impl(device_source_impl::get_context(uri),
				  true, frequency1, frequency2,
				  samplerate, decimation, bandwidth,
				  ch1_en, ch2_en, ch3_en, ch4_en,
				  ch5_en, ch6_en, ch7_en, ch8_en,
				  buffer_size,
				  quadrature, rfdc, bbdc, gain1, gain1_value,
				  gain2, gain2_value, gain3, gain3_value,
				  gain4, gain4_value, port_select, filter));
    }

    fmcomms5_source::sptr
    fmcomms5_source::make_from(struct iio_context *ctx,
		    unsigned long long frequency1,
		    unsigned long long frequency2, unsigned long samplerate,
		    unsigned long decimation, unsigned long bandwidth,
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    bool ch5_en, bool ch6_en, bool ch7_en, bool ch8_en,
		    unsigned long buffer_size, bool quadrature, bool rfdc,
		    bool bbdc, const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value,
		    const char *gain3, double gain3_value,
		    const char *gain4, double gain4_value,
		    const char *port_select, const char *filter)
    {
      return gnuradio::get_initial_sptr
        (new fmcomms5_source_impl(ctx, false, frequency1, frequency2,
				  samplerate, decimation, bandwidth,
				  ch1_en, ch2_en, ch3_en, ch4_en,
				  ch5_en, ch6_en, ch7_en, ch8_en,
				  buffer_size,
				  quadrature, rfdc, bbdc, gain1, gain1_value,
				  gain2, gain2_value, gain3, gain3_value,
				  gain4, gain4_value, port_select, filter));
    }

    std::vector<std::string> fmcomms5_source_impl::get_channels_vector(
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    bool ch5_en, bool ch6_en, bool ch7_en, bool ch8_en)
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
	    if (ch5_en)
		    channels.push_back("voltage4");
	    if (ch6_en)
		    channels.push_back("voltage5");
	    if (ch7_en)
		    channels.push_back("voltage6");
	    if (ch8_en)
		    channels.push_back("voltage7");
	    return channels;
    }

    fmcomms5_source_impl::fmcomms5_source_impl(struct iio_context *ctx,
		    bool destroy_ctx, unsigned long long frequency1,
		    unsigned long long frequency2, unsigned long samplerate,
		    unsigned long decimation, unsigned long bandwidth,
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    bool ch5_en, bool ch6_en, bool ch7_en, bool ch8_en,
		    unsigned long buffer_size, bool quadrature, bool rfdc,
		    bool bbdc, const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value,
		    const char *gain3, double gain3_value,
		    const char *gain4, double gain4_value,
		    const char *port_select, const char *filter)
      : gr::sync_block("fmcomms5_source",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, -1, sizeof(short))),
        device_source_impl(ctx, destroy_ctx, "cf-ad9361-A",
		      get_channels_vector(ch1_en, ch2_en, ch3_en, ch4_en,
			      ch5_en, ch6_en, ch7_en, ch8_en),
		      "ad9361-phy", std::vector<std::string>(),
		      buffer_size, decimation),
        samplerate(0)
    {
	    phy2 = iio_context_find_device(ctx, "ad9361-phy-B");
	    if (!phy2)
		    throw std::runtime_error("Device not found");

	    set_params(frequency1, frequency2,
			    samplerate, bandwidth, quadrature, rfdc, bbdc,
			    gain1, gain1_value, gain2, gain2_value,
			    gain3, gain3_value, gain4, gain4_value,
			    port_select);

	    std::string filt(filter);
	    if (!filt.empty() && !(load_fir_filter(filt, phy) &&
				    load_fir_filter(filt, phy2)))
		    throw std::runtime_error("Unable to load filter file");
    }

    void fmcomms5_source_impl::set_params(struct iio_device *phy_device,
		    unsigned long long frequency,
		    unsigned long samplerate, unsigned long bandwidth,
		    bool quadrature, bool rfdc, bool bbdc,
		    const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value,
		    const char *port_select)
    {
	    std::vector<std::string> params;

	    params.push_back("out_altvoltage0_RX_LO_frequency=" +
			    boost::to_string(frequency));
	    params.push_back("in_voltage_sampling_frequency=" +
			    boost::to_string(samplerate));
	    params.push_back("in_voltage_rf_bandwidth=" +
			    boost::to_string(bandwidth));
	    params.push_back("in_voltage_quadrature_tracking_en=" +
			    boost::to_string(quadrature));
	    params.push_back("in_voltage_rf_dc_offset_tracking_en=" +
			    boost::to_string(rfdc));
	    params.push_back("in_voltage_bb_dc_offset_tracking_en=" +
			    boost::to_string(bbdc));
	    params.push_back("in_voltage0_gain_control_mode=" +
			    boost::to_string(gain1));
	    params.push_back("in_voltage0_hardwaregain=" +
			    boost::to_string(gain1_value));
	    params.push_back("in_voltage1_gain_control_mode=" +
			    boost::to_string(gain2));
	    params.push_back("in_voltage1_hardwaregain=" +
			    boost::to_string(gain2_value));
	    params.push_back("in_voltage0_rf_port_select=" +
			    boost::to_string(port_select));

	    device_source_impl::set_params(phy_device, params);
    }

    void fmcomms5_source_impl::set_params(unsigned long long frequency1,
		    unsigned long long frequency2,
		    unsigned long samplerate, unsigned long bandwidth,
		    bool quadrature, bool rfdc, bool bbdc,
		    const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value,
		    const char *gain3, double gain3_value,
		    const char *gain4, double gain4_value,
		    const char *port_select)
    {
	    set_params(this->phy, frequency1, samplerate, bandwidth,
			    quadrature, rfdc, bbdc, gain1, gain1_value,
			    gain2, gain2_value, port_select);
	    set_params(this->phy2, frequency2, samplerate, bandwidth,
			    quadrature, rfdc, bbdc, gain3, gain3_value,
			    gain4, gain4_value, port_select);

	    if (this->samplerate != samplerate) {
		    ad9361_fmcomms5_multichip_sync(ctx, FIXUP_INTERFACE_TIMING |
				    CHECK_SAMPLE_RATES);
		    this->samplerate = samplerate;
	    }
    }

  } /* namespace iio */
} /* namespace gr */
