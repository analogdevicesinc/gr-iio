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

#include <gnuradio/io_signature.h>

#include "matlab_source_impl.h"

using namespace gr;
using namespace gr::iio;

matlab_source::sptr matlab_source::make(const std::string &file)
{
	return gnuradio::get_initial_sptr(new matlab_source_impl(file));
}

matlab_source_impl::matlab_source_impl(const std::string &file) :
	gr::sync_block("matlab_source",
			gr::io_signature::make(0, 0, 0),
			gr::io_signature::make(0, 0, 0)),
	class_type(MAT_C_EMPTY)
{
	mat = Mat_Open(file.c_str(), MAT_ACC_RDONLY);
	if (!mat)
		throw std::runtime_error("Not a Matlab file");

	do {
		matvar_t *var = Mat_VarReadNextInfo(mat);
		if (!var)
			break;

		if (class_type == MAT_C_EMPTY) {
			class_type = var->class_type;
			is_complex = var->isComplex;
		} else if (class_type != var->class_type ||
				is_complex != var->isComplex) {
			Mat_VarFree(var);
			cleanup();
			throw std::runtime_error("Variables inside .mat file contain different data formats");
		}

		if (var->rank != 2 || (var->dims[0] > 1 && var->dims[1] > 1)) {
			Mat_VarFree(var);
			cleanup();
			throw std::runtime_error("Variables inside .mat file must be 1-dimentional vectors");
		}

		Mat_VarReadDataAll(mat, var);

		sizes.push_back(var->dims[0] * var->dims[1]);
		counters.push_back(0);
		vars.push_back(var);
	} while (true);

	data_size = Mat_SizeOfClass(class_type);

	switch (class_type) {
	case MAT_C_DOUBLE:
	case MAT_C_SINGLE:
	case MAT_C_UINT8:
	case MAT_C_INT8:
	case MAT_C_UINT16:
	case MAT_C_INT16:
	case MAT_C_UINT32:
	case MAT_C_INT32:
	case MAT_C_UINT64:
	case MAT_C_INT64:
		break;
	default:
		cleanup();
		throw std::runtime_error("Unsupported data class in .mat file: " + (int) class_type);
	}

	/* TODO: We don't support yet complex types. The .mat files store the
	 * Real and Imaginary parts as separate arrays instead of multiplexed in
	 * the same array, which makes supporting complex types more difficult.
	 */
	if (is_complex) {
		cleanup();
		throw std::runtime_error("Complex not yet supported");
	}

	if (is_complex)
		data_size *= 2;

	set_output_signature(gr::io_signature::make(1, vars.size(), data_size));
}

void matlab_source_impl::cleanup()
{
	for (unsigned int i = 0; i < vars.size(); i++)
		Mat_VarFree(vars[i]);

	Mat_Close(mat);
}

matlab_source_impl::~matlab_source_impl()
{
	cleanup();
}

int matlab_source_impl::work(int noutput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items)
{
	for (unsigned int i = 0; i < vars.size(); i++) {
		matvar_t *var = vars[i];
		size_t count = (size_t) noutput_items;

		/* Reset the counter if needed */
		if (counters[i] == sizes[i])
			counters[i] = 0;

		if (count + counters[i] > sizes[i])
			count = sizes[i] - counters[i];

		void *addr = (void *)((uintptr_t) var->data
				+ counters[i] * data_size);
		memcpy(output_items[i], addr, count * data_size);

		counters[i] += count;
		produce(i, count);
	}

	return WORK_CALLED_PRODUCE;
}
