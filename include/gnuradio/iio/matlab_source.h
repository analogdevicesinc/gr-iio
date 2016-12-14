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

#ifndef INCLUDED_IIO_MATLAB_SOURCE_H
#define INCLUDED_IIO_MATLAB_SOURCE_H

#include <gnuradio/iio/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace iio {
	class IIO_API matlab_source : virtual public gr::sync_block
	{
	public:
		typedef boost::shared_ptr<matlab_source> sptr;

		static sptr make(const std::string &file);
	};
} /* namespace iio */
} /* namespace gr */

#endif
