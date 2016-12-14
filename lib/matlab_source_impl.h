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

#ifndef INCLUDED_IIO_MATLAB_SOURCE_IMPL_H
#define INCLUDED_IIO_MATLAB_SOURCE_IMPL_H

#include <gnuradio/iio/matlab_source.h>
#include <matio.h>

#include <vector>

namespace gr {
namespace iio {
	class matlab_source_impl : public matlab_source
	{
	public:
		matlab_source_impl(const std::string &file);
		~matlab_source_impl();

		int work(int noutput_items,
				gr_vector_const_void_star &input_items,
				gr_vector_void_star &output_items);

	private:
		mat_t *mat;
		std::vector<matvar_t *> vars;
		std::vector<size_t> sizes, counters;
		enum matio_classes class_type;
		bool is_complex;
		size_t data_size;

		void cleanup();
	};
}
}

#endif
