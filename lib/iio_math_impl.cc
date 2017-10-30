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
#include <gnuradio/blocks/null_sink.h>
#include <gnuradio/blocks/sub_ff.h>
#include <gnuradio/blocks/transcendental.h>
#include <gnuradio/iio/modulo_ff.h>
#include <gnuradio/iio/power_ff.h>
#include <gnuradio/io_signature.h>

using namespace gr;
using namespace gr::iio;

struct iio_math_impl::block {
	gr::basic_block_sptr sptr;
	unsigned int port;

	block() : port(0) {}
};

iio_math::sptr iio_math::make(const std::string &function, int ninputs)
{
	return gnuradio::get_initial_sptr(new iio_math_impl(function, ninputs));
}

iio_math_impl::iio_math_impl(const std::string &function, int ninputs) :
	hier_block2("math",
		io_signature::make(ninputs, ninputs, sizeof(float)),
		io_signature::make(1, 1, sizeof(float)))
{
	int ret;

	connected_ports.resize(ninputs);

	ret = parse_function(function);
	if (ret)
		throw std::runtime_error("Invalid function");

	connect_null_sinks();
	connected_ports.resize(0);

	cleanup();
}

iio_math_impl::~iio_math_impl()
{
	disconnect_all();
}

int iio_math_impl::parse_function(const std::string &function)
{
	yyscan_t scanner;
	void *ptr;
	int ret;

	yylex_init_extra(this, &scanner);

	ptr = yy_scan_string(function.c_str(), scanner);

	ret = yyparse(scanner);

	yy_delete_buffer(ptr, scanner);
	yylex_destroy(scanner);

	return ret;
}

void iio_math_impl::connect_null_sinks()
{
	basic_block_sptr hier = shared_from_this();

	for (unsigned int i = 0; i < connected_ports.size(); i++) {
		if (connected_ports[i] == false) {
			blocks::null_sink::sptr null_sink =
				blocks::null_sink::make(sizeof(float));
			connect(hier, i, null_sink, 0);
		}
	}
}

void iio_math_impl::cleanup()
{
	/* All blocks have been registered; we can delete the block list now */
	for (std::vector<struct block *>::iterator it = blocks.begin();
			it != blocks.end(); ++it)
		delete *it;

	blocks.clear();
}

void iio_math_impl::register_block(struct block *block)
{
	blocks.push_back(block);
}

gr::basic_block_sptr iio_math_impl::get_src_block()
{
	return shared_from_this();
}

void iio_math_impl::connect_to_output(gr::basic_block_sptr block, unsigned int port)
{
	basic_block_sptr hier = shared_from_this();

	if (hier == block) {
		blocks::copy::sptr copy = blocks::copy::make(sizeof(float));
		/* Handle 'y = x' expression */
		connect(hier, port, copy, 0);
		connect(copy, 0, hier, 0);
	} else {
		connect(block, 0, hier, 0);
	}
}

void iio_math_impl::set_port_used(unsigned int port)
{
	if (!connected_ports.empty())
		connected_ports[port] = true;
}

/* C functions */

void * src_block(void *pdata, unsigned int port)
{
	iio_math_impl *m = (iio_math_impl *) pdata;
	struct iio_math_impl::block *block = new iio_math_impl::block;

	block->sptr = m->get_src_block();
	block->port = port;
	m->set_port_used(port);
	m->register_block(block);
	return block;
}

void * const_block(void *pdata, double value)
{
	iio_math_impl *m = (iio_math_impl *) pdata;
	struct iio_math_impl::block *block = new iio_math_impl::block;

	block->sptr = analog::sig_source_f::make(0,
			analog::GR_CONST_WAVE, 0, 0, (float) value);
	m->register_block(block);
	return block;
}

void * neg_block(void *pdata, void *_input)
{
	iio_math_impl *m = (iio_math_impl *) pdata;
	struct iio_math_impl::block *input = (struct iio_math_impl::block *) _input;
	struct iio_math_impl::block *block = new iio_math_impl::block;

	block->sptr = blocks::multiply_const_ff::make(-1.0f);
	m->connect(input->sptr, input->port, block->sptr, 0);

	m->register_block(block);
	return block;
}

void * add_block(void *pdata, void *_left, void *_right)
{
	struct iio_math_impl::block *block = new iio_math_impl::block;
	iio_math_impl *m = (iio_math_impl *) pdata;
	struct iio_math_impl::block *left = (struct iio_math_impl::block *) _left;
	struct iio_math_impl::block *right = (struct iio_math_impl::block *) _right;

	block->sptr = blocks::add_ff::make();
	m->connect(left->sptr, left->port, block->sptr, 0);
	m->connect(right->sptr, right->port, block->sptr, 1);

	m->register_block(block);
	return block;
}

void * sub_block(void *pdata, void *_left, void *_right)
{
	struct iio_math_impl::block *block = new iio_math_impl::block;
	iio_math_impl *m = (iio_math_impl *) pdata;
	struct iio_math_impl::block *left = (struct iio_math_impl::block *) _left;
	struct iio_math_impl::block *right = (struct iio_math_impl::block *) _right;

	block->sptr = blocks::sub_ff::make();
	m->connect(left->sptr, left->port, block->sptr, 0);
	m->connect(right->sptr, right->port, block->sptr, 1);

	m->register_block(block);
	return block;
}

void * mult_block(void *pdata, void *_left, void *_right)
{
	struct iio_math_impl::block *block = new iio_math_impl::block;
	iio_math_impl *m = (iio_math_impl *) pdata;
	struct iio_math_impl::block *left = (struct iio_math_impl::block *) _left;
	struct iio_math_impl::block *right = (struct iio_math_impl::block *) _right;

	block->sptr = blocks::multiply_ff::make();
	m->connect(left->sptr, left->port, block->sptr, 0);
	m->connect(right->sptr, right->port, block->sptr, 1);

	m->register_block(block);
	return block;
}

void * div_block(void *pdata, void *_left, void *_right)
{
	struct iio_math_impl::block *block = new iio_math_impl::block;
	iio_math_impl *m = (iio_math_impl *) pdata;
	struct iio_math_impl::block *left = (struct iio_math_impl::block *) _left;
	struct iio_math_impl::block *right = (struct iio_math_impl::block *) _right;

	block->sptr = blocks::divide_ff::make();
	m->connect(left->sptr, left->port, block->sptr, 0);
	m->connect(right->sptr, right->port, block->sptr, 1);

	m->register_block(block);
	return block;
}

void * pow_block(void *pdata, void *_left, void *_right)
{
	struct iio_math_impl::block *block = new iio_math_impl::block;
	iio_math_impl *m = (iio_math_impl *) pdata;
	struct iio_math_impl::block *left = (struct iio_math_impl::block *) _left;
	struct iio_math_impl::block *right = (struct iio_math_impl::block *) _right;

	block->sptr = iio::power_ff::make();
	m->connect(left->sptr, left->port, block->sptr, 0);
	m->connect(right->sptr, right->port, block->sptr, 1);

	m->register_block(block);
	return block;
}

void * mod_block(void *pdata, void *_left, void *_right)
{
	struct iio_math_impl::block *block = new iio_math_impl::block;
	iio_math_impl *m = (iio_math_impl *) pdata;
	struct iio_math_impl::block *left = (struct iio_math_impl::block *) _left;
	struct iio_math_impl::block *right = (struct iio_math_impl::block *) _right;

	block->sptr = iio::modulo_ff::make();
	m->connect(left->sptr, left->port, block->sptr, 0);
	m->connect(right->sptr, right->port, block->sptr, 1);

	m->register_block(block);
	return block;
}

void * func_block(void *pdata, void *_input, const char *name)
{
	iio_math_impl *m = (iio_math_impl *) pdata;
	struct iio_math_impl::block *input = (struct iio_math_impl::block *) _input;
	struct iio_math_impl::block *block = new iio_math_impl::block;
	std::string fname(name);

	block->sptr = blocks::transcendental::make(fname);
	m->connect(input->sptr, input->port, block->sptr, 0);

	m->register_block(block);
	return block;
}

void connect_to_output(void *pdata, void *_block)
{
	struct iio_math_impl::block *block = (struct iio_math_impl::block *) _block;
	iio_math_impl *m = (iio_math_impl *) pdata;

	m->connect_to_output(block->sptr, block->port);
}

void delete_block(void *pdata, void *_block)
{
	struct iio_math_impl::block *block = (struct iio_math_impl::block *) _block;
	delete block;
}
