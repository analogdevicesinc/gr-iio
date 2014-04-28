/* -*- c++ -*- */

#define IIO_API

%include "gnuradio.i"

%{
#include "gnuradio/iio/device.h"
%}

%include "gnuradio/iio/device.h"
GR_SWIG_BLOCK_MAGIC2(iio, device);
