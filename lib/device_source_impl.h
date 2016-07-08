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

#ifndef INCLUDED_IIO_DEVICE_SOURCE_IMPL_H
#define INCLUDED_IIO_DEVICE_SOURCE_IMPL_H

#include <string>
#include <vector>

#include <iio.h>
#include <boost/thread.hpp>
#include <gnuradio/iio/device_source.h>

namespace gr {
  namespace iio {

    class device_source_impl : public device_source
    {
     private:
	     void channel_read(const struct iio_channel *chn,
			     void *dst, size_t len);

	     boost::mutex iio_mutex;
	     unsigned long items_in_buffer;
	     off_t byte_offset;
	     unsigned long sample_counter;

     protected:
	     struct iio_context *ctx;
	     struct iio_device *dev, *phy;
	     struct iio_buffer *buf;
	     std::vector <struct iio_channel *> channel_list;
	     unsigned int buffer_size;
	     unsigned int decimation;
	     bool destroy_ctx;

     public:
      device_source_impl(struct iio_context *ctx, bool destroy_ctx,
		      const std::string &device,
		      const std::vector<std::string> &channels,
		      const std::string &device_phy,
		      const std::vector<std::string> &params,
		      unsigned int buffer_size = DEFAULT_BUFFER_SIZE,
		      unsigned int decimation = 0);
      ~device_source_impl();

      static void set_params(struct iio_device *phy,
		      const std::vector<std::string> &params);

      void set_params(const std::vector<std::string> &params);
      void set_buffer_size(unsigned int buffer_size);

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);

      bool start();

      static struct iio_context * get_context(const std::string &uri);
      static bool load_fir_filter(std::string &filter, struct iio_device *phy);
    };

  } // namespace iio
} // namespace gr

#endif /* INCLUDED_IIO_DEVICE_SOURCE_IMPL_H */
