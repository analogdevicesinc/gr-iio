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
#include "vector_select_impl.h"
#include <iostream>

namespace gr {
  namespace iio {

    vector_select::sptr
    vector_select::make(unsigned int vlen)
    {
      return gnuradio::get_initial_sptr
        (new vector_select_impl(vlen));
    }

    /*
     * The private constructor
     */
    vector_select_impl::vector_select_impl(unsigned int vlen)
      : gr::sync_block("vector_select",
              gr::io_signature::make2(2, 2, sizeof(gr_complex)*vlen, sizeof(short)),
              gr::io_signature::make2(2, 2, sizeof(gr_complex), sizeof(short))),
      d_vlen(vlen)
    {}

    /*
     * Our virtual destructor.
     */
    vector_select_impl::~vector_select_impl()
    {
    }

    int
    vector_select_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      const short *in_s = (const short *) input_items[1];
      gr_complex *out = (gr_complex *) output_items[0];
      short *out_s = (short *) output_items[1];

      // Grab sample from each vector
      int k,kv=0;
      int d_vlen_o2 = d_vlen/2;
      for (k=0; k<noutput_items; k++)
      {
        out[k] = in[kv+in_s[k]];
        // Output wrap correction
        if (in_s[k]>d_vlen_o2)
          out_s[k] = in_s[k] - d_vlen;
        else
          out_s[k] = in_s[k];

        kv = kv + d_vlen;
      }
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace iio */
} /* namespace gr */
