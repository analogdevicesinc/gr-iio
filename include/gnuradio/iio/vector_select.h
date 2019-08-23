/* -*- c++ -*- */
/*
 * Copyright 2019 Travis F. Collins <travis.collins@analog.com>.
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


#ifndef INCLUDED_IIO_VECTOR_SELECT_H
#define INCLUDED_IIO_VECTOR_SELECT_H

#include <gnuradio/iio/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace iio {

    /*!
     * \brief <+description of block+>
     * \ingroup iio
     *
     */
    class IIO_API vector_select : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<vector_select> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of iio::vector_select.
       *
       * To avoid accidental use of raw pointers, iio::vector_select's
       * constructor is in a private implementation
       * class. iio::vector_select::make is the public interface for
       * creating new instances.
       */
      static sptr make(unsigned int vlen);
    };

  } // namespace iio
} // namespace gr

#endif /* INCLUDED_IIO_VECTOR_SELECT_H */
