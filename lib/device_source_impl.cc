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

#include <gnuradio/io_signature.h>
#include "device_source_impl.h"

namespace gr {
  namespace iio {

    device_source::sptr
    device_source::make(const std::string &host, const std::string &device,
		    const std::vector<std::string> &channels,
		    unsigned int buffer_size, unsigned int decimation)
    {
      return gnuradio::get_initial_sptr
        (new device_source_impl(host, device, channels,
				buffer_size, decimation));
    }

    /*
     * The private constructor
     */
    device_source_impl::device_source_impl(const std::string &host,
		    const std::string &device,
		    const std::vector<std::string> &channels,
		    unsigned int _buffer_size, unsigned int _decimation)
      : gr::sync_block("device_source",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, -1, sizeof(short)))
    {
	    struct iio_device *dev = NULL;
	    unsigned int nb_channels, i;

	    buffer_size = _buffer_size;
	    decimation = _decimation;

	    /* Set minimum output size */
	    set_output_multiple(buffer_size / (decimation + 1));

	    if (!host.compare("localhost"))
		    ctx = iio_create_local_context();
	    else
		    ctx = iio_create_network_context(host.c_str());
	    if (ctx)
		    dev = iio_context_find_device(ctx, device.c_str());
	    if (ctx && !dev) {
		    iio_context_destroy(ctx);
		    throw std::runtime_error("Device not found");
	    }

	    /* First disable all channels */
	    nb_channels = iio_device_get_channels_count(dev);
	    for (i = 0; i < nb_channels; i++)
		    iio_channel_disable(iio_device_get_channel(dev, i));

	    for (std::vector<std::string>::const_iterator it =
			    channels.begin();
			    it != channels.end(); ++it) {
		    struct iio_channel *chn =
			    iio_device_find_channel(dev,
					    it->c_str(), false);
		    if (!chn) {
			    iio_context_destroy(ctx);
			    throw std::runtime_error("Channel not found");
		    }

		    iio_channel_enable(chn);
		    channel_list.push_back(chn);
	    }

	    buf = iio_device_create_buffer(dev, buffer_size, false);
	    if (!buf)
		    throw std::runtime_error("Unable to create buffer");
    }

    /*
     * Our virtual destructor.
     */
    device_source_impl::~device_source_impl()
    {
	    iio_buffer_destroy(buf);
	    iio_context_destroy(ctx);
    }

    void
    device_source_impl::channel_read(const struct iio_channel *chn,
		    void *dst, size_t len)
    {
	uintptr_t src_ptr, dst_ptr = (uintptr_t) dst, end = dst_ptr + len;
	unsigned int length = iio_channel_get_data_format(chn)->length / 8;
	uintptr_t buf_end = (uintptr_t) iio_buffer_end(buf);
	ptrdiff_t buf_step = iio_buffer_step(buf) * (decimation + 1);

	for (src_ptr = (uintptr_t) iio_buffer_first(buf, chn);
			src_ptr < buf_end && dst_ptr + length <= end;
			src_ptr += buf_step, dst_ptr += length)
		iio_channel_convert(chn,
				(void *) dst_ptr, (const void *) src_ptr);
    }

    int
    device_source_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
	int ret = iio_buffer_refill(buf);
	if (ret < 0)
		return ret;

	for (unsigned int i = 0; i < output_items.size(); i++)
		channel_read(channel_list[i], output_items[i],
				noutput_items * sizeof(short));
	return noutput_items;
    }

  } /* namespace iio */
} /* namespace gr */
