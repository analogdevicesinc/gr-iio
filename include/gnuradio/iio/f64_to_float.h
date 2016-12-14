/* -*- c++ -*- */
/*
 * Copyright 2016 Analog Devices Inc.
 * Author: Paul Cercueil <paul.cercueil@analog.com>
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

#ifndef INCLUDED_IIO_F64_TO_FLOAT_H
#define INCLUDED_IIO_F64_TO_FLOAT_H

#include <gnuradio/iio/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace iio {

    /*!
     * \brief Convert stream of doubles to a stream of floats
     * \ingroup type_converters_blk
     */
    class IIO_API f64_to_float : virtual public sync_block
    {
    public:
      // gr::iio::f64_to_float_ff::sptr
      typedef boost::shared_ptr<f64_to_float> sptr;

      /*!
       * Build a double to float block.
       *
       * \param vlen vector length of data streams.
       */
      static sptr make(size_t vlen=1);
    };

  } /* namespace iio */
} /* namespace gr */

#endif /* INCLUDED_IIO_F64_TO_FLOAT_H */
