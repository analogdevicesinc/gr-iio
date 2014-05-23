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
#include "device_sink_impl.h"

#define SAMPLES_COUNT 4000

struct mux_sample_data {
	const float *in;
	unsigned int nb;
};

ssize_t mux_sample(const struct iio_channel *chn,
		void *sample, size_t size, void *data)
{
	struct mux_sample_data *d =
		(struct mux_sample_data *) iio_channel_get_data(chn);
	const float *in = d->in;

	if (!d->nb)
		return 0;

	if (size == 1) {
		int8_t val = (int8_t) *in;
		iio_channel_convert_inverse(chn, sample, &val);
	} else if (size == 2) {
		int16_t val = (int16_t) *in;
		iio_channel_convert_inverse(chn, sample, &val);
	} else {
		int32_t val = (int32_t) *in;
		iio_channel_convert_inverse(chn, sample, &val);
	}

	d->in++;
	d->nb--;
	return size;
}

namespace gr {
  namespace iio {

    device_sink::sptr
    device_sink::make(const std::string &host, const std::string &device,
		    const std::vector<std::string> &channels)
    {
      return gnuradio::get_initial_sptr
        (new device_sink_impl(host, device, channels));
    }

    /*
     * The private constructor
     */
    device_sink_impl::device_sink_impl(const std::string &host,
		    const std::string &device,
		    const std::vector<std::string> &channels)
      : gr::sync_block("device_sink",
              gr::io_signature::make(1, -1, sizeof(float)),
              gr::io_signature::make(0, 0, 0))
    {
	    struct iio_device *dev = NULL;
	    unsigned int nb_channels, i;

	    /* Set minimum input size */
	    set_output_multiple(SAMPLES_COUNT);

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
					    it->c_str(), true);
		    if (!chn) {
			    iio_context_destroy(ctx);
			    throw std::runtime_error("Channel not found");
		    }

		    iio_channel_enable(chn);
		    if (!iio_channel_is_enabled(chn))
			    throw std::runtime_error("Channel not enabled");
		    channel_list.push_back(chn);
	    }

	    buf = iio_device_create_buffer(dev, SAMPLES_COUNT, false);
	    if (!buf)
		    throw std::runtime_error("Unable to create buffer");
    }

    /*
     * Our virtual destructor.
     */
    device_sink_impl::~device_sink_impl()
    {
	    iio_buffer_destroy(buf);
	    iio_context_destroy(ctx);
	    std::cout << "Buffer destroyed\n";
    }

    int
    device_sink_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
	struct mux_sample_data d[input_items.size()];

	if (noutput_items > SAMPLES_COUNT)
		noutput_items = SAMPLES_COUNT;

	for (unsigned int i = 0; i < input_items.size(); i++) {
		struct iio_channel *chn = channel_list[i];
		d[i].in = (const float *) input_items[i];
		d[i].nb = noutput_items;
		iio_channel_set_data(chn, &d[i]);
	}

	ssize_t ret = iio_buffer_foreach_sample(buf, mux_sample, NULL);
	if (ret > 0) {
		ret = iio_buffer_push(buf);

		for (unsigned int i = 0; i < input_items.size(); i++)
			consume(i, noutput_items - d[i].nb);
	}
	return noutput_items;
    }

    void
    device_sink_impl::forecast(int noutput_items,
		    gr_vector_int &ninput_items_required)
    {
	    for (unsigned int i = 0; i < ninput_items_required.size(); i++)
		    ninput_items_required[i] = noutput_items;
    }

  } /* namespace iio */
} /* namespace gr */
