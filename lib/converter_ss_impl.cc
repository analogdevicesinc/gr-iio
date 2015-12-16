/* -*- c++ -*- */
/* 
 * Copyright 2015 Analog Devices Inc.
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

#include "converter_ss_impl.h"

#include <gnuradio/io_signature.h>

using namespace gr::iio;

converter_ss::sptr converter_ss::make(const struct iio_channel *chn, bool inv)
{
	return gnuradio::get_initial_sptr(new converter_ss_impl(chn, inv));
}

converter_ss_impl::converter_ss_impl(const struct iio_channel *_chn, bool inv) :
	gr::sync_block("iio_converter_ss",
			gr::io_signature::make(1, 1, sizeof(short)),
			gr::io_signature::make(1, 1, sizeof(short))),
	chn(_chn), inverse(inv)
{
}

converter_ss_impl::~converter_ss_impl()
{
}

int converter_ss_impl::work(int noutput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items)
{
	if (inverse)
		for (int i = 0; i < noutput_items; i++)
			iio_channel_convert_inverse(chn,
					&((short *)output_items[0])[i],
					&((const short *)input_items[0])[i]);
	else
		for (int i = 0; i < noutput_items; i++)
			iio_channel_convert(chn, &((short *)output_items[0])[i],
					&((const short *)input_items[0])[i]);

	consume(0, noutput_items);
	return noutput_items;
}
