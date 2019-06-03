/* -*- c++ -*- */

#define IIO_API

%include "gnuradio.i"

%{
#include "iio/device_source.h"
#include "iio/device_sink.h"
#include "iio/fmcomms2_source.h"
#include "iio/fmcomms2_sink.h"
#include "iio/fmcomms5_source.h"
#include "iio/fmcomms5_sink.h"
#include "iio/math.h"
#include "iio/power_ff.h"
#include "iio/modulo_ff.h"
#include "iio/modulo_const_ff.h"
#include "iio/attr_sink.h"
#include "iio/attr_source.h"
%}

%include "iio/device_source.h"
%include "iio/device_sink.h"
%include "iio/fmcomms2_source.h"
%include "iio/fmcomms2_sink.h"
%include "iio/fmcomms5_source.h"
%include "iio/fmcomms5_sink.h"
%include "iio/math.h"
%include "iio/power_ff.h"
%include "iio/modulo_ff.h"
%include "iio/modulo_const_ff.h"
%include "iio/attr_sink.h"
%include "iio/attr_source.h"

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
GR_SWIG_BLOCK_MAGIC2(iio, attr_sink);
GR_SWIG_BLOCK_MAGIC2(iio, attr_source);
