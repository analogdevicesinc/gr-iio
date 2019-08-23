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

#ifndef INCLUDED_IIO_ADRV9009_ZU11EG_SOURCE_IMPL_H
#define INCLUDED_IIO_ADRV9009_ZU11EG_SOURCE_IMPL_H

#include <string>
#include <vector>

#include <gnuradio/iio/adrv9009_zu11eg_source.h>

#include "device_source_impl.h"

namespace gr {
  namespace iio {

    class adrv9009_zu11eg_source_impl : public adrv9009_zu11eg_source
			       , public device_source_impl
    {
     private:
      unsigned long samplerate;
      struct iio_device *phy2;

      static void set_params(struct iio_device *phy_device,
		      unsigned long long frequency,
		      bool quadrature,
		      const char *gain1, double gain1_value,
		      const char *gain2, double gain2_value);

      std::vector<std::string> get_channels_vector(
		      bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		      bool ch5_en, bool ch6_en, bool ch7_en, bool ch8_en);

      bool load_profile(std::string &filter, struct iio_device *phy);

     public:
      adrv9009_zu11eg_source_impl(struct iio_context *ctx,
		    bool destroy_ctx, unsigned long long frequency1,
		    unsigned long long frequency2,
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    bool ch5_en, bool ch6_en, bool ch7_en, bool ch8_en,
		    unsigned long buffer_size, bool quadrature,
		    const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value,
		    const char *gain3, double gain3_value,
		    const char *gain4, double gain4_value,
		    const char *filter);

      void set_params(unsigned long long frequency1,
		      unsigned long long frequency2,
		      bool quadrature,
		      const char *gain1, double gain1_value,
		      const char *gain2, double gain2_value,
		      const char *gain3, double gain3_value,
		      const char *gain4, double gain4_value);
    };

  } // namespace iio
} // namespace gr

#endif /* INCLUDED_IIO_ADRV9009_ZU11EG_SOURCE_IMPL_H */
