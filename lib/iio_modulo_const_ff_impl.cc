/* -*- c++ -*- */
/* 
 * Copyright 2016 Analog Devices Inc.
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

#include "iio_modulo_const_ff_impl.h"

#include <gnuradio/io_signature.h>
#include <cmath>

using namespace gr::iio;

modulo_const_ff::sptr modulo_const_ff::make(float modulo, size_t vlen)
{
	return gnuradio::get_initial_sptr(
			new modulo_const_ff_impl(modulo, vlen));
}

modulo_const_ff_impl::modulo_const_ff_impl(float modulo, size_t vlen)
	: sync_block("modulo_const_ff",
			io_signature::make (1, 1, sizeof(float)*vlen),
			io_signature::make (1, 1, sizeof(float)*vlen)),
	d_modulo(modulo), d_vlen(vlen)
{
}

int
modulo_const_ff_impl::work(int noutput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items)
{
	float *out = (float *) output_items[0];
	const float *in = (const float *) input_items[0];
	int noi = d_vlen*noutput_items;

	for (size_t i = 0; i < noi; i++)
		out[i] = std::fmod(in[i], d_modulo);

	return noutput_items;
}
