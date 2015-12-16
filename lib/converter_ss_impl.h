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

#ifndef INCLUDED_IIO_CONVERTER_SS_IMPL_H
#define INCLUDED_IIO_CONVERTER_SS_IMPL_H

#include <iio.h>
#include <gnuradio/iio/converter_ss.h>

namespace gr {
	namespace iio {
		class converter_ss_impl : public converter_ss
		{
		private:
			const struct iio_channel *chn;
			bool inverse;

		public:
			converter_ss_impl(const struct iio_channel *channel,
					bool inverse);
			~converter_ss_impl();

			int work(int noutput_items,
					gr_vector_const_void_star &input_items,
					gr_vector_void_star &output_items);
		};
	}
}

#endif /* INCLUDED_IIO_CONVERTER_SS_IMPL_H */
