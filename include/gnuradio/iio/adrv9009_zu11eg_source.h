/* -*- c++ -*- */
/*
 * Copyright 2019 Analog Devices Inc.
 * Author: Travis F. Collins <travis.collins@analog.com>
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


#ifndef INCLUDED_IIO_ADRV9009_ZU11EG_SOURCE_H
#define INCLUDED_IIO_ADRV9009_ZU11EG_SOURCE_H

#include <gnuradio/iio/api.h>
#include <gnuradio/hier_block2.h>
#include <gnuradio/sync_block.h>

#include "device_source.h"

namespace gr {
  namespace iio {

    /*!
     * \brief <+description of block+>
     * \ingroup iio
     *
     */
    class IIO_API adrv9009_zu11eg_source : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<adrv9009_zu11eg_source> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of iio::device.
       *
       * To avoid accidental use of raw pointers, iio::device's
       * constructor is in a private implementation
       * class. iio::device::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::string &uri, unsigned long long frequency1,
		    unsigned long long frequency2,
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    bool ch5_en, bool ch6_en, bool ch7_en, bool ch8_en,
		    unsigned long buffer_size, bool quadrature,
		    const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value,
		    const char *gain3, double gain3_value,
		    const char *gain4, double gain4_value,
		    const char *filter = "");

      static sptr make_from(struct iio_context *ctx,
		    unsigned long long frequency1,
		    unsigned long long frequency2,
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    bool ch5_en, bool ch6_en, bool ch7_en, bool ch8_en,
		    unsigned long buffer_size, bool quadrature,
		    const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value,
		    const char *gain3, double gain3_value,
		    const char *gain4, double gain4_value,
		    const char *filter = "");

      virtual void set_params(unsigned long long frequency1,
		      unsigned long long frequency2,
		      bool quadrature,
		      const char *gain1, double gain1_value,
		      const char *gain2, double gain2_value,
		      const char *gain3, double gain3_value,
		      const char *gain4, double gain4_value) = 0;
    };

    class IIO_API adrv9009_zu11eg_source_f32c : virtual public gr::hier_block2
    {
    public:
      typedef boost::shared_ptr<adrv9009_zu11eg_source_f32c> sptr;

      static sptr make(const std::string &uri,
        unsigned long long frequency1, unsigned long long frequency2,
        bool rx1_en, bool rx2_en, bool rx3_en, bool rx4_en,
        unsigned long buffer_size, bool quadrature,
        const char *gain1, double gain1_value,
        const char *gain2, double gain2_value,
        const char *gain3, double gain3_value,
        const char *gain4, double gain4_value,
        const char *filter = "")
      {
        adrv9009_zu11eg_source::sptr block = adrv9009_zu11eg_source::make(uri,
            frequency1, frequency2,
            rx1_en, rx1_en, rx2_en,
            rx2_en, rx3_en, rx3_en, rx4_en,
            rx4_en, buffer_size, quadrature,
            gain1, gain1_value,
            gain2, gain2_value, gain3, gain3_value,
            gain4, gain4_value,
            filter);

        return gnuradio::get_initial_sptr(
            new adrv9009_zu11eg_source_f32c(rx1_en, rx2_en, rx3_en, rx4_en, block));
      }

      void set_params(unsigned long long frequency1,
          unsigned long long frequency2,
          bool quadrature,
          const char *gain1, double gain1_value,
          const char *gain2, double gain2_value,
          const char *gain3, double gain3_value,
          const char *gain4, double gain4_value)
      {
              adrv9009_zu11eg_block->set_params(frequency1, frequency2,
                              quadrature, gain1, gain1_value,
                              gain2, gain2_value, gain3, gain3_value,
                              gain4, gain4_value);
      }
    private:
      adrv9009_zu11eg_source::sptr adrv9009_zu11eg_block;

    protected:
      explicit adrv9009_zu11eg_source_f32c(bool rx1_en, bool rx2_en,
          bool rx3_en, bool rx4_en, adrv9009_zu11eg_source::sptr block);
    };

  } // namespace iio
} // namespace gr

#endif /* INCLUDED_IIO_ADRV9009_ZU11EG_SOURCE_H */
