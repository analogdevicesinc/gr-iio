/* -*- c++ -*- */
/* 
 * Copyright 2017 Analog Devices Inc.
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


#ifndef INCLUDED_IIO_PLUTO_SINK_H
#define INCLUDED_IIO_PLUTO_SINK_H

#include <gnuradio/iio/api.h>
#include <gnuradio/hier_block2.h>

namespace gr {
  namespace iio {
	/*!
	 * \brief Sink block for the PlutoSDR
	 * \ingroup iio
	 *
	 */
	class IIO_API pluto_sink : virtual public gr::hier_block2
	{
	public:
		typedef boost::shared_ptr<pluto_sink> sptr;

		static sptr make(const std::string &uri,
				unsigned long long frequency,
				unsigned long samplerate,
				unsigned long bandwidth,
				unsigned long buffer_size,
				bool cyclic,
				double attenuation,
				const char *filter = "",
				bool auto_filter = true);

		virtual void set_params(unsigned long long frequency,
				unsigned long samplerate,
				unsigned long bandwidth,
				double attenuation,
				const char *filter = "",
				bool auto_filter = true) = 0;
	};

        } // namespace iio
} // namespace gr

#endif /* INCLUDED_IIO_PLUTO_SINK_H */
