/* -*- c++ -*- */
/*
 * Copyright 2019 Travis F. Collins <travis.collins@analog.com>.
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
#include "fftshift_impl.h"

namespace gr {
  namespace iio {

    fftshift::sptr
    fftshift::make(unsigned int vlen)
    {
      return gnuradio::get_initial_sptr
        (new fftshift_impl(vlen));
    }

    /*
     * The private constructor
     */
    fftshift_impl::fftshift_impl(unsigned int vlen)
      : gr::sync_block("fftshift",
              gr::io_signature::make(1, 1, sizeof(float)*vlen),
              gr::io_signature::make(1, 1, sizeof(float)*vlen)),
      d_vlen(vlen)
    {}

    /*
     * Our virtual destructor.
     */
    fftshift_impl::~fftshift_impl()
    {
    }

    int
    fftshift_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float*) input_items[0];
      float *out = (float *) output_items[0];

      // Swap through
      int k;
      int len_o2 = d_vlen/2;
      size_t so_len_o2 = sizeof(float)*d_vlen/2;
      for (k = 0; k<noutput_items; k++)
      {
        memcpy(out,        in+len_o2, so_len_o2);
        memcpy(out+len_o2, in,        so_len_o2);
        out = out + d_vlen;
        in = in + d_vlen;
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace iio */
} /* namespace gr */
