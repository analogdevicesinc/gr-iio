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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "adf4350_sink_impl.h"
#include <boost/lexical_cast.hpp>

namespace gr {
    namespace iio {

        adf4350_sink::sptr
        adf4350_sink::make(const std::string &uri)
        {
            return gnuradio::get_initial_sptr
                    (new adf4350_sink_impl(uri));
        }

        adf4350_sink_impl::adf4350_sink_impl(const std::string &uri)
                : gr::block("adf4350_sink",
                            gr::io_signature::make(0, 0, 0),
                            gr::io_signature::make(0, 0, 0)),
                  uri(uri)
        {
            adf4350_init_param default_adf4350_init_param = {
                    /* SPI */
                    {
                            1000000,
                            3,
                            static_cast<spi_mode>(0),
                            (void*)(uri.c_str())
                    },

                    /* Device settings */
                    10000000,		// clkin;
                    10000,			// channel_spacing;
                    2500000000ul,		// power_up_frequency;
                    0,			// reference_div_factor;
                    0,			// reference_doubler_enable;
                    0,			// reference_div2_enable;

                    /* r2_user_settings */
                    1,			// phase_detector_polarity_positive_enable;
                    0,			// lock_detect_precision_6ns_enable;
                    0,			// lock_detect_function_integer_n_enable;
                    2500,			// charge_pump_current;
                    1,			// muxout_select;
                    0,			// low_spur_mode_enable;

                    /* r3_user_settings */
                    0,			// cycle_slip_reduction_enable;
                    0,			// charge_cancellation_enable;
                    0,			// anti_backlash_3ns_enable;
                    0,			// band_select_clock_mode_high_enable;
                    0,			// clk_divider_12bit;
                    0,			// clk_divider_mode;

                    /* r4_user_settings */
                    0,			// aux_output_enable;
                    1,			// aux_output_fundamental_enable;
                    0,			// mute_till_lock_enable;
                    3,			// output_power;
                    0,			// aux_output_power;
            };
            adf4350_setup(&adf4350_device, default_adf4350_init_param);
            message_port_register_in(pmt::mp("attr"));
            set_msg_handler(pmt::mp("attr"), boost::bind(&adf4350_sink_impl::write_attribute, this, _1));
        }

        adf4350_sink_impl::~adf4350_sink_impl()
        {

        }

        void adf4350_sink_impl::write_attribute(pmt::pmt_t pdu)
        {
            long long freq;
            pmt::pmt_t keys;

            if (!is_dict(pdu))
                throw std::runtime_error("Message not a dictionary!\n");

            keys = pmt::dict_keys(pdu);

            freq = std::stoll(pmt::symbol_to_string(pmt::dict_ref(pdu, pmt::nth(0, keys), pmt::PMT_NIL)));
            adf4350_out_altvoltage0_frequency(adf4350_device, freq);
        }

    }
}