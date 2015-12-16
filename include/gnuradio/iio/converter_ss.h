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


#ifndef INCLUDED_IIO_CONVERTER_SS_H
#define INCLUDED_IIO_CONVERTER_SS_H

#include <gnuradio/iio/api.h>
#include <gnuradio/sync_block.h>

extern "C" {
	struct iio_channel;
};

namespace gr {
	namespace iio {

		/*!
		 * \brief <+description of block+>
		 * \ingroup iio
		 *
		 */
		class IIO_API converter_ss : virtual public gr::sync_block
		{
		public:
			typedef boost::shared_ptr<converter_ss> sptr;

			static sptr make(const struct iio_channel *channel,
					bool inverse);
		};
	}
}

#endif /* INCLUDED_IIO_CONVERTER_SS_H */
