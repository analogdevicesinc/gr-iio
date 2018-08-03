/* -*- c++ -*- */
/*
 * Copyright 2018 Analog Devices Inc.
 * Author: Travis Collins <travis.collins@analog.com>
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
#include "attr_source_impl.h"
#include <boost/lexical_cast.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>
#include <vector>


namespace gr {
  namespace iio {

    attr_source::sptr
    attr_source::make(const std::string &uri, const std::string &device, const std::string &channel,
      const std::vector<std::string> &attributes, int update_interval_ms, int samples_per_update)
    {
      return gnuradio::get_initial_sptr
        (new attr_source_impl(uri, device, channel, attributes, update_interval_ms, samples_per_update));
    }

    /*
     * The private constructor
     */
    attr_source_impl::attr_source_impl(const std::string &uri, const std::string &device, const std::string &channel,
      const std::vector<std::string> &attributes, int update_interval_ms, int samples_per_update)
      : gr::sync_block("attr_source",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, -1, sizeof(float))),
      device(device),
      channel(channel),
      uri(uri),
      attributes(attributes),
      update_interval_ms(update_interval_ms),
      samples_per_update(samples_per_update)
    {

      ctx = device_source_impl::get_context(uri);
      if (!ctx)
        throw std::runtime_error("Unable to create context");

      dev = iio_context_find_device(ctx, device.c_str());
      if (!dev) {
		    iio_context_destroy(ctx);
		    throw std::runtime_error("Device not found");
	    }

      chan = iio_device_find_channel(dev, channel.c_str(), false);
      if (!chan) {
        iio_context_destroy(ctx);
        throw std::runtime_error("Channel not found");
      }

      set_output_multiple(samples_per_update);

    }

    /*
     * Our virtual destructor.
     */
    attr_source_impl::~attr_source_impl()
    {
    }

    float
    attr_source_impl::get_attribute_data(const std::string& attribute)
    {
      int ret;
      // char buf[1024];
      double val;
      // ret = iio_channel_attr_read(chan, attribute.c_str(), buf, sizeof(buf));
      ret = iio_channel_attr_read_double(chan, attribute.c_str(), &val);
      if (ret > 0) {
        std::cerr << "Reading parameter failed: "<< ret << std::endl;
      }
      return boost::lexical_cast<float>(val);
    }

    int
    attr_source_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      int sample, output;
      float *out;


      for (sample = 0; sample<samples_per_update; sample++)
      {
        boost::this_thread::sleep_for(boost::chrono::milliseconds(update_interval_ms));
        for (output = 0; output<output_items.size(); output++)
        {
          out = (float*) output_items[output];
          out[sample] = get_attribute_data(attributes[output]);
        }
      }

      // Tell runtime system how many output items we produced.
      return samples_per_update;
    }

  } /* namespace iio */
} /* namespace gr */
