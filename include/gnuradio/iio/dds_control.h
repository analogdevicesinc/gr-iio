/* -*- c++ -*- */
/*
 * Copyright 2019 Analog Devices Inc.
 * Author: Travis Collins <travis.collins@analog.com>
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


#ifndef INCLUDED_IIO_DDS_CONTROL_H
#define INCLUDED_IIO_DDS_CONTROL_H

#include <gnuradio/block.h>
#include <gnuradio/iio/api.h>

namespace gr {
namespace iio {

/*!
 * \brief <+description of block+>
 * \ingroup iio
 *
 */
class IIO_API dds_control : virtual public gr::block
{
public:
    typedef boost::shared_ptr<dds_control> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of iio::dds_control.
     *
     * To avoid accidental use of raw pointers, iio::dds_control's
     * constructor is in a private implementation
     * class. iio::dds_control::make is the public interface for
     * creating new instances.
     */
    static sptr make(const std::string& uri,
                     std::vector<int> enabled,
                     std::vector<long> frequencies,
                     std::vector<float> phases,
                     std::vector<float> scales);

    virtual void set_dds_confg(std::vector<long> frequencies,
                               std::vector<float> phases,
                               std::vector<float> scales) = 0;
};

} // namespace iio
} // namespace gr

#endif /* INCLUDED_IIO_DDS_CONTROL_H */
