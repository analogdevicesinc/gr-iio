/* -*- c++ -*- */
/* 
 * Copyright 2014 Analog Devices Inc.
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


#ifndef INCLUDED_IIO_FMCOMMS2_SOURCE_H
#define INCLUDED_IIO_FMCOMMS2_SOURCE_H

#include <gnuradio/iio/api.h>
#include <gnuradio/hier_block2.h>
#include <gnuradio/sync_block.h>

#include "device_source.h"

namespace gr {
  namespace iio {

    /*!
     * \brief <+description of block+>
     * \ingroup iio
     *
     */
    class IIO_API fmcomms2_source : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<fmcomms2_source> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of iio::device.
       *
       * To avoid accidental use of raw pointers, iio::device's
       * constructor is in a private implementation
       * class. iio::device::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::string &uri, unsigned long long frequency,
		    unsigned long samplerate,
		    unsigned long bandwidth,
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    unsigned long buffer_size, bool quadrature, bool rfdc,
		    bool bbdc, const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value,
		    const char *rf_port_select, const char *filter = "",
		    bool auto_filter = true);

      static sptr make_from(struct iio_context *ctx,
		    unsigned long long frequency, unsigned long samplerate,
		    unsigned long bandwidth,
		    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en,
		    unsigned long buffer_size, bool quadrature, bool rfdc,
		    bool bbdc, const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value,
		    const char *rf_port_select, const char *filter = "",
		    bool auto_filter = true);

      virtual void set_params(unsigned long long frequency,
		      unsigned long samplerate, unsigned long bandwidth,
		      bool quadrature, bool rfdc, bool bbdc,
		      const char *gain1, double gain1_value,
		      const char *gain2, double gain2_value,
		      const char *rf_port_select,
		      const char *filter = "", bool auto_filter = true) = 0;
    };

    class IIO_API fmcomms2_source_f32c : virtual public gr::hier_block2
    {
    public:
      typedef boost::shared_ptr<fmcomms2_source_f32c> sptr;

      static sptr make(const std::string &uri,
		    unsigned long long frequency, unsigned long samplerate,
		    unsigned long bandwidth,
		    bool rx1_en, bool rx2_en,
		    unsigned long buffer_size, bool quadrature, bool rfdc,
		    bool bbdc, const char *gain1, double gain1_value,
		    const char *gain2, double gain2_value,
		    const char *rf_port_select, const char *filter = "",
		    bool auto_filter = true)
      {
	      fmcomms2_source::sptr block = fmcomms2_source::make(uri,
			      frequency, samplerate,
			      bandwidth, rx1_en, rx1_en, rx2_en,
			      rx2_en, buffer_size, quadrature,
			      rfdc, bbdc, gain1, gain1_value,
			      gain2, gain2_value, rf_port_select,
			      filter, auto_filter);

	      return gnuradio::get_initial_sptr(
			      new fmcomms2_source_f32c(rx1_en, rx2_en, block));
      }

      void set_params(unsigned long long frequency,
		      unsigned long samplerate, unsigned long bandwidth,
		      bool quadrature, bool rfdc, bool bbdc,
		      const char *gain1, double gain1_value,
		      const char *gain2, double gain2_value,
		      const char *rf_port_select,
		      const char *filter = "", bool auto_filter = true)
      {
              fmcomms2_block->set_params(frequency, samplerate, bandwidth,
                              quadrature, rfdc, bbdc, gain1, gain1_value,
                              gain2, gain2_value, rf_port_select, filter,
                              auto_filter);
      }
    private:
      fmcomms2_source::sptr fmcomms2_block;

    protected:
      explicit fmcomms2_source_f32c(bool rx1_en, bool rx2_en,
		      fmcomms2_source::sptr block);
    };

  } // namespace iio
} // namespace gr

#endif /* INCLUDED_IIO_FMCOMMS2_SOURCE_H */

