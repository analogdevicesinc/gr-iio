/* -*- c++ -*- */
/*
 * Copyright 2012 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_GR_IIO_MODULO_CONST_FF_H
#define INCLUDED_GR_IIO_MODULO_CONST_FF_H

#include <gnuradio/iio/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
	namespace iio {
		class IIO_API modulo_const_ff : virtual public sync_block
		{
		public:
			typedef std::shared_ptr<modulo_const_ff> sptr;

			static sptr make(float modulo, size_t vlen=1);
		};

	}
}

#endif /* INCLUDED_GR_IIO_MODULO_CONST_FF_H */
