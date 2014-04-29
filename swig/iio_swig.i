/* -*- c++ -*- */

#define IIO_API

%include "gnuradio.i"

%{
#include "gnuradio/iio/device_source.h"
%}

%include "gnuradio/iio/device_source.h"
GR_SWIG_BLOCK_MAGIC2(iio, device_source);
