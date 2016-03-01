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

#include "iio_math_impl.h"

#include <boost/lexical_cast.hpp>

#include <gnuradio/analog/sig_source_f.h>
#include <gnuradio/analog/sig_source_waveform.h>
#include <gnuradio/blocks/add_ff.h>
#include <gnuradio/blocks/copy.h>
#include <gnuradio/blocks/divide_ff.h>
#include <gnuradio/blocks/multiply_const_ff.h>
#include <gnuradio/blocks/multiply_ff.h>
#include <gnuradio/blocks/sub_ff.h>
#include <gnuradio/blocks/transcendental.h>
#include <gnuradio/iio/power_ff.h>
#include <gnuradio/io_signature.h>

using namespace gr;
using namespace gr::iio;

iio_math::sptr iio_math::make(const std::string &function)
{
	return gnuradio::get_initial_sptr(new iio_math_impl(function));
}

iio_math_impl::iio_math_impl(const std::string &function) : hier_block2("math",
		io_signature::make(1, 1, sizeof(float)),
		io_signature::make(1, 1, sizeof(float)))
{
	yyscan_t scanner;
	void *ptr;
	int ret;

	yylex_init_extra(this, &scanner);

	std::cout << "Function: " << function << std::endl;
	ptr = yy_scan_string(function.c_str(), scanner);

	ret = yyparse(scanner);

	yy_delete_buffer(ptr, scanner);

	if (ret)
		throw std::runtime_error("Invalid function");
}

iio_math_impl::~iio_math_impl()
{
	disconnect_all();
}

/* C functions */

void * src_block(void *pdata)
{
	struct iio_math_impl::block *block = new iio_math_impl::block;

	block->sptr = ((iio_math_impl *) pdata)->shared_from_this();
	return block;
}

void * const_block(void *pdata, double value)
{
	struct iio_math_impl::block *block = new iio_math_impl::block;

	block->sptr = analog::sig_source_f::make(0,
			analog::GR_CONST_WAVE, 0, 0, (float) value);
	return block;
}

void * neg_block(void *pdata, void *_input)
{
	iio_math_impl *m = (iio_math_impl *) pdata;
	struct iio_math_impl::block *input = (struct iio_math_impl::block *) _input;
	struct iio_math_impl::block *block = new iio_math_impl::block;

	block->sptr = blocks::multiply_const_ff::make(-1.0f);

	m->connect(input->sptr, 0, block->sptr, 0);
	return block;
}

void * add_block(void *pdata, void *_left, void *_right)
{
	struct iio_math_impl::block *block = new iio_math_impl::block;
	iio_math_impl *m = (iio_math_impl *) pdata;
	struct iio_math_impl::block *left = (struct iio_math_impl::block *) _left;
	struct iio_math_impl::block *right = (struct iio_math_impl::block *) _right;

	block->sptr = blocks::add_ff::make();

	m->connect(left->sptr, 0, block->sptr, 0);
	m->connect(right->sptr, 0, block->sptr, 1);
	return block;
}

void * sub_block(void *pdata, void *_left, void *_right)
{
	struct iio_math_impl::block *block = new iio_math_impl::block;
	iio_math_impl *m = (iio_math_impl *) pdata;
	struct iio_math_impl::block *left = (struct iio_math_impl::block *) _left;
	struct iio_math_impl::block *right = (struct iio_math_impl::block *) _right;

	block->sptr = blocks::sub_ff::make();

	m->connect(left->sptr, 0, block->sptr, 0);
	m->connect(right->sptr, 0, block->sptr, 1);
	return block;
}

void * mult_block(void *pdata, void *_left, void *_right)
{
	struct iio_math_impl::block *block = new iio_math_impl::block;
	iio_math_impl *m = (iio_math_impl *) pdata;
	struct iio_math_impl::block *left = (struct iio_math_impl::block *) _left;
	struct iio_math_impl::block *right = (struct iio_math_impl::block *) _right;

	block->sptr = blocks::multiply_ff::make();

	m->connect(left->sptr, 0, block->sptr, 0);
	m->connect(right->sptr, 0, block->sptr, 1);
	return block;
}

void * div_block(void *pdata, void *_left, void *_right)
{
	struct iio_math_impl::block *block = new iio_math_impl::block;
	iio_math_impl *m = (iio_math_impl *) pdata;
	struct iio_math_impl::block *left = (struct iio_math_impl::block *) _left;
	struct iio_math_impl::block *right = (struct iio_math_impl::block *) _right;

	block->sptr = blocks::divide_ff::make();

	m->connect(left->sptr, 0, block->sptr, 0);
	m->connect(right->sptr, 0, block->sptr, 1);
	return block;
}

void * pow_block(void *pdata, void *_left, void *_right)
{
	struct iio_math_impl::block *block = new iio_math_impl::block;
	iio_math_impl *m = (iio_math_impl *) pdata;
	struct iio_math_impl::block *left = (struct iio_math_impl::block *) _left;
	struct iio_math_impl::block *right = (struct iio_math_impl::block *) _right;

	block->sptr = iio::power_ff::make();

	m->connect(left->sptr, 0, block->sptr, 0);
	m->connect(right->sptr, 0, block->sptr, 1);
	return block;
}

void * func_block(void *pdata, void *_input, const char *name)
{
	iio_math_impl *m = (iio_math_impl *) pdata;
	struct iio_math_impl::block *input = (struct iio_math_impl::block *) _input;
	struct iio_math_impl::block *block = new iio_math_impl::block;

	std::string fname(name);
	block->sptr = blocks::transcendental::make(fname);

	m->connect(input->sptr, 0, block->sptr, 0);
	return block;
}

void connect_to_output(void *pdata, void *_block)
{
	struct iio_math_impl::block *block = (struct iio_math_impl::block *) _block;
	iio_math_impl *m = (iio_math_impl *) pdata;

	basic_block_sptr hier = m->shared_from_this();

	if (hier == block->sptr) {
		blocks::copy::sptr copy = blocks::copy::make(sizeof(float));

		/* Handle 'y = x' expression */
		m->connect(hier, 0, copy, 0);
		m->connect(copy, 0, hier, 0);
	} else {
		m->connect(block->sptr, 0, hier, 0);
	}
}

void delete_block(void *pdata, void *_block)
{
	struct iio_math_impl::block *block = (struct iio_math_impl::block *) _block;
	delete block;
}
