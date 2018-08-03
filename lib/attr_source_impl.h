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

#ifndef INCLUDED_IIO_ATTR_SOURCE_IMPL_H
#define INCLUDED_IIO_ATTR_SOURCE_IMPL_H

#include <gnuradio/iio/attr_source.h>
#include <vector>

#include "device_source_impl.h"

namespace gr {
  namespace iio {

    class attr_source_impl : public attr_source
    {
     private:
       std::string device;
       std::string channel;
       std::string uri;
       std::string attribute;
       int update_interval_ms;
       int samples_per_update;
       int data_type;

     protected:
      struct iio_context *ctx;
      struct iio_device *dev;
      struct iio_channel * chan;

     public:
      attr_source_impl(const std::string &uri, const std::string &device, const std::string &channel,
        const std::string &attribute, int update_interval_ms, int samples_per_update,
        int data_type);
      ~attr_source_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);

      float get_attribute_data(const std::string& attribute);

    };

  } // namespace iio
} // namespace gr

#endif /* INCLUDED_IIO_ATTR_SOURCE_IMPL_H */
