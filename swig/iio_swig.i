/* -*- c++ -*- */

#define IIO_API

%include "gnuradio.i"

%{
#include "gnuradio/iio/device_source.h"
#include "gnuradio/iio/device_sink.h"
#include "gnuradio/iio/fmcomms2_source.h"
#include "gnuradio/iio/fmcomms2_sink.h"
#include "gnuradio/iio/fmcomms5_source.h"
#include "gnuradio/iio/fmcomms5_sink.h"
#include "gnuradio/iio/math.h"
#include "gnuradio/iio/power_ff.h"
#include "gnuradio/iio/modulo_ff.h"
#include "gnuradio/iio/modulo_const_ff.h"
%}

%include "gnuradio/iio/device_source.h"
%include "gnuradio/iio/device_sink.h"
%include "gnuradio/iio/fmcomms2_source.h"
%include "gnuradio/iio/fmcomms2_sink.h"
%include "gnuradio/iio/fmcomms5_source.h"
%include "gnuradio/iio/fmcomms5_sink.h"
%include "gnuradio/iio/math.h"
%include "gnuradio/iio/power_ff.h"
%include "gnuradio/iio/modulo_ff.h"
%include "gnuradio/iio/modulo_const_ff.h"

GR_SWIG_BLOCK_MAGIC2(iio, device_source);
GR_SWIG_BLOCK_MAGIC2(iio, device_sink);
GR_SWIG_BLOCK_MAGIC2(iio, fmcomms2_source);
GR_SWIG_BLOCK_MAGIC2(iio, fmcomms2_source_f32c);
GR_SWIG_BLOCK_MAGIC2(iio, fmcomms2_sink);
GR_SWIG_BLOCK_MAGIC2(iio, fmcomms2_sink_f32c);
GR_SWIG_BLOCK_MAGIC2(iio, fmcomms5_source);
GR_SWIG_BLOCK_MAGIC2(iio, fmcomms5_source_f32c);
GR_SWIG_BLOCK_MAGIC2(iio, fmcomms5_sink);
GR_SWIG_BLOCK_MAGIC2(iio, fmcomms5_sink_f32c);
GR_SWIG_BLOCK_MAGIC2(iio, iio_math);
GR_SWIG_BLOCK_MAGIC2(iio, iio_math_gen);
GR_SWIG_BLOCK_MAGIC2(iio, power_ff);
GR_SWIG_BLOCK_MAGIC2(iio, modulo_ff);
GR_SWIG_BLOCK_MAGIC2(iio, modulo_const_ff);
