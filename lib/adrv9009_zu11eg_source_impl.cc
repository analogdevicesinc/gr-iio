/* -*- c++ -*- */
/*
 * Copyright 2019 Analog Devices Inc.
 * Author: Travis F. Collins <travis.collins@analog.com>
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

#include <cstdio>

#include "adrv9009_zu11eg_source_impl.h"
#include <gnuradio/io_signature.h>

#include <gnuradio/blocks/float_to_complex.h>
#include <gnuradio/blocks/short_to_float.h>

using namespace gr::blocks;

namespace gr {
namespace iio {

bool adrv9009_zu11eg_source_impl::load_profile(std::string &filter,
                                         struct iio_device *phy) {
  if (filter.empty() || !iio_device_find_attr(phy, "profile_config"))
    return false;

  std::ifstream ifs(filter.c_str(), std::ifstream::binary);
  if (!ifs)
    return false;

  /* Here, we verify that the filter file contains data for both RX+TX. */
  {
    char buf[256];

    do {
      ifs.getline(buf, sizeof(buf));
    } while (!(buf[0] == '-' || (buf[0] >= '0' && buf[0] <= '9')));

    std::string line(buf);
    if (line.find(',') == std::string::npos)
      throw std::runtime_error("Incompatible profile");
  }

  ifs.seekg(0, ifs.end);
  int length = ifs.tellg();
  ifs.seekg(0, ifs.beg);

  char *buffer = new char[length];

  ifs.read(buffer, length);
  ifs.close();

  int ret = iio_device_attr_write_raw(phy, "profile_config", buffer, length);

  delete[] buffer;
  return ret > 0;
}

adrv9009_zu11eg_source_f32c::adrv9009_zu11eg_source_f32c(
    bool rx1_en, bool rx2_en, bool rx3_en, bool rx4_en,
    adrv9009_zu11eg_source::sptr src_block)
    : hier_block2("adrv9009_zu11eg_f32c", io_signature::make(0, 0, 0),
                  io_signature::make(
                      (int)rx1_en + (int)rx2_en + (int)rx3_en + (int)rx4_en,
                      (int)rx1_en + (int)rx2_en + (int)rx3_en + (int)rx4_en,
                      sizeof(gr_complex))),
      adrv9009_zu11eg_block(src_block) {
  basic_block_sptr hier = shared_from_this();
  unsigned int num_streams =
      (int)rx1_en + (int)rx2_en + (int)rx3_en + (int)rx4_en;

  for (unsigned int i = 0; i < num_streams; i++) {
    short_to_float::sptr s2f1 = short_to_float::make(1, 32768.0);
    short_to_float::sptr s2f2 = short_to_float::make(1, 32768.0);
    float_to_complex::sptr f2c = float_to_complex::make(1);

    connect(src_block, i * 2, s2f1, 0);
    connect(src_block, i * 2 + 1, s2f2, 0);
    connect(s2f1, 0, f2c, 0);
    connect(s2f2, 0, f2c, 1);
    connect(f2c, 0, hier, i);
  }
}

adrv9009_zu11eg_source::sptr adrv9009_zu11eg_source::make(
    const std::string &uri, unsigned long long frequency1,
    unsigned long long frequency2, bool ch1_en, bool ch2_en, bool ch3_en,
    bool ch4_en, bool ch5_en, bool ch6_en, bool ch7_en, bool ch8_en,
    unsigned long buffer_size, bool quadrature, const char *gain1,
    double gain1_value, const char *gain2, double gain2_value,
    const char *gain3, double gain3_value, const char *gain4,
    double gain4_value, const char *filter) {
  return gnuradio::get_initial_sptr(new adrv9009_zu11eg_source_impl(
      device_source_impl::get_context(uri), true, frequency1, frequency2,
      ch1_en, ch2_en, ch3_en, ch4_en, ch5_en, ch6_en, ch7_en, ch8_en,
      buffer_size, quadrature, gain1, gain1_value, gain2, gain2_value, gain3,
      gain3_value, gain4, gain4_value, filter));
}

adrv9009_zu11eg_source::sptr adrv9009_zu11eg_source::make_from(
    struct iio_context *ctx, unsigned long long frequency1,
    unsigned long long frequency2, bool ch1_en, bool ch2_en, bool ch3_en,
    bool ch4_en, bool ch5_en, bool ch6_en, bool ch7_en, bool ch8_en,
    unsigned long buffer_size, bool quadrature, const char *gain1,
    double gain1_value, const char *gain2, double gain2_value,
    const char *gain3, double gain3_value, const char *gain4,
    double gain4_value, const char *filter) {
  return gnuradio::get_initial_sptr(new adrv9009_zu11eg_source_impl(
      ctx, false, frequency1, frequency2, ch1_en, ch2_en, ch3_en, ch4_en,
      ch5_en, ch6_en, ch7_en, ch8_en, buffer_size, quadrature, gain1,
      gain1_value, gain2, gain2_value, gain3, gain3_value, gain4, gain4_value,
      filter));
}

std::vector<std::string> adrv9009_zu11eg_source_impl::get_channels_vector(
    bool ch1_en, bool ch2_en, bool ch3_en, bool ch4_en, bool ch5_en,
    bool ch6_en, bool ch7_en, bool ch8_en) {
  std::vector<std::string> channels;
  if (ch1_en)
    channels.push_back("voltage0_i");
  if (ch2_en)
    channels.push_back("voltage0_q");
  if (ch3_en)
    channels.push_back("voltage1_i");
  if (ch4_en)
    channels.push_back("voltage1_q");
  if (ch5_en)
    channels.push_back("voltage2_i");
  if (ch6_en)
    channels.push_back("voltage2_q");
  if (ch7_en)
    channels.push_back("voltage3_i");
  if (ch8_en)
    channels.push_back("voltage3_q");
  return channels;
}

adrv9009_zu11eg_source_impl::adrv9009_zu11eg_source_impl(
    struct iio_context *ctx, bool destroy_ctx, unsigned long long frequency1,
    unsigned long long frequency2, bool ch1_en, bool ch2_en, bool ch3_en,
    bool ch4_en, bool ch5_en, bool ch6_en, bool ch7_en, bool ch8_en,
    unsigned long buffer_size, bool quadrature, const char *gain1,
    double gain1_value, const char *gain2, double gain2_value,
    const char *gain3, double gain3_value, const char *gain4,
    double gain4_value, const char *filter)
    : gr::sync_block("adrv9009_zu11eg_source", gr::io_signature::make(0, 0, 0),
                     gr::io_signature::make(1, -1, sizeof(short))),
      device_source_impl(ctx, destroy_ctx, "axi-adrv9009-rx-hpc",
                         get_channels_vector(ch1_en, ch2_en, ch3_en, ch4_en,
                                             ch5_en, ch6_en, ch7_en, ch8_en),
                         "adrv9009-phy", std::vector<std::string>(),
                         buffer_size, 0),
      samplerate(0) {
  phy2 = iio_context_find_device(ctx, "adrv9009-phy-b");
  if (!phy2)
    throw std::runtime_error("Device 'adrv9009-phy-b' not found");

  // Power down obs
  std::vector<std::string> params1;
  params1.push_back("in_voltage2_powerdown=1");
  params1.push_back("in_voltage3_powerdown=1");
  device_source_impl::set_params(this->phy, params1);
  device_source_impl::set_params(this->phy2, params1);

  // Power up receivers
  std::vector<std::string> params2;
  params2.push_back("in_voltage0_powerdown=0");
  params2.push_back("in_voltage1_powerdown=0");
  device_source_impl::set_params(this->phy, params2);
  device_source_impl::set_params(this->phy2, params2);

  // Set rest
  set_params(frequency1, frequency2, quadrature, gain1, gain1_value, gain2,
             gain2_value, gain3, gain3_value, gain4, gain4_value);

  std::string filt(filter);
  if (!filt.empty() &&
      !(load_profile(filt, phy) && load_profile(filt, phy2)))
    throw std::runtime_error("Unable to load filter file");
}

void adrv9009_zu11eg_source_impl::set_params(struct iio_device *phy_device,
                                             unsigned long long frequency,
                                             bool quadrature, const char *gain1,
                                             double gain1_value,
                                             const char *gain2,
                                             double gain2_value) {
  std::vector<std::string> params;

  params.push_back("out_altvoltage0_TRX_LO_frequency=" +
                   boost::to_string(frequency));
  params.push_back("in_voltage0_quadrature_tracking_en=" +
                   boost::to_string(quadrature));
  params.push_back("in_voltage1_quadrature_tracking_en=" +
                   boost::to_string(quadrature));
  params.push_back("in_voltage0_gain_control_mode=" + boost::to_string(gain1));
  params.push_back("in_voltage0_hardwaregain=" + boost::to_string(gain1_value));
  params.push_back("in_voltage1_hardwaregain=" + boost::to_string(gain2_value));

  device_source_impl::set_params(phy_device, params);
}

void adrv9009_zu11eg_source_impl::set_params(
    unsigned long long frequency1, unsigned long long frequency2,
    bool quadrature, const char *gain1, double gain1_value, const char *gain2,
    double gain2_value, const char *gain3, double gain3_value,
    const char *gain4, double gain4_value) {
  set_params(this->phy, frequency1, quadrature, gain1, gain1_value, gain2,
             gain2_value);
  set_params(this->phy2, frequency2, quadrature, gain3, gain3_value, gain4,
             gain4_value);

  // if (this->samplerate != samplerate) {
  //   ad9361_adrv9009_zu11eg_multichip_sync(ctx, FIXUP_INTERFACE_TIMING |
  // 	    CHECK_SAMPLE_RATES);
  //   this->samplerate = samplerate;
  // }
}

} /* namespace iio */
} /* namespace gr */
