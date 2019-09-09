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


#ifndef INCLUDED_IIO_ADF4350_SINK_H
#define INCLUDED_IIO_ADF4350_SINK_H

#include <gnuradio/iio/api.h>
#include <gnuradio/block.h>

namespace gr {
    namespace iio {

        /*!
         * \brief <+description of block+>
         * \ingroup iio
         *
         */
        class IIO_API adf4350_sink : virtual public gr::block
        {
        public:
            typedef boost::shared_ptr<adf4350_sink> sptr;

            /*!
             * \brief Return a shared_ptr to a new instance of iio::adf4350_sink.
             *
             * To avoid accidental use of raw pointers, iio::adf4350_sink's
             * constructor is in a private implementation
             * class. iio::adf4350_sink::make is the public interface for
             * creating new instances.
             */
            static sptr make(const std::string &uri,
            uint32_t    clkin,
            uint32_t	channel_spacing,
            uint32_t	power_up_frequency,
            uint32_t	reference_div_factor,
            uint8_t	reference_doubler_enable,
            uint8_t	reference_div2_enable,

            /* r2_user_settings */
            uint8_t	phase_detector_polarity_positive_enable,
            uint8_t	lock_detect_precision_6ns_enable,
            uint8_t	lock_detect_function_integer_n_enable,
            uint32_t	charge_pump_current,
            uint32_t	muxout_select,
            uint8_t	low_spur_mode_enable,

            /* r3_user_settings */
            uint8_t	cycle_slip_reduction_enable,
            uint8_t	charge_cancellation_enable,
            uint8_t	anti_backlash_3ns_enable,
            uint8_t	band_select_clock_mode_high_enable,
            uint32_t	clk_divider_12bit,
            uint32_t	clk_divider_mode,

            /* r4_user_settings */
            uint8_t	aux_output_enable,
            uint8_t	aux_output_fundamental_enable,
            uint8_t	mute_till_lock_enable,
            uint32_t	output_power,
            uint32_t	aux_output_power);
        };

    } // namespace iio
} // namespace gr

#endif /* INCLUDED_IIO_ADF4350_SINK_H */

