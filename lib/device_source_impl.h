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
#include <iio/device_source.h>

namespace gr {
  namespace iio {

    struct ctxInfo{std::string uri; struct iio_context * ctx; int count;};
    static std::vector<ctxInfo> contexts;
    static boost::mutex ctx_mutex;

    typedef std::vector<ctxInfo>::iterator ctx_it;

    class device_source_impl : public device_source
    {
     private:
	     void channel_read(const struct iio_channel *chn,
			     void *dst, size_t len);

	     boost::mutex iio_mutex;
	     boost::condition_variable iio_cond, iio_cond2;
	     unsigned long items_in_buffer;
	     off_t byte_offset;
	     volatile bool please_refill_buffer, thread_stopped;
	     pmt::pmt_t port_id;

	     boost::thread refill_thd;

	     unsigned long timeout;

	     void refill_thread();

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
      void set_timeout_ms(unsigned long timeout);

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);

      bool start();
      bool stop();

      static void remove_ctx_history(struct iio_context *ctx, bool destroy_ctx);

      static struct iio_context * get_context(const std::string &uri);
      static bool load_fir_filter(std::string &filter, struct iio_device *phy);
    };

  } // namespace iio
} // namespace gr

#endif /* INCLUDED_IIO_DEVICE_SOURCE_IMPL_H */
