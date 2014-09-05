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

#define SAMPLES_COUNT 0x8000

ssize_t demux_sample(const struct iio_channel *chn,
		void *sample, size_t size, void *d)
{
	float **out = (float **) iio_channel_get_data(chn);

	if (size == 1) {
		int8_t val;
		iio_channel_convert(chn, &val, sample);
		**out = (float) val;
	} else if (size == 2) {
		int16_t val;
		iio_channel_convert(chn, &val, sample);
		**out = (float) val;
	} else {
		int32_t val;
		iio_channel_convert(chn, &val, sample);
		**out = (float) val;
	}

	(*out)++;
	return 1;
}

namespace gr {
  namespace iio {

    device_source::sptr
    device_source::make(const std::string &host, const std::string &device,
		    const std::vector<std::string> &channels)
    {
      return gnuradio::get_initial_sptr
        (new device_source_impl(host, device, channels));
    }

    /*
     * The private constructor
     */
    device_source_impl::device_source_impl(const std::string &host,
		    const std::string &device,
		    const std::vector<std::string> &channels)
      : gr::sync_block("device_source",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, -1, sizeof(float)))
    {
	    struct iio_device *dev = NULL;
	    unsigned int nb_channels, i;

	    /* Set minimum output size */
	    set_min_output_buffer(SAMPLES_COUNT);

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

	    buf = iio_device_create_buffer(dev, SAMPLES_COUNT, false);
	    if (!buf)
		    throw std::runtime_error("Unable to create buffer");

	    refill_thd = new std::thread(&device_source_impl::refill, this);
    }

    /*
     * Our virtual destructor.
     */
    device_source_impl::~device_source_impl()
    {
	    refill_thd->join();
	    delete refill_thd;

	    iio_buffer_destroy(buf);
	    iio_context_destroy(ctx);
    }

    void device_source_impl::refill()
    {
	    while (!iio_buffer_refill(buf));
    }

    int
    device_source_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
	float *out[output_items.size()];

	if (noutput_items > SAMPLES_COUNT)
		noutput_items = SAMPLES_COUNT;

	for (unsigned int i = 0; i < output_items.size(); i++) {
		struct iio_channel *chn = channel_list[i];
		out[i] = (float *) output_items[i];
		iio_channel_set_data(chn, &out[i]);
	}

	refill_thd->join();
	delete refill_thd;
	int ret = iio_buffer_foreach_sample(buf, demux_sample, NULL);

	/* Refill the buffer for next time */
	refill_thd = new std::thread(&device_source_impl::refill, this);
	return ret / output_items.size();
    }

  } /* namespace iio */
} /* namespace gr */
